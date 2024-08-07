/******************************************************************************
*   Program Title	Domino
*   Module			Main game
*	Author  		J. Marcos Matilla (marcosjo@wanadoo.es)
*	Date    		01/2001 
*	Rev     		1.2
*	MMSoft 2001 - Freeware
*	Notes:	
*
******************************************************************************/

#include	<domino.h>
#include	<define.h>
#include	<libc.h>


T_FICHA Domino[28];
T_SIDE Board[2];
T_PLAYER Player[N_PLAYERS];


bool newgame;
bool resume;
int levelgame;


byte far *Image[8]={ImgPlayer0,
					ImgPlayer1,
					ImgPlayer2,
					ImgPlayer3,
					ImgPlayer4,
					ImgPlayer5,
					ImgPlayer6,
					ImgPlayer7};

byte far ImgPlayer0[]={  /* Player image Man 1*/
	GSIZE(40, 40),
	0x00, 0x07, 0x80, 0x00, 0x00,
	0x00, 0x7C, 0x7C, 0x00, 0x00,
	0x01, 0xC0, 0x07, 0x00, 0x00,
	0x07, 0x00, 0x00, 0xC0, 0x00,
	0x04, 0x00, 0x00, 0x60, 0x00,
	0x0C, 0x00, 0x00, 0x20, 0x00,
	0x08, 0x07, 0x00, 0x18, 0x00,
	0x30, 0x05, 0xC3, 0x08, 0x00,
	0x74, 0x0F, 0x23, 0x9C, 0x00,
	0x7C, 0x05, 0xC1, 0x9C, 0x00,
	0x7F, 0x8F, 0xE1, 0xFC, 0x00,
	0x7F, 0xC5, 0xF1, 0xFC, 0x00,
	0x7F, 0xC3, 0xFF, 0xFC, 0x00,
	0x7F, 0xC7, 0xFF, 0xEC, 0x00,
	0x7F, 0xC7, 0xFF, 0xFF, 0x80,
	0x3F, 0xE4, 0x1F, 0x06, 0x40,
	0x7F, 0xE0, 0x3E, 0x01, 0x40,
	0x7F, 0xC0, 0x7C, 0x01, 0x20,
	0x7F, 0xE0, 0xC1, 0xC0, 0xB0,
	0x3C, 0xE0, 0x02, 0x00, 0x90,
	0x18, 0x60, 0x03, 0x00, 0x90,
	0x19, 0xC0, 0x07, 0x1D, 0x90,
	0x0D, 0x40, 0x04, 0xFF, 0x70,
	0x0D, 0x60, 0x04, 0x00, 0xE0,
	0x0D, 0x60, 0x04, 0x3F, 0x60,
	0x0F, 0x60, 0x04, 0x60, 0x40,
	0x0E, 0xE0, 0x04, 0x47, 0xE0,
	0x0A, 0x48, 0x04, 0xCF, 0xE0,
	0x09, 0x0C, 0x34, 0x9F, 0xE0,
	0x0C, 0xC4, 0x35, 0x3C, 0x20,
	0x04, 0x7C, 0x34, 0x20, 0x30,
	0x07, 0x00, 0x64, 0x00, 0x10,
	0x01, 0xC0, 0x64, 0x00, 0x30,
	0x00, 0x60, 0x70, 0x00, 0x20,
	0x00, 0x38, 0x7C, 0x09, 0xE0,
	0x00, 0x0C, 0x3F, 0x0F, 0x80,
	0x00, 0x04, 0x1C, 0x05, 0x00,
	0x00, 0x04, 0x00, 0x02, 0x00,
	0x00, 0x03, 0x00, 0x04, 0x00,
	0x00, 0x01, 0xE0, 0x78, 0x00
	};

byte far ImgPlayer1[]={  /* Player image Man 2 */
	GSIZE(40, 40),
	0x00, 0x07, 0xFF, 0x80, 0x00,
	0x00, 0x3F, 0xFF, 0xC0, 0x00,
	0x00, 0xFF, 0xFF, 0xFC, 0x00,
	0x03, 0xFF, 0xFF, 0xFE, 0x00,
	0x03, 0xFF, 0xFF, 0xFF, 0x00,
	0x07, 0xFF, 0xFF, 0xFF, 0x80,
	0x07, 0xFF, 0xFF, 0xFF, 0x80,
	0x0F, 0xFF, 0xFF, 0xFF, 0xC0,
	0x1F, 0xFF, 0xFF, 0xFF, 0xC0,
	0x1F, 0xFF, 0xFF, 0xFF, 0xE0,
	0x7F, 0xFF, 0xFF, 0xFF, 0xE0,
	0x7F, 0xFF, 0xFF, 0xFF, 0xE0,
	0x7F, 0xFF, 0xFF, 0xFF, 0xE0,
	0x3F, 0x7F, 0xFF, 0xFF, 0xE0,
	0x3E, 0x7F, 0xFF, 0xFF, 0xE0,
	0x1E, 0x3F, 0x9F, 0xFF, 0xE0,
	0x1E, 0x3F, 0x03, 0xCF, 0xC0,
	0x0E, 0x3C, 0x00, 0x0F, 0xC0,
	0x06, 0x3C, 0x01, 0x0F, 0xC0,
	0x16, 0x00, 0x0F, 0xC7, 0xE0,
	0x16, 0x3C, 0x3F, 0xF7, 0x60,
	0x16, 0xFF, 0x04, 0xC7, 0x60,
	0x0E, 0x6F, 0x07, 0x03, 0xE0,
	0x0E, 0x01, 0x00, 0x03, 0xA0,
	0x0E, 0x01, 0x00, 0x03, 0xE0,
	0x0C, 0x03, 0x00, 0x02, 0xE0,
	0x0C, 0x03, 0x00, 0x03, 0xE0,
	0x04, 0x04, 0x00, 0x01, 0x80,
	0x06, 0x04, 0x02, 0x02, 0x00,
	0x02, 0x06, 0x7B, 0x02, 0x00,
	0x03, 0x13, 0x61, 0x82, 0x00,
	0x01, 0x20, 0x00, 0x86, 0x00,
	0x01, 0xA0, 0xF8, 0x86, 0x00,
	0x00, 0x8F, 0xF8, 0x06, 0x00,
	0x00, 0xC3, 0x38, 0x06, 0x00,
	0x00, 0x40, 0x00, 0x0C, 0x00,
	0x00, 0x71, 0xE0, 0x1C, 0x00,
	0x00, 0x31, 0xC0, 0x38, 0x00,
	0x00, 0x18, 0x00, 0x70, 0x00,
	0x00, 0x0F, 0xFF, 0xC0, 0x00
	};

byte far ImgPlayer2[]={  /* Player image Man 3*/
	GSIZE(40, 40),
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xC0, 0x00,
	0x00, 0x00, 0x01, 0xC0, 0x40,
	0x00, 0x00, 0x02, 0xC3, 0xC0,
	0x00, 0x00, 0x36, 0x5D, 0x80,
	0x00, 0x00, 0x7C, 0xEA, 0x00,
	0x00, 0x00, 0x7A, 0x24, 0x00,
	0x00, 0x0E, 0x48, 0x8F, 0x80,
	0x00, 0x07, 0xE2, 0x27, 0x00,
	0x00, 0x0E, 0xC8, 0x8C, 0x00,
	0x00, 0x1E, 0x22, 0x2C, 0x00,
	0x00, 0x0E, 0xB8, 0xF0, 0x00,
	0x00, 0x07, 0xEF, 0x30, 0x00,
	0x00, 0x0D, 0x86, 0x08, 0x00,
	0x00, 0x06, 0xE1, 0x84, 0x00,
	0x00, 0x0D, 0x32, 0x46, 0x00,
	0x00, 0x1B, 0xF3, 0xE2, 0x00,
	0x00, 0x31, 0x00, 0x03, 0x00,
	0x3F, 0xE0, 0x71, 0xC3, 0xF0,
	0x60, 0x60, 0xB2, 0x23, 0x0C,
	0xC7, 0xE3, 0x73, 0x23, 0xF2,
	0x40, 0x61, 0x73, 0xE3, 0x02,
	0x20, 0x71, 0xE3, 0xC3, 0x06,
	0x18, 0xC2, 0x80, 0xB3, 0x0C,
	0x0F, 0x00, 0xB3, 0x81, 0xF0,
	0x06, 0x00, 0x0C, 0x40, 0x60,
	0x04, 0x02, 0x00, 0x20, 0x20,
	0x04, 0x04, 0x00, 0x20, 0x20,
	0x04, 0x04, 0x80, 0xB0, 0x20,
	0x04, 0x0C, 0x7E, 0x30, 0x20,
	0x06, 0x08, 0x00, 0x30, 0xC0,
	0x03, 0x08, 0x3E, 0x30, 0x80,
	0x03, 0x0C, 0x00, 0x21, 0x80,
	0x03, 0x00, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x00, 0x01, 0x00,
	0x00, 0x80, 0x6E, 0x03, 0x00,
	0x00, 0x60, 0x10, 0x06, 0x00,
	0x00, 0x38, 0x00, 0x0C, 0x00,
	0x00, 0x07, 0xFF, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00
    };

byte far ImgPlayer3[]={  /* Player image Man 4*/
	GSIZE(40, 40),
	0x00, 0x07, 0xFF, 0xE0, 0x00,
	0x00, 0x1F, 0xFF, 0xF8, 0x00,
	0x00, 0xFF, 0xFF, 0xFE, 0x00,
	0x01, 0xFF, 0xFF, 0xFF, 0x80,
	0x07, 0xFF, 0xFF, 0xFF, 0xC0,
	0x0F, 0xFF, 0xFF, 0xFF, 0xE0,
	0x1F, 0xFF, 0xFF, 0xFF, 0xF0,
	0x1F, 0xFF, 0xFF, 0xFF, 0xF8,
	0x3F, 0xFF, 0xFF, 0xFF, 0xFC,
	0x7F, 0xFF, 0xFF, 0xFF, 0xFE,
	0x7F, 0xF7, 0xF8, 0x07, 0xFE,
	0x7F, 0xC0, 0x00, 0x01, 0xFE,
	0x7F, 0x80, 0x00, 0x00, 0x7F,
	0xFF, 0x07, 0xE0, 0xFC, 0x3F,
	0xFE, 0x1F, 0xF1, 0xFE, 0x3F,
	0xFE, 0x30, 0x00, 0x02, 0x3F,
	0xFE, 0x07, 0x84, 0xF8, 0x33,
	0xE6, 0x01, 0x92, 0x60, 0x3B,
	0xC6, 0x00, 0x12, 0x00, 0x79,
	0x86, 0x00, 0x25, 0x00, 0x73,
	0xB6, 0x00, 0xC3, 0x80, 0x3A,
	0x94, 0x01, 0x00, 0x40, 0x32,
	0x44, 0x01, 0x80, 0x00, 0x34,
	0x24, 0x00, 0xFF, 0xC0, 0x38,
	0x24, 0x03, 0xFF, 0xE0, 0x20,
	0x1C, 0x03, 0xFF, 0xF0, 0x20,
	0x02, 0x07, 0xC1, 0xF8, 0x40,
	0x03, 0x07, 0x00, 0x78, 0x40,
	0x01, 0x00, 0x3E, 0x00, 0x80,
	0x00, 0x80, 0x00, 0x01, 0x00,
	0x00, 0x60, 0x00, 0x02, 0x00,
	0x00, 0x30, 0x00, 0x02, 0x00,
	0x00, 0x18, 0x00, 0x04, 0x00,
	0x00, 0x14, 0x00, 0x0C, 0x00,
	0x00, 0x13, 0x1F, 0x94, 0x00,
	0x00, 0x78, 0xC0, 0x42, 0x00,
	0x01, 0xC6, 0x00, 0x13, 0x00,
	0x03, 0xE1, 0x80, 0x23, 0xC0,
	0x1F, 0xE0, 0x60, 0x47, 0xE0,
	0xFF, 0xD0, 0x10, 0x87, 0xF8
	};

byte far ImgPlayer4[]={  /* Player image Woman 1 */
	GSIZE(40, 40),
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7C, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0x80, 0x00,
	0x00, 0x01, 0xFF, 0xC0, 0x00,
	0x00, 0x0F, 0xFF, 0xE0, 0x00,
	0x00, 0x7F, 0xFF, 0xFC, 0x00,
	0x00, 0x7F, 0xFF, 0xFF, 0x80,
	0x00, 0xFF, 0xFF, 0xFF, 0xC0,
	0x01, 0xFF, 0xFF, 0xFF, 0xC0,
	0x01, 0xFF, 0xFF, 0xFF, 0xE0,
	0x03, 0xFF, 0xFF, 0xFF, 0xF8,
	0x03, 0xFF, 0xFF, 0xFF, 0xF8,
	0x03, 0xF7, 0xFF, 0xFF, 0xFC,
	0x07, 0xF7, 0xFF, 0xFF, 0xFE,
	0x07, 0xC1, 0xFF, 0xFF, 0xFE,
	0x07, 0xC0, 0xFF, 0xFF, 0xFE,
	0x0F, 0xC0, 0x7F, 0xFF, 0xFC,
	0x0F, 0xEE, 0x3F, 0xFF, 0xFE,
	0x0F, 0x9C, 0x01, 0xFF, 0xFE,
	0x1F, 0xDC, 0x06, 0x7F, 0xFE,
	0x1F, 0x98, 0x07, 0x7F, 0xFC,
	0x77, 0x00, 0x04, 0x0F, 0xFC,
	0x5C, 0x02, 0x10, 0x05, 0xF0,
	0x5C, 0x01, 0xE0, 0x06, 0xC0,
	0x64, 0x40, 0x00, 0x86, 0x80,
	0xED, 0xFF, 0xFF, 0xC6, 0x80,
	0x65, 0x7F, 0xF0, 0xC6, 0x80,
	0x74, 0x3C, 0x07, 0x8E, 0x80,
	0x5C, 0x1F, 0xFF, 0x1E, 0x80,
	0x7F, 0xCF, 0xFE, 0x75, 0x80,
	0x3C, 0x47, 0xF8, 0xFF, 0x00,
	0x00, 0x31, 0xE3, 0x8C, 0x00,
	0x00, 0x08, 0x07, 0x00, 0x00,
	0x00, 0x06, 0x04, 0x00, 0x00,
	0x00, 0x01, 0x0C, 0x00, 0x00,
	0x00, 0x00, 0xF8, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00
	};

byte far ImgPlayer5[]={  /* Player image Woman 2 */
	GSIZE(40, 40),
	0x00, 0x00, 0x70, 0x38, 0x00,
	0x00, 0x0F, 0xFF, 0xEF, 0x80,
	0x00, 0x1C, 0x03, 0x80, 0x40,
	0x00, 0x7E, 0x07, 0xC1, 0x80,
	0x00, 0xBF, 0xFF, 0xFF, 0x80,
	0x00, 0xFF, 0xFF, 0xFF, 0x80,
	0x01, 0xFF, 0xFF, 0xFF, 0xC0,
	0x01, 0xFF, 0xFF, 0xFF, 0xF0,
	0x03, 0xFF, 0xFF, 0xFF, 0xF8,
	0x03, 0xFC, 0x7F, 0xFF, 0xFC,
	0x03, 0xE0, 0x0F, 0xFF, 0xFE,
	0x03, 0xC7, 0x81, 0x7F, 0xFE,
	0x07, 0xC0, 0x00, 0x17, 0xFF,
	0x07, 0xC7, 0x41, 0xE7, 0xFF,
	0x07, 0x81, 0x00, 0x43, 0xFF,
	0x07, 0xC0, 0x00, 0x03, 0xFF,
	0x0F, 0xC0, 0x00, 0x03, 0xFF,
	0x0F, 0xC0, 0x20, 0x03, 0xFF,
	0x3F, 0xC1, 0x02, 0x03, 0xFE,
	0xFF, 0xE2, 0x70, 0x07, 0xFE,
	0xFF, 0xE2, 0x00, 0x07, 0xFE,
	0x7F, 0xF0, 0x3E, 0x0F, 0xFF,
	0x1F, 0xF8, 0x00, 0x1F, 0xFF,
	0x00, 0xFC, 0x00, 0x7F, 0xF8,
	0x00, 0x1C, 0x00, 0xBF, 0xC0,
	0x00, 0x0B, 0x01, 0x18, 0x00,
	0xCC, 0x10, 0xFA, 0x0E, 0x00,
	0xFF, 0xE0, 0x00, 0x05, 0xC0,
	0xF3, 0xC0, 0x00, 0x02, 0xFC,
	0xFB, 0xA0, 0x00, 0x1C, 0xEA,
	0x39, 0xB0, 0x00, 0x62, 0xE3,
	0x9D, 0xD0, 0x00, 0x48, 0xE8,
	0x2D, 0xA8, 0x00, 0x63, 0xE2,
	0x8F, 0x8C, 0x00, 0x89, 0xE8,
	0x27, 0xA3, 0x00, 0xA3, 0xE2,
	0x8F, 0x88, 0x81, 0x8B, 0xC8,
	0x27, 0xE2, 0x41, 0x23, 0xA2,
	0x8F, 0xA8, 0xA2, 0x8F, 0x88,
	0x27, 0xF2, 0x32, 0x27, 0xA2,
	0xB8, 0xF8, 0x9C, 0x8F, 0x88
	};

byte far ImgPlayer6[]={  /* Player image Woman 3 */
	GSIZE(40, 40),
	0x00, 0x01, 0xE0, 0x00, 0x00,
	0x00, 0x0E, 0xBC, 0x00, 0x00,
	0x00, 0x12, 0x2B, 0x00, 0x00,
	0x00, 0x28, 0xB8, 0xC0, 0x00,
	0x00, 0xE2, 0xFB, 0x20, 0x00,
	0x01, 0x88, 0x88, 0xF0, 0x00,
	0x02, 0x22, 0x22, 0x78, 0x00,
	0x04, 0x88, 0x88, 0xCC, 0x00,
	0x06, 0x22, 0x22, 0xE6, 0x00,
	0x08, 0x88, 0xB8, 0x89, 0x00,
	0x0A, 0xA2, 0xFF, 0xF2, 0xC0,
	0x09, 0x5F, 0x00, 0x79, 0xC0,
	0x0A, 0xA0, 0xF0, 0x0E, 0x40,
	0x0C, 0xA0, 0x00, 0x08, 0xA0,
	0x0A, 0xC2, 0x1E, 0x0F, 0x20,
	0x18, 0xC0, 0x40, 0x4A, 0xA0,
	0x23, 0x40, 0x00, 0x0E, 0x20,
	0x29, 0xC0, 0x00, 0x0C, 0x90,
	0x22, 0x45, 0x01, 0x6E, 0xB0,
	0x48, 0xC8, 0x02, 0x05, 0xF0,
	0x62, 0x73, 0xC0, 0xF7, 0x20,
	0x5F, 0xDE, 0x63, 0x9E, 0xA0,
	0x66, 0xC5, 0xBF, 0x6F, 0x60,
	0x28, 0xC4, 0x25, 0x0A, 0xA0,
	0x22, 0x46, 0x65, 0x9B, 0x60,
	0x39, 0xC3, 0xC4, 0xF2, 0xA0,
	0x0A, 0x40, 0x04, 0x03, 0x60,
	0x06, 0xC0, 0x04, 0x03, 0x80,
	0x06, 0xC0, 0x44, 0x01, 0x80,
	0x07, 0x60, 0x85, 0x02, 0x80,
	0x03, 0xC1, 0x29, 0x43, 0x00,
	0x03, 0xC8, 0x00, 0x0F, 0x00,
	0x01, 0xC0, 0x3C, 0x26, 0x00,
	0x00, 0xE5, 0xC3, 0x8C, 0x00,
	0x00, 0x24, 0x7F, 0x28, 0x00,
	0x00, 0x30, 0x0C, 0x10, 0x00,
	0x00, 0x20, 0x00, 0x38, 0x00,
	0x01, 0xA8, 0x00, 0xB0, 0x00,
	0x03, 0x68, 0x00, 0xF8, 0x00,
	0x0E, 0xA6, 0x00, 0x8E, 0x00
    };

byte far ImgPlayer7[]={  /* Player image Woman 4 */
	GSIZE(40, 40),
	0x00, 0x00, 0x7C, 0x00, 0x00,
	0x00, 0x01, 0xA3, 0xF0, 0x00,
	0x00, 0x02, 0x00, 0x18, 0x00,
	0x00, 0x0E, 0x8A, 0x88, 0x00,
	0x00, 0x70, 0x00, 0x0E, 0x00,
	0x00, 0xA2, 0x22, 0x23, 0x00,
	0x01, 0x00, 0x00, 0x19, 0x00,
	0x01, 0xAA, 0xAA, 0xBA, 0x80,
	0x01, 0x80, 0x00, 0x28, 0x40,
	0x03, 0xA2, 0x22, 0x0E, 0x40,
	0x0D, 0x00, 0x00, 0x8C, 0x20,
	0x1A, 0x8A, 0x8B, 0x0E, 0x90,
	0x10, 0x01, 0x6A, 0x02, 0x30,
	0x1A, 0x23, 0x80, 0x07, 0x60,
	0x38, 0x05, 0x00, 0x04, 0x40,
	0x2A, 0xA8, 0x00, 0x66, 0xD0,
	0x28, 0x57, 0xC3, 0x9C, 0x28,
	0x62, 0x67, 0x42, 0xE6, 0x24,
	0xC1, 0x6F, 0x91, 0xD4, 0x04,
	0x8B, 0xE6, 0x90, 0xE2, 0x88,
	0xC2, 0x20, 0x10, 0x04, 0x08,
	0xF2, 0x60, 0x00, 0x06, 0x28,
	0xC0, 0x60, 0x54, 0x04, 0x0C,
	0xEB, 0xE0, 0x40, 0x02, 0xA8,
	0xEF, 0x30, 0x00, 0x0C, 0x18,
	0x97, 0xB0, 0x7F, 0x8A, 0x3C,
	0x7F, 0xF9, 0x61, 0x08, 0x74,
	0x3F, 0x6C, 0x36, 0x16, 0xD8,
	0x00, 0xFE, 0x00, 0x3F, 0xE0,
	0x00, 0x0F, 0x00, 0x50, 0x00,
	0x00, 0x0F, 0x80, 0xA0, 0x00,
	0x00, 0x0F, 0xF3, 0x20, 0x00,
	0x00, 0x0F, 0xF8, 0x00, 0x00,
	0x00, 0x7F, 0xF8, 0x60, 0x00,
	0x03, 0x9F, 0xF0, 0x7C, 0x00,
	0x0E, 0x0F, 0xE0, 0x47, 0x00,
	0x30, 0x0F, 0xE0, 0xC0, 0xE0,
	0xC0, 0x07, 0xE0, 0xC0, 0x18,
	0x00, 0x07, 0xF9, 0x80, 0x04,
	0x00, 0x00, 0x00, 0x00, 0x00
	};
byte far ShapeL[]={  /* Shape left image */
	GSIZE(10, 15),
	0x3F, 0xFF,
	0x60, 0x3F,
	0xC0, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0x80, 0x3F,
	0xC0, 0x3F,
	0x60, 0x3F,
	0x3E, 0x3F,
	0x02, 0x7F,
	0x02, 0xBF,
	0x05, 0x3F,
	0x06, 0x3F
};

byte far ShapeR[]={  /* Shape right image */
	GSIZE(4, 11),
	0xCF,
	0x6F,
	0x3F,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x3F,
	0x6F,
	0xCF
};

byte far Copa[]={  /* Coup */
	GSIZE(33, 50),
	0x00, 0x0F, 0xF0, 0x00, 0x7F,
	0x00, 0x7F, 0xFE, 0x00, 0x7F,
	0x01, 0xFF, 0xFF, 0x80, 0x7F,
	0x03, 0xFF, 0xFF, 0xC0, 0x7F,
	0x03, 0xFF, 0xFF, 0xE0, 0x7F,
	0x03, 0xFF, 0xFF, 0xA0, 0x7F,
	0x02, 0x7F, 0xFE, 0x60, 0x7F,
	0xFF, 0x1F, 0xF1, 0x7F, 0xFF,
	0xFF, 0x44, 0x44, 0x7F, 0xFF,
	0x61, 0x11, 0x11, 0x43, 0x7F,
	0x71, 0x44, 0x44, 0x47, 0x7F,
	0x31, 0x11, 0x11, 0x46, 0x7F,
	0x38, 0xC4, 0x44, 0x8E, 0x7F,
	0x18, 0x91, 0x11, 0x8C, 0x7F,
	0x18, 0xC4, 0x44, 0x9C, 0x7F,
	0x0C, 0x91, 0x11, 0x98, 0x7F,
	0x0C, 0x44, 0x45, 0x38, 0x7F,
	0x06, 0x51, 0x11, 0x30, 0x7F,
	0x07, 0x44, 0x45, 0x70, 0x7F,
	0x03, 0xD1, 0x11, 0xE0, 0x7F,
	0x03, 0xC4, 0x45, 0xC0, 0x7F,
	0x00, 0xF1, 0x13, 0x80, 0x7F,
	0x00, 0x24, 0x46, 0x00, 0x7F,
	0x00, 0x31, 0x16, 0x00, 0x7F,
	0x00, 0x1C, 0x58, 0x00, 0x7F,
	0x00, 0x07, 0xF0, 0x00, 0x7F,
	0x00, 0x01, 0x40, 0x00, 0x7F,
	0x00, 0x01, 0x40, 0x00, 0x7F,
	0x00, 0x01, 0x40, 0x00, 0x7F,
	0x00, 0x01, 0x40, 0x00, 0x7F,
	0x00, 0x01, 0x40, 0x00, 0x7F,
	0x00, 0x01, 0x40, 0x00, 0x7F,
	0x00, 0x01, 0x40, 0x00, 0x7F,
	0x00, 0x07, 0xF0, 0x00, 0x7F,
	0x00, 0x3C, 0x1E, 0x00, 0x7F,
	0x00, 0x47, 0xF1, 0x00, 0x7F,
	0x00, 0x80, 0x00, 0x80, 0x7F,
	0x01, 0xFF, 0xFF, 0xC0, 0x7F,
	0x01, 0xFF, 0xFF, 0xC0, 0x7F,
	0x01, 0xFF, 0xFF, 0xE0, 0x7F,
	0x03, 0x80, 0x00, 0xE0, 0x7F,
	0x03, 0x80, 0x80, 0xE0, 0x7F,
	0x03, 0x81, 0x80, 0xF0, 0x7F,
	0x03, 0x80, 0x80, 0xF0, 0x7F,
	0x07, 0x80, 0x80, 0xF0, 0x7F,
	0x07, 0x80, 0x80, 0xF8, 0x7F,
	0x07, 0x80, 0x00, 0xF8, 0x7F,
	0x07, 0xFF, 0xFF, 0xF8, 0x7F,
	0x0F, 0xFF, 0xFF, 0xFC, 0x7F,
	0x0F, 0xFF, 0xFF, 0xFC, 0x7F
};	

byte far Logo[]={  /* Logo*/
	GSIZE(128, 61),
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE1, 0xE9, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1F, 0xE0, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x78, 0xBE, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xEF, 0x8B, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x27, 0xE2, 0x2E, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x3C, 0x8B, 0xC8, 0x89, 0xF0, 0x78, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x0C, 0xE2, 0x22, 0x22, 0x3F, 0x2F, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC8, 0x88, 0x88, 0xFE, 0x89, 0x98, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x3E, 0x23, 0xE2, 0x7E, 0x27, 0x90, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x60, 0x78, 0xBC, 0x8F, 0xF8, 0x88, 0x8F, 0x90, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0x83, 0xE2, 0x7E, 0x23, 0xE2, 0x22, 0x3F, 0x90, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1E, 0x1F, 0xF8, 0x88, 0x88, 0x88, 0x88, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x7C, 0x32, 0x7F, 0x22, 0x22, 0x22, 0x23, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x08, 0xC0, 0xC8, 0x8F, 0xC8, 0x88, 0xE8, 0x8F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x30, 0x07, 0x27, 0xC3, 0xFA, 0x23, 0xFA, 0x2F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0xE0, 0x38, 0x8F, 0xE8, 0xFF, 0x89, 0xF8, 0xBF, 0xFE, 0x00, 0x00, 0x07, 0xF0, 0x00,
	0x00, 0x03, 0x80, 0xE2, 0x27, 0xE2, 0x2F, 0xE2, 0xE2, 0x7F, 0xF8, 0x7F, 0xFC, 0x00, 0x70, 0x00,
	0x00, 0x1F, 0x07, 0x88, 0x8B, 0xC8, 0x89, 0xFC, 0x89, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x18, 0x00,
	0x00, 0x78, 0x1E, 0x22, 0x22, 0x22, 0x22, 0x3F, 0x23, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x08, 0x00,
	0x00, 0xC0, 0x68, 0x88, 0x88, 0x88, 0x89, 0x8F, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x84, 0x00,
	0x00, 0x01, 0xA2, 0x22, 0x22, 0x22, 0x27, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
	0x00, 0x0E, 0x8B, 0x88, 0x88, 0x88, 0x8F, 0xE8, 0xBF, 0xFF, 0x98, 0x88, 0x88, 0x88, 0xC0, 0x00,
	0x04, 0x3A, 0x2F, 0xE2, 0x22, 0x22, 0x2F, 0xE2, 0x3F, 0xFE, 0x2A, 0x7A, 0x22, 0x22, 0x60, 0x00,
	0x71, 0xE8, 0xBF, 0xE8, 0x88, 0x88, 0x8F, 0x88, 0xFF, 0xFC, 0x8C, 0xFC, 0x88, 0x88, 0xB0, 0x00,
	0x47, 0xA2, 0x3F, 0xE2, 0x22, 0x22, 0x22, 0x23, 0xFF, 0xF2, 0x26, 0x3E, 0x0C, 0x22, 0x30, 0x80,
	0x1E, 0x88, 0x9F, 0xC8, 0x88, 0x88, 0x88, 0x8B, 0xFF, 0xE8, 0x8E, 0x98, 0xBE, 0x88, 0x98, 0x80,
	0x1E, 0x22, 0x3F, 0x22, 0x22, 0x22, 0x22, 0x27, 0xFF, 0xE2, 0x23, 0x22, 0x1F, 0x22, 0x28, 0x40,
	0x1F, 0xC8, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8F, 0xFF, 0x8C, 0x89, 0x88, 0x8F, 0x88, 0x8C, 0x60,
	0x0F, 0xF2, 0x22, 0x22, 0x22, 0x22, 0x22, 0x3F, 0xFE, 0x3F, 0x23, 0xA2, 0x22, 0x26, 0x26, 0x20,
	0x4F, 0xFC, 0x88, 0x88, 0x88, 0x88, 0x88, 0xFF, 0xFC, 0xDF, 0x88, 0xC8, 0x88, 0x8F, 0x8E, 0x20,
	0x4F, 0xFF, 0x22, 0x22, 0x22, 0xFA, 0x22, 0xFF, 0xF2, 0x1F, 0x22, 0x62, 0x22, 0x27, 0xE3, 0x30,
	0x4F, 0xFF, 0xE8, 0x88, 0x89, 0xFC, 0x89, 0xFF, 0xF8, 0x88, 0x88, 0xE8, 0x88, 0x89, 0xCB, 0x10,
	0x4F, 0xFF, 0xFA, 0x22, 0x23, 0xFE, 0x23, 0xFF, 0xE2, 0x22, 0x22, 0x32, 0x22, 0x22, 0x23, 0x08,
	0xCF, 0xFF, 0xFE, 0x88, 0x8B, 0xFC, 0x8B, 0xFF, 0x88, 0x88, 0x88, 0xB8, 0x88, 0x88, 0x89, 0x88,
	0x4F, 0xFF, 0xFF, 0xA2, 0x23, 0xFE, 0x27, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80,
	0xE1, 0xFF, 0xFF, 0xE8, 0x89, 0xF8, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80,
	0xE0, 0x7F, 0xFF, 0xFA, 0x22, 0x22, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x84,
	0x7C, 0x1F, 0xFF, 0xFE, 0x88, 0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x86,
	0x7E, 0x07, 0xFF, 0xFF, 0xA2, 0x22, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x82,
	0x07, 0x01, 0xFF, 0xFF, 0xE8, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x82,
	0x01, 0x80, 0x7F, 0xFF, 0xFA, 0x23, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x82,
	0x00, 0xE0, 0x1F, 0xFF, 0xFE, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x90,
	0x00, 0x21, 0x87, 0xFF, 0xFF, 0x2F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
	0x00, 0x18, 0xE1, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x3F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x10,
	0x00, 0x00, 0x38, 0x7F, 0xFF, 0xFF, 0xF1, 0xFF, 0x00, 0x1F, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x30,
	0x00, 0x00, 0x1E, 0x1F, 0xFF, 0xFF, 0xE3, 0xFF, 0xF0, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x02, 0xF0,
	0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xE0,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x80, 0x3F, 0xFF, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x0F, 0xFE, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3E, 0x07, 0xFC, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0xC1, 0xF8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0xF0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};