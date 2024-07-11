#include <stdrom.h>
#include <define.h>
#include <libc.h>
#include <L_libc.h>

#include "modehook.h"

#define OBJ_IC_PAD 0x9000
#define OBJ_IC_MENU 0x9001

TCHTBL TchList[] = {
  0,0,159,149,
  ACT_MOVE | ACT_MAKE | ACT_BREAK_IN | ACT_MOVE_OUT,
  OBJ_IC_PAD,
  0x0000,

  0,150,159,159,
  ACT_MOVE | ACT_MAKE | ACT_BREAK_IN | ACT_MOVE_OUT,
  OBJ_IC_MENU,
  0x0000,

  0, 0, 0, 0,
  ACT_NONE,
  OBJ_END,
  0x0000
};

TCHSTS g_tch;

/* Status data */

typedef struct {
  word line;
  word lines;
  word buf_size;
  byte pen_type;
  byte pen_mesh;
  byte pen_size;
  byte eraser_size;
  byte extension[128];
} t_stat;

#define MAX_BUFFER_LEN (30720-sizeof(t_stat))

byte g_scr[3004];
SHOT_INF g_shot;

byte call_shot;

#define LINE_COMPRESSED 0xE0
#define LINE_FREE 0xC0
#define BUFFER_END 0xFF

#define PEN_ERASE 0
#define PEN_DRAW 1
#define PEN_CORRECT 2

byte far g_pen[9][8] = {
  {GSIZE(1, 1), 0xff, 0x00, 0x00, 0x00},
  {GSIZE(2, 2), 0xFF, 0xFF, 0x00, 0x00},
  {GSIZE(4, 4), 0x6f, 0xff, 0xff, 0x6f},

  {GSIZE(1, 1), 0x00, 0x00, 0x00, 0x00},
  {GSIZE(2, 2), 0x5f, 0xAf, 0x00, 0x00},
  {GSIZE(4, 4), 0x4f, 0xAA, 0x55, 0x2f},

  {GSIZE(1, 1), 0xff, 0x00, 0x00, 0x00},
  {GSIZE(2, 2), 0xAf, 0x5f, 0x00, 0x00},
  {GSIZE(4, 4), 0x2f, 0x5f, 0xAf, 0x4f},

};

word far g_pen_icon[2][3] = {
  {152, 150, 151},
  {155, 153, 154}
};

word far g_mesh_icon[] = { 160, 161 };

byte g_bit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

byte g_correct_icon[] = {
	GSIZE(18, 9),
	0x00, 0xC0, 0x3F,
	0x03, 0xF0, 0x3F,
	0x0F, 0xFC, 0x3F,
	0x00, 0x00, 0x3F,
	0x7F, 0xFF, 0xBF,
	0x00, 0x00, 0x3F,
	0x0F, 0xFC, 0x3F,
	0x03, 0xF0, 0x3F,
	0x00, 0xC0, 0x3F
};

byte g_undo_icon[] = {
	GSIZE(18, 9),
	0x02, 0x00, 0x3F,
	0x06, 0x00, 0x3F,
	0x0F, 0xF8, 0x3F,
	0x0F, 0xFC, 0x3F,
	0x06, 0x0E, 0x3F,
	0x02, 0x06, 0x3F,
	0x00, 0x0E, 0x3F,
	0x0F, 0xFC, 0x3F,
	0x0F, 0xF8, 0x3F
};

LFILE_BUF far fb;
FILE_INFO finf;
byte far *const g_buf = fb.fbuf.bin.bin_buf + sizeof(t_stat);
t_stat far *const g_stat = (t_stat far *const)fb.fbuf.bin.bin_buf;
byte mode;
byte submode;
byte new_file;
word first_memo;
word sel_memo;

int Sub(int sub)
{
  return sub & 0xf;
}

int Info(int sub)
{
  if (sub>15) return 0;
  return 0x80;
}

char memo_init() {
  if (!LibGetAllEntry("IDraw", &mode, &submode)) {
    if (!LibSubEntrySave("IDraw", &submode)) return 0;
    LibGetAllEntry("IDraw", &mode, &submode);
  }
  return 1;
}

char memo_load_first(void) {
  fb.fsb_main_entry_ = mode;
  fb.fsb_sub_entry_ = submode;
  fb.fsb_scrt_info_ = 0x80;
  finf.fp = 0xFFFF;
  finf.kind = FILE_KIND_BIN;
  if (!LibLFileFindNext(&fb, &finf, 0x00)) return 0;
  if (!LibLFileRead(&fb, &finf)) return 0;
  g_stat->buf_size = fb.fbuf.bin.char_num-1-sizeof(t_stat);
  return 1;
}

char memo_load_next(void) {
  if (!LibLFileFindNext(&fb, &finf, 0x00)) return 0;
  if (!LibLFileRead(&fb, &finf)) return 0;
  g_stat->buf_size = fb.fbuf.bin.char_num-1-sizeof(t_stat);
  return 1;
}

char memo_save_new(void) {
  fb.fsb_main_entry_ = mode;
  fb.fsb_sub_entry_ = submode;
  fb.fsb_scrt_info_ = 0x80;
  fb.fbuf.bin.char_num = g_stat->buf_size+1+sizeof(t_stat);
  finf.fp = 0xFFFF;
  finf.kind = FILE_KIND_BIN;
  if (!LibLFileWrite(&fb, &finf)) return 0;
  new_file = 0;
  return 1;
}

char memo_save_curr(void) {
  fb.fbuf.bin.char_num = g_stat->buf_size+1+sizeof(t_stat);
  return LibLFileWrite(&fb, &finf);
}

char memo_save(void) {
  if (new_file) return memo_save_new();
  else return memo_save_curr();
}

char memo_delete(word num) {
  int i;
  fb.fsb_main_entry_ = mode;
  fb.fsb_sub_entry_ = submode;
  fb.fsb_scrt_info_ = 0x80;
  finf.fp = 0xFFFF;
  finf.kind = FILE_KIND_BIN;

  if (!LibLFileFindNext(&fb, &finf, 0x00)) return 0;
  for (i=0; i<num; i++) {
    if (!LibLFileFindNext(&fb, &finf, 0x00)) return 0;
  }
  LibLFileRemove(&fb, &finf);
  return 1;
}

int far getleverpush();

void wait_release(void) {
  do {
    LibTchWait(&g_tch);
  } while(g_tch.act != ACT_BREAK_IN);
}

void wait_press(void) {
  do {
    LibTchWait(&g_tch);
  } while(g_tch.act != ACT_MAKE);
}

void wait_move(void) {
  LibTchWait(&g_tch);
}


word pad_get_line(word line_num, byte far **line) {
  word pos = 0;
  int i;
  for (i=0; i<line_num; i++) {
    byte b = g_buf[pos];
    if (b == BUFFER_END) {
      *line = &g_buf[pos];
      return 0;
    }
    pos += (b & 0x1F);
  }
  *line = &g_buf[pos];
  return pos;
}

byte pad_get_next_line(byte far **line) {
  byte len;
  if (**line == BUFFER_END) return 0;
  len = **line & 0x1F;
  *line += len;
  return len;
}

byte pad_compress_line(byte far *dest, byte far* src) {
  byte pos=1;
  byte len=0;
  byte i;
  byte curr=0;

  if (src[0] & 0x80) {
    dest[pos++] = 0;
    curr=1;
  }
  for (i=0; i<20; i++) {
    byte s = src[i];
    if (s == (curr ? 0xFF : 0)) {
      len += 8;
    } else {
      byte j;
      for (j=0; j<8; j++) {
        if ((s & 0x80) == (curr ? 0x0 : 0x80)) {
          curr = 1-curr;
          if (pos == 20) goto nocompress;
          dest[pos++] = len;
          len=0;
        } 
	s <<= 1;
        len++;
      }
    }
  }
  dest[0] = LINE_COMPRESSED | pos;
  return pos;

  nocompress:
  dest[0] = LINE_FREE | 21;
  far_memmove(&dest[1], src, 20);
  return 21;
}

void pad_decompress_line(byte far *dest, byte far *src) {
  byte s = src[0];
  if ((s & 0xE0) == LINE_COMPRESSED) {
    byte len=s & 0x1F;
    byte i=1;
    byte pos=0;
    s=src[i++];
    far_memset(dest, 0, 20);
    if (len==1) return;
    if (s) {
      pos = s;
    } else {
      if (len==2) {
	far_memset(dest, 0xFF, 20);
        return;
      }
    }
    s=src[i];
    do {
      byte start_pos = pos;
      byte stop_pos = pos+s;
      byte start_byte = start_pos >> 3;
      byte stop_byte = stop_pos >> 3;
      byte start_bit = start_pos & 0x07;
      byte stop_bit = stop_pos & 0x07;
      if (start_byte == stop_byte) {
	dest[start_byte] |= (0xFF >> (8-stop_bit+start_bit)) << (8-stop_bit);
      } else {
        byte j;
        byte curr_byte = start_byte;
	dest[curr_byte++] |= 0xFF >> start_bit;
	for (j=0; j<stop_byte-start_byte-1; j++) {
	  dest[curr_byte++] = 0xFF;
	}
	dest[curr_byte++] |= 0xFF << (8-stop_bit);
      }
      pos+=s;
      i++;
      if (i<len) {
        pos += src[i++];
	if (i==len) { s = 160-pos; i--;  }
        else s = src[i];
      }
    } while(i<len);
  } else {
    far_memmove(dest, &src[1], 20);
  }
}

void pad_get_scr(void) {
  byte far *line;
  byte i=0;
  pad_get_line(g_stat->line, &line);
  memset(&g_scr[4], 0, 150*20);
  while(*line != 0xFF && i<150) {
    pad_decompress_line(&g_scr[4+i*20], line);
    pad_get_next_line(&line);
    i++;
  }
  if (i<150) {
    byte t = 150-i;
    far_memset(line, 0xE1, t);
    line[t] = 0xFF;
    g_stat->lines += t;
    g_stat->buf_size += t;
  }
}

char pad_save_scr(void) {
  byte buffer[150*21];
  word scr_size = 0;
  word old_size = 0;
  byte i;
  byte far *start_line;
  byte far *line;
  word pos = pad_get_line(g_stat->line, &line);
  start_line = line;
  for (i=0; i<150; i++) {
    byte size = pad_compress_line(&buffer[scr_size], &g_scr[4+i*20]);
    scr_size += size;
    old_size += pad_get_next_line(&line);
  }
  if (g_stat->buf_size + scr_size - old_size > MAX_BUFFER_LEN) {
    pad_get_scr();
    return 0;
  }
  if (*line == BUFFER_END) {
    start_line[scr_size] = BUFFER_END;
    g_stat->lines = g_stat->line + 150;
  } else {
    far_memmove(&start_line[scr_size], line, g_stat->buf_size-pos-old_size+1);
  }
  far_memmove(start_line, buffer, scr_size);
  g_stat->buf_size -= old_size;
  g_stat->buf_size += scr_size;

  return 1;
}

void pad_draw(byte line) {
  LibPutGraph(0, 0, g_scr);
  if (g_stat->line < 30 && line) {
    byte y = 30 - g_stat->line;
    LibGdsLineMesh(0, y, 159, y);
  }
}

void pad_insert_lines(byte y, byte h) {
  byte far *line;
  word abs_y = g_stat->line + y;
  word pos = pad_get_line(abs_y, &line);
  if (g_stat->buf_size + h < MAX_BUFFER_LEN) {
    far_memmove(line+h, line, g_stat->buf_size - pos + 1);
    far_memset(line, 0xE1, h);
    g_stat->buf_size += h;
    g_stat->lines += h;
  }
}

void pad_delete_lines(byte y, byte h) {
  byte far *line1;
  byte far *line2;
  word abs_y = g_stat->line + y;
  if (y >= h) {
    word pos1 = pad_get_line(abs_y - h, &line1);
    word pos2 = pad_get_line(abs_y, &line2);
    far_memmove(line1, line2, g_stat->buf_size - pos2 + 1);
    g_stat->buf_size -= pos2-pos1;
    g_stat->lines -= h;
  }
}

/*
void file_draw_header(byte y) {

}

byte file_draw(word file) {
  word i;
  if (!memo_load_first()) return 0;
  for (i=1; i<num; i++) {
    if (!memo_load_next()) break;
  }
  for (i=0)
}

void file_check(void) {

}

*/

void pad_init(void) {
  g_stat->line = 0;
  g_stat->pen_type = PEN_DRAW;
  g_stat->pen_size = 1;
  g_stat->pen_mesh = 0;
  g_stat->eraser_size = 2;
  g_stat->lines = 0;
  g_scr[0] = 160;
  g_scr[1] = 0;
  g_scr[2] = 150;
  g_scr[3] = 0;
  g_buf[0] = 0xFF;
  g_stat->buf_size = 0;
  memset(&g_scr[4], 0, 3000);
}

void menu_draw_stat(void) {
  byte str[40];
  sprintf(str,"%5.1d/%5.1d %5.1d", g_stat->line, g_stat->lines, g_stat->buf_size);
  LibPut35Str(91, 154, str);
}

void menu_draw(void) {
  LibGdsClr(0, 150, 159, 159);
  LibPutFarData(0, 150, g_pen_icon[0][g_stat->pen_size]);
  LibPutFarData(18, 150, g_pen_icon[1][g_stat->eraser_size]);
  LibPutGraph(36, 150, g_correct_icon);
  LibPutFarData(54, 150, g_mesh_icon[g_stat->pen_mesh]);
  LibPutGraph(72, 150, g_undo_icon);
  switch (g_stat->pen_type) {
    case PEN_DRAW:    LibGdsReverse( 0, 150, 18, 159); break;
    case PEN_CORRECT: LibGdsReverse(36, 150, 54, 159); break;
    default:          LibGdsReverse(18, 150, 36, 159);
  }
  menu_draw_stat();
}

void pad_up(byte scroll_step) {
  byte empty = 1;
  byte far *start_line;
  byte far *line;
  word pos;

  if (g_stat->line > scroll_step) g_stat->line -= scroll_step;
  else g_stat->line = 0;
  pad_get_scr();

  pos = pad_get_line( g_stat->line + 150, &line);
  start_line = line;
  while(*line != BUFFER_END) {
    if (*line != 0xE1) {
      empty = 0;
      break;
    }
    pad_get_next_line(&line);
  }
  if (empty && *start_line != BUFFER_END) {
    *start_line = BUFFER_END;
    g_stat->lines = g_stat->line + 150;
    g_stat->buf_size = pos;
  }

  pad_draw(1);
  menu_draw_stat();
  LibPutDisp();
}

void pad_down(byte scroll_step) {
  g_stat->line += scroll_step;
  pad_get_scr();
  pad_draw(1);
  menu_draw_stat();
  LibPutDisp();
}

void pad_point_alg(int x, int y, int x0, int y0, int revx, int revy, int swap) {
  int x1,y1,x2,y2;
  word draw_byte;
  byte draw_bit;

  byte size = (g_stat->pen_type == PEN_DRAW) ? g_stat->pen_size :
					    g_stat->eraser_size;
  byte draw = (g_stat->pen_type == PEN_DRAW) ? 1 : 0;
  byte mesh = g_stat->pen_mesh;
  byte hsize = size == 2 ? 2 : size;
  byte pen;
  int mode;

  if (swap==1) {
    y1=x; x1=y;
  } else {
    x1=x; y1=y;
  }
  if (revx==1) x1 = x0 - x1;
  else x1 = x0 + x1;
  if (revy==1) y1 = y0 - y1;
  else y1 = y0 + y1;

  x2 = x1-hsize;
  y2 = y1-hsize;

  if (draw) {
    mode = IB_GPOR;
    if (mesh) {
      if ((x2 + y2 + g_stat->line) % 2 == 0)
	pen = size + 3;
      else
	pen = size + 6;
    }
    else {
      pen = size;
    }
  } else {
    mode = IB_GPAND;
    pen = size;
  }

  LibPutGraphO(x2, y2, g_pen[pen], mode);
}

void pad_line_alg(int x, int y, int x0, int y0, int revx, int revy, int swap) {
  int i;
  float k;

  if (x==0) {
    pad_point_alg(0,0,x0,y0,revx,revy,swap);
    return;
  }

  k=(float)y/(float)x;
  for(i=0; i<=x; i++) {
    byte n;
    int j=k*(float)i;
    pad_point_alg(i,j,x0,y0,revx,revy,swap);
  }
}

void pad_line(int x1, int y1, int x2, int y2) {
  int swap, x0, y0, revx, revy;
  int dx = x2 - x1;
  int dy = y2 - y1;
  int adx = abs(dx);
  int ady = abs(dy);
  if (ady>adx) {
    swap = 1;
    if (dy<0) { 
      revy = 0;
      x0 = x2;
      y0 = y2;
      revx = (dx<0) ? 0 : 1;
    }
    else {
      revy = 0;
      x0 = x1;
      y0 = y1;
      revx = (dx<0) ? 1 : 0;
    }
    pad_line_alg(ady,adx, x0, y0, revx, revy, swap);
  }
  else {
    swap = 0;
    if (dx<0) {
      revx = 0;
      x0 = x2;
      y0 = y2;
      revy = (dy<0) ? 0 : 1;
    }
    else {
      revx = 0;
      x0 = x1;
      y0 = y1;
      revy = (dy<0) ? 1 : 0;
    }
    pad_line_alg(adx,ady, x0, y0, revx, revy, swap);
  }
}

char pad_check(void) {
  byte old_x = g_tch.x;
  byte old_y = g_tch.y;
  if (g_tch.obj == OBJ_IC_PAD) {
    pad_save_scr();
    pad_draw(0);
    do {
      if (g_tch.x >= 0 && g_tch.y >=0 && g_tch.x < 160 && g_tch.y < 150) {
	if (g_stat->pen_type == PEN_CORRECT) {
	  if (g_tch.y > old_y)
	    pad_insert_lines(old_y, g_tch.y - old_y);
	  if (g_tch.y < old_y)
	    pad_delete_lines(old_y, old_y - g_tch.y);

	  pad_get_scr();
	} else {
	  pad_line(g_tch.x, g_tch.y, old_x, old_y);
	}
	old_x = g_tch.x;
	old_y = g_tch.y;
	if (g_stat->pen_type == PEN_CORRECT) {
	  pad_draw(1);
	  LibGdsLine(0, g_tch.y, 159, g_tch.y);
	}
	LibPutDisp();
      }
      wait_move();
    } while(g_tch.act != ACT_BREAK_IN && g_tch.act != ACT_MOVE_OUT);

    if (g_stat->pen_type != PEN_CORRECT) {
      LibGetGraph(0, 0, 160, 150, &g_scr[4]);
    }
    pad_draw(1);
    menu_draw_stat();
    LibPutDisp();
  } else if (g_tch.obj == OBJ_IC_MENU) {
    if (g_tch.x < 18) {
      if (g_stat->pen_type == PEN_DRAW) {
	g_stat->pen_size++;
	if (g_stat->pen_size >= 3) g_stat->pen_size = 0;
      }
      g_stat->pen_type = PEN_DRAW;
    } else if (g_tch.x < 36) {
      if (g_stat->pen_type == PEN_ERASE ) {
	g_stat->eraser_size++;
	if (g_stat->eraser_size >= 3) g_stat->eraser_size = 0;
      }
      g_stat->pen_type = PEN_ERASE;
    } else if (g_tch.x < 54) {
      g_stat->pen_type = PEN_CORRECT;
    } else if (g_tch.x < 72) {
      g_stat->pen_mesh ^= 1;
    } else if (g_tch.x < 90) {
      pad_get_scr();
      pad_draw(1);
    }
    menu_draw();
    LibPutDisp();
  } else if (g_tch.obj == OBJ_LPSW_UP) {
    byte i=0;
    byte scroll_step = 1;
    pad_save_scr();
    while (getleverpush()) {
      i++;
      if (i==2 && scroll_step < 80) {
	i=0;
	scroll_step++;
      }
      pad_up(scroll_step);
    }
  }
  else if (g_tch.obj == OBJ_LPSW_DOWN) {
    byte i=0;
    byte scroll_step = 1;
    pad_save_scr();
    while (getleverpush()) {
      i++;
      if (i==2 && scroll_step < 75) {
	i=0;
	scroll_step++;
      }
      pad_down(scroll_step);
    }
  }
  else if (g_tch.obj == OBJ_LPSW_PUSH) {
    g_stat->pen_type = (g_stat->pen_type == PEN_DRAW) ? PEN_ERASE : PEN_DRAW;
    menu_draw();
    LibPutDisp();
  }
  else if (g_tch.obj == OBJ_HIC_ESC) {
    pad_init();
    pad_draw(1);
    menu_draw_stat();
    LibPutDisp();
  }
  return 1;
}

int far mode_hook(unsigned int iRegAX, unsigned int iRegBX)
{
  switch (iRegAX >> 8)
  {
      case IB_MDWINON:
	if (call_shot == 0) {
	  byte far *ptr;
	  call_shot = 1;
	  g_shot.x=0;
	  g_shot.y=0;
	  g_shot.x_size=156;
	  g_shot.y_size=156-29;
	  g_shot.p_x=2;
	  g_shot.p_y=29;
	  pad_draw(0);
	  LibScrShot(&g_shot);

/*	  ptr = LibDualWin(0x0900, 4, &g_shot);
	  LibPutMsgDlg("%x:%x", FP_SEG(ptr), FP_OFF(ptr)); */
	  ptr = MK_FP(0x1240, 81*20+8);

	  far_memmove(&g_scr[4], ptr, (156-29)*20);
	  pad_draw(1);
	  LibPutDisp();
	  call_shot = 0;
	  return 1;
	}
	return 0;
      case IB_OPENSYNCJP: return 0;
      case IB_MJUMP:
	if ((iRegBX & 0xFF00) == IW_MCOM) {
          return 0;
        }
      default:
	pad_save_scr();
	memo_save();
	return 0;
  }

  return 0;
}

void main(void) {

  LibTchStackClr();
  LibTchStackPush(NULL);
  LibTchStackPush(TchHardIcon);
  LibTchStackPush(TchModeIcon);
  LibTchStackPush(TchActionKey);
  LibTchStackPush(TchList);
  
  setmodehook(&mode_hook);

  LibTchInit();

  memo_init();
  pad_init();
  if (memo_load_first()) {
    pad_get_scr();
    new_file = 0;
  } else new_file = 1;
  pad_draw(1);
  menu_draw();
  LibPutDisp();
  call_shot = 0;
  do {
    wait_press();
  } while(pad_check());

  releasemodehook();
  LibJumpMenu();
}
