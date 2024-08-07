/******************************************************************************
*	[Title]		PVBallLines GraphicsData
*	[Model] 		PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#include "define.h"
#include "libc.h"

byte far GraphALine[] = {    /* Puzzle */
	GSIZE(11, 11),
	0xDB, 0x7F,
	0xDB, 0x7F,
	0x00, 0x1F,
	0xDB, 0x7F,
	0xDB, 0x7F,
	0x00, 0x1F,
	0xDB, 0x7F,
	0xDB, 0x7F,
	0x00, 0x1F,
	0xDB, 0x1F,
	0xDB, 0x1F,
};

byte far GraphP01[] = {    /* 1 */
	GSIZE(15, 15),
	0x00, 0x01,
	0x00, 0x01,
	0x01, 0x01,
	0x02, 0x81,
	0x02, 0x81,
	0x04, 0x41,
	0x04, 0x41,
	0x08, 0x21,
	0x08, 0x21,
	0x10, 0x11,
	0x10, 0x11,
	0x3F, 0xF9,
	0x00, 0x01,
	0x00, 0x01,
	0x00, 0x01
};


byte far GraphP02[] = {    /* 2 */
	GSIZE(15, 15),
	0x00, 0x01,
	0x00, 0x01,
	0x03, 0x81,
	0x0C, 0x61,
	0x10, 0x11,
	0x16, 0xD1,
	0x26, 0xC9,
	0x20, 0x09,
	0x20, 0x09,
	0x14, 0x51,
	0x13, 0x91,
	0x0C, 0x61,
	0x03, 0x81,
	0x00, 0x01,
	0x00, 0x01
};

byte far GraphP03[] = {    /* 3 */
	GSIZE(15, 15),
	0x00, 0x01,
	0x00, 0x01,
	0x03, 0x81,
	0x0C, 0x61,
	0x10, 0x11,
	0x10, 0x11,
	0x20, 0x09,
	0x20, 0x09,
	0x20, 0x09,
	0x10, 0x11,
	0x10, 0x11,
	0x0C, 0x61,
	0x03, 0x81,
	0x00, 0x01,
	0x00, 0x01
};

byte far GraphP04[] = {    /* 4 */
	GSIZE(15, 15),
	0x00, 0x01,
	0x00, 0x01,
	0x0F, 0xE1,
	0x10, 0x11,
	0x27, 0x89,
	0x48, 0x49,
	0x48, 0x25,
	0x48, 0x25,
	0x49, 0x25,
	0x26, 0x49,
	0x20, 0x49,
	0x1F, 0x91,
	0x00, 0x21,
	0x0F, 0xC1,
	0x00, 0x01
};

byte far GraphP05[] = {    /* 5 */
	GSIZE(15, 15),
	0x00, 0x01,
	0x00, 0x01,
	0x3F, 0xF9,
	0x20, 0x09,
	0x2F, 0xE9,
	0x28, 0x29,
	0x28, 0x29,
	0x28, 0x29,
	0x28, 0x29,
	0x28, 0x29,
	0x2F, 0xE9,
	0x20, 0x09,
	0x3F, 0xF9,
	0x00, 0x01,
	0x00, 0x01
};

byte far GraphP06[] = {    /* 6 */
	GSIZE(15, 15),
	0x00, 0x01,
	0x00, 0x01,
	0x04, 0x41,
	0x04, 0x41,
	0x04, 0x41,
	0x3F, 0xF9,
	0x04, 0x41,
	0x04, 0x41,
	0x04, 0x41,
	0x3F, 0xF9,
	0x04, 0x41,
	0x04, 0x41,
	0x04, 0x41,
	0x00, 0x01,
	0x00, 0x01
};

byte far GraphP07[] = {    /* 7 */
	GSIZE(15, 15),
	0x00, 0x01,
	0x00, 0x01,
	0x00, 0xF1,
	0x01, 0x21,
	0x02, 0x41,
	0x04, 0x81,
	0x04, 0x81,
	0x09, 0x01,
	0x12, 0x01,
	0x27, 0xF9,
	0x20, 0x09,
	0x20, 0x09,
	0x3F, 0xF9,
	0x00, 0x01,
	0x00, 0x01
};

byte far GraphP08[] = {    /* 8 */
	GSIZE(15, 15),
	0x00, 0x01,
	0x00, 0x01,
	0x00, 0x01,
	0x07, 0xC1,
	0x38, 0x39,
	0x40, 0x05,
	0x80, 0x03,
	0x80, 0x03,
	0x80, 0x03,
	0x40, 0x05,
	0x38, 0x39,
	0x07, 0xC1,
	0x00, 0x01,
	0x00, 0x01,
	0x00, 0x01
};

byte far *Graphs[]={
	GraphP01, GraphP02, GraphP03,GraphP04,
	GraphP05, GraphP06, GraphP07,GraphP08
};
