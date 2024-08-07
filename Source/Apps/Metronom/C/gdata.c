/******************************************************************************
*	[Title]		Pocket Exchange Graphic Data File
*	[Model] 		PV Unit
*	[Version]	Ver1.00
******************************************************************************/
#include "define.h"
#include "libc.h"

byte far GraphTime[] = {    /* Time */
	GSIZE(27, 18),
	0x0C, 0x00, 0x00, 0x1F,
	0x0E, 0x00, 0x00, 0x1F,
	0x0B, 0x00, 0x00, 0x1F,
	0x09, 0x80, 0x00, 0x1F,
	0x08, 0x80, 0x00, 0x1F,
	0x08, 0xC0, 0x00, 0x1F,
	0x08, 0x4F, 0xFF, 0xFF,
	0x08, 0x40, 0x00, 0x1F,
	0x08, 0x40, 0x00, 0x1F,
	0x08, 0x40, 0x00, 0x1F,
	0x08, 0x8F, 0xFF, 0xFF,
	0x09, 0x00, 0x00, 0x1F,
	0x08, 0x00, 0x00, 0x1F,
	0x08, 0x00, 0x00, 0x1F,
	0x38, 0x00, 0x00, 0x1F,
	0x78, 0x00, 0x00, 0x1F,
	0xF0, 0x00, 0x00, 0x1F,
	0xE0, 0x00, 0x00, 0x1F
};

byte far GraphNumKey[] = {    /* NumberKey */
	GSIZE(150, 70),
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
	0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB,
	0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x07, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0xF8, 0x1F, 0xFF, 0xFE, 0x07, 0xFF, 0xFF, 0x80, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x0F, 0xE0, 0xC1, 0x01, 0xE0, 0x30, 0x40, 0x7C, 0x0C, 0x10, 0x1F, 0x03, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0xE0, 0xC1, 0x03, 0x30, 0x30, 0x40, 0xCE, 0x0C, 0x10, 0x3B, 0x83, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0xC0, 0xC1, 0x03, 0x30, 0x30, 0x40, 0xC6, 0x0C, 0x10, 0x31, 0x83, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x01, 0x80, 0xC1, 0x03, 0x30, 0x30, 0x40, 0xC6, 0x0C, 0x10, 0x31, 0x83, 0x04, 0x00, 0x40, 0xC0, 0x1B,
	0x60, 0x04, 0x01, 0x80, 0xC1, 0x01, 0xE0, 0x30, 0x40, 0xC6, 0x0C, 0x10, 0x31, 0x83, 0x04, 0x00, 0xC0, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x03, 0x30, 0x30, 0x40, 0x7E, 0x0C, 0x10, 0x31, 0x83, 0x04, 0x01, 0xC0, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x03, 0x30, 0x30, 0x40, 0x0C, 0x0C, 0x10, 0x31, 0x83, 0x04, 0x02, 0xC0, 0xC0, 0x1B,
	0x60, 0x04, 0x07, 0x00, 0xC1, 0x03, 0x30, 0x30, 0x40, 0x18, 0x0C, 0x10, 0x3B, 0x03, 0x04, 0x04, 0xC0, 0xC0, 0x1B,
	0x60, 0x04, 0x06, 0x00, 0xC1, 0x01, 0xE0, 0x30, 0x40, 0x70, 0x0C, 0x10, 0x1E, 0x03, 0x04, 0x07, 0xC0, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x08, 0xC0, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x10, 0xC0, 0xC0, 0x1B,
	0x60, 0x07, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0x04, 0x39, 0xE0, 0xC0, 0x1B,
	0x60, 0x03, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFC, 0x07, 0xFF, 0xFF, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x07, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0xF8, 0x1F, 0xFF, 0xFE, 0x04, 0x07, 0xA0, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x0C, 0x60, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x18, 0x40, 0xC0, 0x1B,
	0x60, 0x04, 0x01, 0x80, 0xC1, 0x03, 0xF0, 0x30, 0x40, 0x38, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x38, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x80, 0xC1, 0x03, 0x00, 0x30, 0x40, 0xE0, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x30, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x07, 0x80, 0xC1, 0x03, 0x00, 0x30, 0x41, 0x80, 0x0C, 0x10, 0x03, 0x03, 0x04, 0x30, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x0F, 0x80, 0xC1, 0x03, 0xE0, 0x30, 0x41, 0xF8, 0x0C, 0x10, 0x01, 0xC3, 0x04, 0x30, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x1D, 0x80, 0xC1, 0x00, 0x70, 0x30, 0x41, 0x9C, 0x0C, 0x11, 0xFF, 0xE3, 0x04, 0x18, 0xC0, 0xC0, 0x1B,
	0x60, 0x04, 0x19, 0x80, 0xC1, 0x00, 0x30, 0x30, 0x41, 0x8C, 0x0C, 0x10, 0x01, 0xC3, 0x04, 0x0F, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x1F, 0xE0, 0xC1, 0x00, 0x30, 0x30, 0x41, 0x8C, 0x0C, 0x10, 0x03, 0x03, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x01, 0x80, 0xC1, 0x00, 0x60, 0x30, 0x41, 0x98, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x01, 0x80, 0xC1, 0x07, 0xC0, 0x30, 0x40, 0xF0, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x01, 0x80, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x03, 0x04, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x07, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0xC0, 0x1B,
	0x60, 0x03, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFC, 0x0F, 0xFF, 0xFF, 0x03, 0xFF, 0xFF, 0xC0, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x07, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x07, 0x00, 0xC1, 0x03, 0xF0, 0x30, 0x40, 0xF8, 0x0C, 0x10, 0x00, 0xEB, 0xF9, 0xFC, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x03, 0x30, 0x30, 0x40, 0x0C, 0x0C, 0x10, 0x01, 0xB1, 0x8A, 0x68, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x00, 0x30, 0x30, 0x40, 0x0C, 0x0C, 0x10, 0x01, 0x93, 0x00, 0x60, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x00, 0x30, 0x30, 0x40, 0x18, 0x0C, 0x10, 0x01, 0x83, 0x20, 0xC0, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x00, 0x70, 0x30, 0x40, 0x78, 0x0C, 0x10, 0x00, 0xC3, 0xE0, 0xC0, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x00, 0xE0, 0x30, 0x40, 0x0C, 0x0C, 0x10, 0x00, 0xE3, 0x20, 0xC0, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x01, 0xC0, 0x30, 0x40, 0x0C, 0x0C, 0x10, 0x02, 0x66, 0x00, 0x80, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x00, 0xC1, 0x03, 0x00, 0x30, 0x40, 0x1C, 0x0C, 0x10, 0x06, 0x66, 0x11, 0x80, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x03, 0x80, 0xC1, 0x07, 0xF0, 0x30, 0x40, 0xF8, 0x0C, 0x10, 0x05, 0xCF, 0xE3, 0xC0, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x04, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x30, 0x40, 0x00, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x1B,
	0x60, 0x07, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xF0, 0x7F, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x1B,
	0x60, 0x03, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFC, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B,
	0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB,
	0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
};

byte far GraphPrickermiddle[] = {    /* Prickermiddle */
	GSIZE(114, 66),
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x7F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x3F,
	0x3F, 0x86, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x3F,
	0x1F, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x10, 0x1F, 0x3F,
	0x0F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3F, 0xBF,
	0x07, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x7F, 0x3F,
	0x03, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1E, 0xFE, 0x3F,
	0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFC, 0x3F,
	0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x3F,
	0x03, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x3F,
	0x07, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x3F,
	0x0F, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x3F,
	0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF8, 0x3F,
	0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFC, 0x3F,
	0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x3F,
	0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x3F,
	0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x3F,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x3F,
	0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x3F,
	0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x3F,
	0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x01, 0xF0, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x01, 0xF0, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x01, 0xF0, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x01, 0xF0, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x01, 0xF0, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x01, 0xF0, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x01, 0xF0, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x01, 0xF0, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x01, 0xF0, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x01, 0xF0, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x01, 0xF0, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x01, 0xF0, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x01, 0xF0, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x01, 0xF0, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x81, 0xF0, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC1, 0xF0, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE1, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF1, 0xF1, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF9, 0xF3, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFD, 0xF7, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F
};

byte far GraphPrickerleft[] = {    /* Prickerleft */
	GSIZE(114, 66),
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x7F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x3F, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x1F, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x0F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x07, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x03, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x03, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x07, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x0F, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F
};

byte far GraphPrickerright[] = {    /* Prickerright */
	GSIZE(114, 66),
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x1F, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3F, 0xBF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x7F, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0xFE, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFC, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF8, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFC, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F
};

byte far *GraphPricker[]={
	GraphPrickermiddle, GraphPrickerleft, GraphPrickerright
};