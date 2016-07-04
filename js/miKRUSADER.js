/**
 * KRUSADER - An Assembly Language IDE for the Replica 1
 * 6502 Version 1.3 - 23 December, 2007
 *
 * @copyright Ken Wessen (ken.wessen@gmail.com) 2007
 * @website http://school.anhb.uwa.edu.au/personalpages/kwessen/apple1/Krusader.htm
 *
 */

function miKRUSADER() {
	this.start = 0xF000;

	var assembler = [

		0xA9, 0x03, 0x85, 0xF8, 0xA9, 0x20, 0x85, 0xFF, 0xA9, 0x7C, 0x85, 0xF9, 0xA2, 0x1B, 0xBD, 0x67,
		0xFD, 0x20, 0xEF, 0xFF, 0xCA, 0xD0, 0xF7, 0xCA, 0x9A, 0x20, 0x71, 0xF0, 0xD8, 0xA9, 0x00, 0x85,
		0x5B, 0x20, 0xCE, 0xF0, 0xA2, 0x0F, 0x86, 0x58, 0x86, 0x59, 0x20, 0xE5, 0xFE, 0xA9, 0x3F, 0x20,
		0xEF, 0xFF, 0x20, 0xE0, 0xFE, 0x20, 0xEA, 0xFE, 0xC9, 0x08, 0xF0, 0xE0, 0xC9, 0x0D, 0xF0, 0x08,
		0x20, 0xEF, 0xFF, 0x95, 0x00, 0xE8, 0xD0, 0xED, 0xA5, 0x0F, 0xF0, 0xD0, 0xA5, 0x10, 0xF0, 0x04,
		0xC9, 0x20, 0xD0, 0x0C, 0xA2, 0x0D, 0xBD, 0x27, 0xFD, 0xC5, 0x0F, 0xF0, 0x0E, 0xCA, 0xD0, 0xF6,
		0x48, 0x48, 0xA0, 0x03, 0x68, 0x68, 0x20, 0x69, 0xF4, 0xD0, 0xB1, 0x20, 0xD6, 0xF0, 0x4C, 0x1C,
		0xF0, 0xA9, 0x00, 0xA8, 0x85, 0xFE, 0x91, 0xFE, 0xA5, 0xFF, 0x85, 0xFD, 0xA9, 0x00, 0x85, 0xFA,
		0x85, 0xFB, 0x85, 0xFC, 0x60, 0x20, 0x71, 0xF0, 0xA5, 0x11, 0xD0, 0x02, 0xA9, 0x01, 0x91, 0xFE,
		0x60, 0x20, 0xAB, 0xF0, 0xF0, 0xCC, 0x20, 0xE5, 0xFE, 0xA5, 0x3F, 0xA6, 0x3E, 0x4C, 0x67, 0xFB,
		0x20, 0xAB, 0xF0, 0xF0, 0xBD, 0x20, 0xE5, 0xFE, 0x6C, 0x3E, 0x00, 0xA2, 0x02, 0xB5, 0x0F, 0xF0,
		0x08, 0x48, 0x20, 0xD3, 0xF7, 0x68, 0xE8, 0xF0, 0xAB, 0x60, 0xA5, 0xF5, 0x85, 0x3E, 0xA5, 0xF6,
		0x85, 0x3F, 0x60, 0xA5, 0x3E, 0x85, 0xF5, 0xA5, 0x3F, 0x85, 0xF6, 0x60, 0xA9, 0x20, 0xA2, 0x27,
		0x95, 0xFF, 0xCA, 0xD0, 0xFB, 0x60, 0xBD, 0x34, 0xFD, 0x48, 0xBD, 0x41, 0xFD, 0x48, 0x60, 0x20,
		0x71, 0xF0, 0x4C, 0x58, 0xF1, 0x20, 0x78, 0xF0, 0x20, 0x1D, 0xF1, 0xF0, 0x03, 0x20, 0x53, 0xF2,
		0xA0, 0x00, 0xB1, 0xFC, 0xF0, 0x0E, 0x20, 0x72, 0xF2, 0x20, 0xDC, 0xF4, 0xAD, 0x11, 0xD0, 0x10,
		0xEF, 0xAD, 0x10, 0xD0, 0x60, 0x20, 0x4F, 0xF2, 0x20, 0xE5, 0xFE, 0xA2, 0x04, 0xB5, 0xFB, 0x20,
		0xDC, 0xFF, 0xE0, 0x03, 0xD0, 0x03, 0x20, 0xB9, 0xFE, 0xCA, 0xD0, 0xF1, 0x60, 0xA0, 0x00, 0x84,
		0x30, 0xA2, 0x01, 0xB5, 0x0F, 0xF0, 0x25, 0xC9, 0x20, 0xF0, 0x07, 0xC9, 0x24, 0xF0, 0x03, 0xE8,
		0xD0, 0xF1, 0xE6, 0x30, 0xA9, 0x24, 0x95, 0x0F, 0x20, 0x5B, 0xF9, 0xE8, 0xF0, 0x5F, 0xA5, 0x3E,
		0x99, 0x54, 0x00, 0xC8, 0xA5, 0x3F, 0x99, 0x54, 0x00, 0xC8, 0xD0, 0xD7, 0xA4, 0x30, 0x60, 0x20,
		0x1D, 0xF1, 0x88, 0xD0, 0x48, 0x20, 0xC7, 0xF1, 0x20, 0x1D, 0xF1, 0xE8, 0xF0, 0x3F, 0x98, 0xD0,
		0x06, 0x20, 0x4F, 0xF2, 0x18, 0x90, 0x03, 0x20, 0x53, 0xF2, 0x20, 0xDE, 0xF2, 0xE0, 0xFF, 0xF0,
		0xAB, 0x86, 0x2F, 0xA5, 0xFD, 0x85, 0x51, 0x85, 0x53, 0xA5, 0xFC, 0x85, 0x50, 0x18, 0x65, 0x2F,
		0x85, 0x52, 0x90, 0x02, 0xE6, 0x53, 0x20, 0xA0, 0xF1, 0x20, 0x25, 0xF2, 0xA0, 0x00, 0xB9, 0x00,
		0x00, 0x91, 0xFC, 0xC8, 0xC4, 0x2F, 0xD0, 0xF6, 0x20, 0xDC, 0xF4, 0xD0, 0xCD, 0x4C, 0x62, 0xF0,
		0xA2, 0xFC, 0xB5, 0xFE, 0x48, 0xE8, 0xD0, 0xFA, 0x20, 0x4F, 0xF2, 0x38, 0xA5, 0xFC, 0xE5, 0x50,
		0x85, 0x54, 0xA5, 0xFD, 0xE5, 0x51, 0x85, 0x55, 0xE6, 0x54, 0xD0, 0x02, 0xE6, 0x55, 0xA2, 0x04,
		0x68, 0x95, 0xF9, 0xCA, 0xD0, 0xFA, 0x60, 0x20, 0x1D, 0xF1, 0xF0, 0xD1, 0x84, 0x30, 0x20, 0x53,
		0xF2, 0xE0, 0xFF, 0xF0, 0xC8, 0xA5, 0xFC, 0x85, 0x52, 0xA5, 0xFD, 0x85, 0x53, 0xA5, 0x30, 0x4A,
		0xF0, 0x0C, 0xA6, 0x57, 0xA4, 0x56, 0xE4, 0x55, 0xD0, 0x02, 0xC4, 0x54, 0x90, 0xAF, 0xC8, 0xD0,
		0x01, 0xE8, 0x86, 0x55, 0x84, 0x54, 0x20, 0x53, 0xF2, 0xA5, 0xFC, 0x85, 0x50, 0xA5, 0xFD, 0x85,
		0x51, 0x20, 0xA0, 0xF1, 0xA0, 0x00, 0xA6, 0x55, 0xF0, 0x0E, 0xB1, 0x50, 0x91, 0x52, 0xC8, 0xD0,
		0xF9, 0xE6, 0x51, 0xE6, 0x53, 0xCA, 0xD0, 0xF2, 0xA6, 0x54, 0xF0, 0x08, 0xB1, 0x50, 0x91, 0x52,
		0xC8, 0xCA, 0xD0, 0xF8, 0x60, 0xA6, 0x55, 0x18, 0x8A, 0x65, 0x51, 0x85, 0x51, 0x18, 0x8A, 0x65,
		0x53, 0x85, 0x53, 0xE8, 0xA4, 0x54, 0xF0, 0x0E, 0x88, 0xF0, 0x07, 0xB1, 0x50, 0x91, 0x52, 0x88,
		0xD0, 0xF9, 0xB1, 0x50, 0x91, 0x52, 0x88, 0xC6, 0x51, 0xC6, 0x53, 0xCA, 0xD0, 0xED, 0x60, 0xA9,
		0xFF, 0x85, 0x55, 0x20, 0x78, 0xF0, 0xA4, 0x54, 0xC4, 0xFA, 0xD0, 0x06, 0xA6, 0x55, 0xE4, 0xFB,
		0xF0, 0x51, 0xA0, 0xFF, 0xC8, 0xB1, 0xFC, 0xD0, 0xFB, 0x98, 0xF0, 0x45, 0xC8, 0x20, 0xDC, 0xF4,
		0xD0, 0xE4, 0x20, 0xE5, 0xFE, 0x86, 0x2F, 0x20, 0xEE, 0xF3, 0xC8, 0x20, 0xD6, 0xFE, 0xA2, 0x00,
		0xB5, 0x04, 0xF0, 0x06, 0x20, 0xEF, 0xFF, 0xE8, 0xD0, 0xF6, 0xA6, 0x2F, 0x60, 0x20, 0xEA, 0xFE,
		0xC9, 0x09, 0xD0, 0x02, 0xA9, 0x20, 0xC9, 0x20, 0x10, 0x1A, 0xA8, 0x68, 0x68, 0x68, 0x68, 0xC0,
		0x08, 0xF0, 0x3B, 0xC0, 0x0D, 0xD0, 0x0A, 0xE0, 0x04, 0xF0, 0x29, 0xA9, 0x00, 0x95, 0x00, 0xF0,
		0x5C, 0xA2, 0xFF, 0x60, 0xE0, 0x27, 0x10, 0x1A, 0xC9, 0x5E, 0x10, 0x16, 0x38, 0x60, 0xC9, 0x2E,
		0xF0, 0xFA, 0xC9, 0x30, 0x30, 0x0C, 0xC9, 0x3A, 0x30, 0xF2, 0xC9, 0x41, 0x30, 0x04, 0xC9, 0x5B,
		0x30, 0xEA, 0x18, 0x60, 0xA9, 0x02, 0xAA, 0x85, 0x00, 0xA9, 0x00, 0x85, 0x01, 0x60, 0x20, 0xCC,
		0xF0, 0xA9, 0x00, 0x85, 0x1D, 0x20, 0xE5, 0xFE, 0x20, 0xD6, 0xFE, 0xA2, 0x04, 0xA9, 0x0A, 0x20,
		0x9C, 0xF3, 0x20, 0xC4, 0xF3, 0xA5, 0x04, 0xC9, 0x3B, 0xF0, 0x0D, 0xA9, 0x0E, 0x20, 0x9C, 0xF3,
		0x20, 0xC4, 0xF3, 0xA9, 0x1D, 0x20, 0x9C, 0xF3, 0xA9, 0x00, 0x20, 0x9C, 0xF3, 0xA2, 0x00, 0x86,
		0x51, 0xA9, 0x20, 0x85, 0x55, 0xA9, 0x04, 0x85, 0x50, 0xA9, 0x01, 0x85, 0x54, 0x20, 0xD7, 0xF3,
		0xA4, 0x04, 0xC0, 0x3B, 0xD0, 0x04, 0xA9, 0x0B, 0xD0, 0x5F, 0x8A, 0x48, 0x18, 0x66, 0x56, 0xA2,
		0x03, 0x38, 0xB5, 0x0A, 0xE9, 0x40, 0xA0, 0x05, 0x4A, 0x66, 0x56, 0x66, 0x57, 0x88, 0xD0, 0xF8,
		0xCA, 0xD0, 0xEE, 0xA2, 0x38, 0xBD, 0xE8, 0xFB, 0xC5, 0x56, 0xD0, 0x07, 0xBD, 0x20, 0xFC, 0xC5,
		0x57, 0xF0, 0x03, 0xCA, 0xD0, 0xEF, 0xCA, 0x8A, 0xC9, 0xFF, 0xD0, 0x19, 0xA5, 0x0B, 0xC9, 0x2E,
		0xD0, 0x0C, 0xA2, 0x05, 0xA5, 0x0C, 0xDD, 0x4E, 0xFD, 0xF0, 0x09, 0xCA, 0xD0, 0xF8, 0x68, 0xA0,
		0x01, 0x4C, 0x69, 0xF4, 0xCA, 0xA8, 0xC8, 0x68, 0xAA, 0x94, 0x00, 0xE8, 0xA9, 0x0F, 0x85, 0x50,
		0x20, 0xD7, 0xF3, 0x86, 0x2F, 0xE6, 0x2F, 0xA9, 0x1D, 0x85, 0x50, 0xA9, 0x00, 0x85, 0x54, 0x85,
		0x55, 0x20, 0xD7, 0xF3, 0xE4, 0x2F, 0xD0, 0x03, 0xCA, 0x95, 0xFF, 0x60, 0x85, 0x54, 0x20, 0x8D,
		0xF2, 0x90, 0xFB, 0x20, 0xEF, 0xFF, 0x95, 0x00, 0xE8, 0xC9, 0x20, 0xF0, 0x05, 0xE4, 0x54, 0xD0,
		0xED, 0x60, 0xA5, 0x54, 0xF0, 0xE8, 0xE4, 0x54, 0xF0, 0xF7, 0xA9, 0x20, 0x95, 0x00, 0x20, 0xEF,
		0xFF, 0xE8, 0xD0, 0xEE, 0xB5, 0xFF, 0xC9, 0x20, 0xF0, 0x07, 0x20, 0x8D, 0xF2, 0xC9, 0x20, 0xD0,
		0xF9, 0x95, 0x00, 0xE8, 0x4C, 0xEF, 0xFF, 0xA0, 0x00, 0xB1, 0x50, 0xF0, 0x0B, 0xC5, 0x55, 0xF0,
		0x07, 0x95, 0x00, 0xE8, 0xE6, 0x50, 0xD0, 0xF1, 0xA5, 0x54, 0x95, 0x00, 0xE8, 0x60, 0x20, 0xCC,
		0xF0, 0xA0, 0x00, 0xA2, 0x04, 0xB1, 0xFC, 0xF0, 0x4D, 0xC9, 0x02, 0xD0, 0x05, 0xC8, 0xA9, 0x00,
		0xF0, 0x46, 0xC9, 0x01, 0xF0, 0x06, 0x95, 0x00, 0xE8, 0xC8, 0xD0, 0xE9, 0xA5, 0x04, 0xC9, 0x3B,
		0xD0, 0x04, 0xA2, 0x0B, 0xD0, 0x2D, 0xC8, 0xB1, 0xFC, 0xAA, 0xCA, 0x86, 0x3C, 0xE0, 0x38, 0x10,
		0x09, 0x98, 0x48, 0x20, 0x99, 0xFA, 0x68, 0xA8, 0xD0, 0x06, 0x86, 0x0C, 0xA9, 0x2E, 0x85, 0x0B,
		0xC8, 0xA2, 0x0F, 0xB1, 0xFC, 0xF0, 0x11, 0xC9, 0x01, 0xD0, 0x05, 0xC8, 0xA2, 0x1D, 0xD0, 0xF3,
		0x95, 0x00, 0xE8, 0xC8, 0xD0, 0xED, 0xA2, 0xFE, 0x95, 0x00, 0x60, 0x20, 0x94, 0xF4, 0x20, 0xE5,
		0xFE, 0x20, 0xF8, 0xF5, 0x20, 0xB8, 0xF4, 0xE8, 0xF0, 0x0F, 0xE0, 0xFF, 0xD0, 0xF6, 0xE6, 0x58,
		0x20, 0x10, 0xF6, 0xE8, 0xF0, 0x03, 0x4C, 0xF8, 0xF5, 0x20, 0xE5, 0xFE, 0xA2, 0x05, 0xBD, 0x53,
		0xFD, 0x20, 0xEF, 0xFF, 0xCA, 0xD0, 0xF7, 0x98, 0x18, 0x8A, 0x69, 0x03, 0x88, 0xD0, 0xFB, 0xA8,
		0xA2, 0x03, 0xB9, 0x56, 0xFD, 0x20, 0xEF, 0xFF, 0xC8, 0xCA, 0xD0, 0xF6, 0xCA, 0xA5, 0x59, 0xD0,
		0x26, 0x4C, 0x72, 0xF2, 0x20, 0x78, 0xF0, 0x85, 0x58, 0x85, 0xEB, 0x85, 0xE9, 0x85, 0xF5, 0xA5,
		0xF8, 0x85, 0xF6, 0x20, 0xEB, 0xF5, 0x86, 0xEA, 0xA9, 0x00, 0x85, 0x2B, 0x85, 0x29, 0x85, 0x46,
		0xA4, 0xF9, 0xC8, 0x84, 0x2A, 0x84, 0x47, 0x60, 0x20, 0xEE, 0xF3, 0xE0, 0xFE, 0xF0, 0x1D, 0xE0,
		0x04, 0xF0, 0x18, 0xA9, 0x00, 0x85, 0x59, 0x85, 0x58, 0x85, 0x5A, 0x20, 0x44, 0xF5, 0xE0, 0xFF,
		0xF0, 0x1D, 0xA0, 0x00, 0xB1, 0xFC, 0xF0, 0x03, 0xC8, 0xD0, 0xF9, 0xC8, 0xA5, 0xFC, 0x84, 0x44,
		0x18, 0x65, 0x44, 0x85, 0xFC, 0x90, 0x02, 0xE6, 0xFD, 0xE6, 0xFA, 0xD0, 0x02, 0xE6, 0xFB, 0x60,
		0xA4, 0x3C, 0xB9, 0x73, 0xFC, 0xA6, 0x3D, 0x18, 0x7D, 0xAB, 0xFC, 0xE0, 0x0B, 0xF0, 0x0E, 0xE0,
		0x02, 0xD0, 0x11, 0xC0, 0x28, 0x30, 0x0D, 0xC0, 0x30, 0xB0, 0x09, 0x69, 0x08, 0xC0, 0x35, 0xD0,
		0x03, 0x18, 0x69, 0x04, 0x20, 0x2F, 0xF5, 0xC9, 0x00, 0xD0, 0x03, 0x20, 0x2F, 0xF5, 0x8A, 0xF0,
		0xCE, 0xCA, 0xF0, 0xCB, 0xA5, 0x3E, 0xE0, 0x08, 0x30, 0x05, 0x20, 0x2F, 0xF5, 0xA5, 0x3F, 0xA0,
		0x00, 0x91, 0xF5, 0xE6, 0xF5, 0xD0, 0x02, 0xE6, 0xF6, 0x60, 0x20, 0xA9, 0xF6, 0xE0, 0xFF, 0xD0,
		0xAF, 0xA0, 0x02, 0x60, 0xA5, 0x04, 0xC9, 0x3B, 0xF0, 0xA5, 0xA6, 0x0B, 0xE0, 0x2E, 0xD0, 0x0D,
		0xA6, 0x0C, 0xE0, 0x4D, 0xD0, 0x03, 0x4C, 0xB6, 0xF5, 0xE0, 0x3D, 0xF0, 0x47, 0xC9, 0x20, 0xF0,
		0x03, 0x20, 0xDF, 0xF8, 0xA5, 0x0B, 0xC9, 0x2E, 0xD0, 0xD0, 0xA2, 0x00, 0xA5, 0x0C, 0xC9, 0x53,
		0xF0, 0x19, 0x85, 0x58, 0x20, 0x7A, 0xF7, 0xE8, 0xF0, 0x0C, 0xA5, 0x3E, 0xA6, 0x0C, 0xE0, 0x57,
		0xF0, 0xA8, 0xA6, 0x3F, 0xF0, 0xA9, 0xA0, 0x03, 0xA2, 0xFF, 0x60, 0xB5, 0x0F, 0xC9, 0x27, 0xD0,
		0xF5, 0xE8, 0xB5, 0x0F, 0xF0, 0xF0, 0xC9, 0x27, 0xF0, 0x09, 0x20, 0x2F, 0xF5, 0xE0, 0x0E, 0xD0,
		0xF0, 0xF0, 0xE3, 0x60, 0x85, 0x58, 0x20, 0xC2, 0xF2, 0x90, 0xDB, 0xA2, 0x00, 0x20, 0x7A, 0xF7,
		0xE8, 0xF0, 0xD3, 0x4C, 0xE2, 0xF8, 0x20, 0xC2, 0xF2, 0x90, 0xCB, 0xA0, 0x00, 0xA5, 0x0F, 0xF0,
		0x14, 0xC9, 0x20, 0xF0, 0x10, 0x20, 0xF6, 0xF5, 0xA2, 0x00, 0xA5, 0x0F, 0x20, 0x5B, 0xF9, 0xE8,
		0xF0, 0xB4, 0x20, 0xC3, 0xF0, 0x20, 0xDF, 0xF8, 0xE0, 0xFF, 0xF0, 0xC7, 0x20, 0x10, 0xF6, 0xE0,
		0xFF, 0xF0, 0xC0, 0x20, 0xC4, 0xFE, 0xA9, 0x00, 0x20, 0xF6, 0xF5, 0xA2, 0x00, 0x86, 0xEE, 0x86,
		0xEC, 0xA6, 0xF9, 0x86, 0xED, 0x60, 0x85, 0x58, 0xA5, 0xF6, 0xA6, 0xF5, 0xA4, 0x58, 0xF0, 0x0D,
		0x48, 0x20, 0xB9, 0xFE, 0x68, 0xE0, 0x00, 0xD0, 0x03, 0x38, 0xE9, 0x01, 0xCA, 0x4C, 0x67, 0xFB,
		0xA6, 0x2B, 0xF0, 0x72, 0x86, 0x59, 0x86, 0x45, 0xA5, 0xF5, 0x48, 0xA5, 0xF6, 0x48, 0x20, 0xA8,
		0xF4, 0xA0, 0x00, 0xA5, 0x58, 0x85, 0x48, 0x84, 0x5A, 0xB1, 0x46, 0xC9, 0x2E, 0xD0, 0x02, 0x85,
		0x58, 0xB1, 0x46, 0x99, 0x1D, 0x00, 0xC8, 0xC0, 0x06, 0xD0, 0xF6, 0xB1, 0x46, 0x85, 0xF5, 0xC8,
		0xB1, 0x46, 0x85, 0xF6, 0xC8, 0xB1, 0x46, 0x85, 0x54, 0x20, 0x7E, 0xF8, 0xE0, 0xFF, 0xF0, 0x47,
		0xA5, 0x5A, 0xF0, 0x04, 0xA5, 0x54, 0x91, 0x50, 0x20, 0xC3, 0xF7, 0xA0, 0x00, 0xB1, 0xF5, 0x29,
		0x1F, 0xC9, 0x10, 0xF0, 0x22, 0x20, 0x33, 0xF5, 0xA5, 0x3E, 0x20, 0x2A, 0xF5, 0x18, 0xA5, 0x46,
		0x69, 0x09, 0x85, 0x46, 0x90, 0x02, 0xE6, 0x47, 0xA5, 0x48, 0x85, 0x58, 0xC6, 0x45, 0xD0, 0xA1,
		0x68, 0x85, 0xF6, 0x68, 0x85, 0xF5, 0x60, 0x20, 0x62, 0xF7, 0xE0, 0xFF, 0xF0, 0x09, 0xA0, 0x01,
		0xA5, 0x3E, 0x91, 0xF5, 0x4C, 0x6D, 0xF6, 0xA0, 0x00, 0x20, 0xE0, 0xFE, 0xB1, 0x46, 0x20, 0xEF,
		0xFF, 0xC8, 0xC0, 0x06, 0xD0, 0xF6, 0x88, 0xD0, 0xD7, 0xA2, 0xFF, 0x86, 0x3D, 0xA5, 0x3C, 0xA6,
		0x0F, 0xF0, 0x04, 0xE0, 0x20, 0xD0, 0x0E, 0xA2, 0x00, 0x20, 0x3D, 0xF7, 0xE0, 0xFF, 0xD0, 0x35,
		0xA2, 0x01, 0x4C, 0x3D, 0xF7, 0xE0, 0x23, 0xF0, 0x0E, 0xA2, 0x03, 0x20, 0x3D, 0xF7, 0xE0, 0xFF,
		0xF0, 0x24, 0xA5, 0x0F, 0x4C, 0x4F, 0xF7, 0xC9, 0x2C, 0xF0, 0x71, 0xA2, 0x02, 0xC9, 0x35, 0xF0,
		0x07, 0x20, 0x3D, 0xF7, 0xE0, 0xFF, 0xF0, 0x0D, 0x86, 0x3D, 0xCA, 0x20, 0x7A, 0xF7, 0xE8, 0xF0,
		0x5B, 0xA5, 0x3F, 0xD0, 0x57, 0x60, 0xA2, 0x00, 0xA5, 0x0F, 0xC9, 0x28, 0xD0, 0x01, 0xE8, 0x20,
		0xD3, 0xF7, 0xE0, 0xFF, 0xF0, 0xEF, 0x20, 0x9C, 0xF9, 0xE0, 0xFF, 0xF0, 0xE8, 0x86, 0x3D, 0xE0,
		0x06, 0xD0, 0x0E, 0xA5, 0x3C, 0xC9, 0x28, 0x90, 0x08, 0xC9, 0x30, 0xB0, 0x04, 0xA2, 0x0B, 0xD0,
		0x28, 0xA0, 0x06, 0xB9, 0x15, 0xFD, 0xC5, 0x3C, 0xD0, 0x0E, 0xBE, 0x1B, 0xFD, 0xE4, 0x3D, 0xF0,
		0x18, 0xBE, 0x21, 0xFD, 0xE4, 0x3D, 0xF0, 0x11, 0x88, 0xD0, 0xE8, 0xA6, 0x3D, 0xA5, 0x3C, 0xDD,
		0x59, 0xFC, 0x90, 0x08, 0xDD, 0x66, 0xFC, 0xB0, 0x03, 0x86, 0x3D, 0x60, 0xA2, 0xFF, 0x60, 0xA2,
		0x00, 0x86, 0x3E, 0x86, 0x3F, 0xC9, 0x2A, 0xD0, 0x06, 0x20, 0xBA, 0xF0, 0x20, 0xFD, 0xF7, 0x20,
		0xD3, 0xF7, 0x38, 0xA5, 0x3E, 0xE5, 0xF5, 0x85, 0x3E, 0xA5, 0x3F, 0xE5, 0xF6, 0x85, 0x3F, 0xF0,
		0x04, 0xE6, 0x3F, 0xD0, 0xD7, 0xC6, 0x3E, 0xC6, 0x3E, 0x60, 0xB5, 0x0F, 0xF0, 0xCE, 0xC9, 0x27,
		0xF0, 0x03, 0x4C, 0xD3, 0xF7, 0xE8, 0xA9, 0x00, 0x85, 0x3F, 0xB5, 0x0F, 0x85, 0x3E, 0xE8, 0xB5,
		0x0F, 0xC9, 0x27, 0xD0, 0xB7, 0xE8, 0xB5, 0x0F, 0xF0, 0x7C, 0xC9, 0x20, 0xF0, 0x78, 0x48, 0xE8,
		0xB5, 0x0F, 0x20, 0x51, 0xFA, 0xE0, 0xFF, 0xD0, 0x02, 0x68, 0x60, 0x85, 0x54, 0x68, 0xC9, 0x2B,
		0xF0, 0x09, 0xA5, 0x54, 0x18, 0x49, 0xFF, 0x69, 0x01, 0x85, 0x54, 0xA5, 0x5A, 0xF0, 0x04, 0xA5,
		0x54, 0x91, 0x50, 0xA5, 0x54, 0x10, 0x02, 0xC6, 0x3F, 0x18, 0x65, 0x3E, 0x85, 0x3E, 0x90, 0x02,
		0xE6, 0x3F, 0x60, 0x86, 0x56, 0xB5, 0x0F, 0xC9, 0x3C, 0xF0, 0x04, 0xC9, 0x3E, 0xD0, 0x05, 0x85,
		0x58, 0xE8, 0xB5, 0x0F, 0x20, 0xBE, 0xF2, 0xB0, 0x09, 0x20, 0x5B, 0xF9, 0xE0, 0xFF, 0xF0, 0x24,
		0xD0, 0x0B, 0x86, 0x2F, 0x20, 0x60, 0xF8, 0xE0, 0xFF, 0xF0, 0x1B, 0xA6, 0x2F, 0xE8, 0xB5, 0x0F,
		0x20, 0xBE, 0xF2, 0xB0, 0xF8, 0xC9, 0x2B, 0xF0, 0x04, 0xC9, 0x2D, 0xD0, 0x0A, 0x20, 0x9E, 0xF7,
		0xE0, 0xFF, 0xD0, 0xE9, 0xA0, 0x03, 0x60, 0xA0, 0x00, 0xA5, 0x58, 0xC9, 0x3C, 0xF0, 0x08, 0xC9,
		0x3E, 0xD0, 0x06, 0xA5, 0x3F, 0x85, 0x3E, 0x84, 0x3F, 0xB5, 0x0F, 0x99, 0x1D, 0x00, 0xF0, 0x0A,
		0xC9, 0x20, 0xF0, 0x06, 0xE8, 0xC8, 0xE0, 0x0E, 0xD0, 0xEF, 0xA9, 0x00, 0x99, 0x1D, 0x00, 0xA4,
		0x56, 0xA9, 0x24, 0x99, 0x0F, 0x00, 0xC8, 0xA5, 0x3F, 0xF0, 0x03, 0x20, 0x82, 0xFA, 0xA5, 0x3E,
		0x20, 0x82, 0xFA, 0xA2, 0x00, 0xB5, 0x1D, 0x99, 0x0F, 0x00, 0xF0, 0xBA, 0xE8, 0xC8, 0xD0, 0xF5,
		0xA0, 0x00, 0xC0, 0x06, 0xF0, 0x18, 0x20, 0xBE, 0xF2, 0x90, 0x09, 0x99, 0x1D, 0x00, 0xE8, 0xB5,
		0x0F, 0xC8, 0xD0, 0xEE, 0xA9, 0x20, 0x99, 0x1D, 0x00, 0xC8, 0xC0, 0x06, 0xD0, 0xF8, 0xA9, 0x1D,
		0x85, 0x42, 0xA2, 0x00, 0x86, 0x43, 0xA9, 0x06, 0x85, 0x2E, 0xA9, 0x08, 0x85, 0x2D, 0xA5, 0x1D,
		0xC9, 0x2E, 0xF0, 0x11, 0x20, 0x8A, 0xF9, 0xF0, 0x13, 0xA0, 0x06, 0xB1, 0x40, 0x85, 0x3E, 0xC8,
		0xB1, 0x40, 0x85, 0x3F, 0x60, 0xA2, 0x03, 0x20, 0x8A, 0xF9, 0xD0, 0xED, 0xA5, 0x58, 0xD0, 0x4F,
		0x20, 0xBA, 0xF0, 0xA5, 0x2A, 0x85, 0x51, 0xA5, 0x29, 0xA6, 0x2B, 0xF0, 0x0A, 0x18, 0x69, 0x09,
		0x90, 0x02, 0xE6, 0x51, 0xCA, 0xD0, 0xF6, 0x85, 0x50, 0xE6, 0x2B, 0xA5, 0x2B, 0xC9, 0x55, 0x10,
		0x32, 0xA9, 0x1D, 0x85, 0x5A, 0x85, 0x52, 0x20, 0x43, 0xF9, 0xC8, 0x8A, 0x91, 0x50, 0x60, 0x20,
		0xBA, 0xF0, 0xA9, 0x04, 0x85, 0x52, 0x85, 0x42, 0xA2, 0x00, 0x86, 0x43, 0xA9, 0x06, 0x85, 0x2E,
		0xA5, 0x04, 0xC9, 0x2E, 0xD0, 0x02, 0xA2, 0x03, 0x20, 0x8A, 0xF9, 0xF0, 0x0B, 0x68, 0x68, 0xA0,
		0x05, 0xD0, 0x02, 0xA0, 0x04, 0xA2, 0xFF, 0x60, 0xA6, 0x04, 0xE0, 0x2E, 0xF0, 0x17, 0x38, 0xA5,
		0xE9, 0xE9, 0x08, 0xB0, 0x02, 0xC6, 0xEA, 0x85, 0xE9, 0xE6, 0xEB, 0xF0, 0xE6, 0x85, 0x50, 0xA5,
		0xEA, 0x85, 0x51, 0xD0, 0x1E, 0xA5, 0xED, 0x85, 0x51, 0xA5, 0xEC, 0xA6, 0xEE, 0xF0, 0x0A, 0x18,
		0x69, 0x08, 0x90, 0x02, 0xE6, 0x51, 0xCA, 0xD0, 0xF6, 0x85, 0x50, 0xE6, 0xEE, 0xA5, 0xEE, 0xC9,
		0x20, 0x10, 0xC0, 0xA0, 0x00, 0x84, 0x53, 0xA2, 0x06, 0xB1, 0x52, 0x91, 0x50, 0xC8, 0xCA, 0xD0,
		0xF8, 0xA5, 0x3E, 0x91, 0x50, 0xC8, 0xA5, 0x3F, 0x91, 0x50, 0x60, 0xC9, 0x24, 0xD0, 0xA6, 0x84,
		0x1E, 0x20, 0x18, 0xFA, 0xE0, 0xFF, 0xF0, 0x9D, 0x85, 0x1D, 0xA0, 0x00, 0x84, 0x3F, 0xCA, 0xCA,
		0xB5, 0x0F, 0xC9, 0x24, 0xF0, 0x06, 0x20, 0x51, 0xFA, 0x38, 0xB0, 0x03, 0x20, 0x6D, 0xFA, 0x99,
		0x3E, 0x00, 0xC8, 0xC4, 0x1D, 0xD0, 0xE7, 0xA4, 0x1E, 0x60, 0xB5, 0xE9, 0x85, 0x40, 0xB5, 0xEA,
		0x85, 0x41, 0xB5, 0xEB, 0x85, 0x2C, 0x20, 0x2C, 0xFA, 0xE0, 0xFF, 0x60, 0xA2, 0x00, 0xA9, 0x04,
		0xB4, 0x0F, 0xC0, 0x28, 0xD0, 0x04, 0x18, 0x69, 0x03, 0xE8, 0x48, 0x20, 0x18, 0xFA, 0xA8, 0xCA,
		0xA5, 0x3C, 0xC9, 0x21, 0xF0, 0x04, 0xC9, 0x23, 0xD0, 0x01, 0xC8, 0x68, 0xE8, 0xF0, 0x56, 0x88,
		0xF0, 0x03, 0x18, 0x69, 0x06, 0xA8, 0xB5, 0x0F, 0xF0, 0x04, 0xC9, 0x20, 0xD0, 0x14, 0xA5, 0x0F,
		0xC9, 0x28, 0xF0, 0x41, 0xC0, 0x0F, 0x10, 0x3D, 0xC0, 0x07, 0xF0, 0x39, 0x30, 0x01, 0x88, 0x98,
		0xAA, 0x60, 0xC9, 0x29, 0xD0, 0x0B, 0xA9, 0x20, 0x85, 0x0F, 0xE8, 0xB5, 0x0F, 0xC9, 0x2C, 0xD0,
		0xD5, 0xB5, 0x0F, 0xC9, 0x2C, 0xD0, 0x1E, 0xE8, 0xB5, 0x0F, 0xC9, 0x58, 0xF0, 0x0D, 0xC9, 0x59,
		0xD0, 0x13, 0xA5, 0x0F, 0xC9, 0x28, 0xF0, 0x0D, 0x95, 0x0D, 0xC8, 0xC8, 0xB5, 0x0D, 0xC9, 0x29,
		0xF0, 0x03, 0xE8, 0xD0, 0xB1, 0xA2, 0xFF, 0x60, 0xA0, 0x00, 0xE8, 0xC8, 0x20, 0x6E, 0xFA, 0xC9,
		0xFF, 0xD0, 0xF7, 0x98, 0x4A, 0xF0, 0xEE, 0xC9, 0x03, 0xB0, 0xEA, 0x60, 0xA5, 0x2C, 0xF0, 0xE5,
		0xA2, 0x00, 0xA0, 0xFF, 0xC8, 0xC4, 0x2E, 0xF0, 0xDE, 0xB1, 0x40, 0xD1, 0x42, 0xF0, 0xF5, 0xE8,
		0xE4, 0x2C, 0xF0, 0xD1, 0xA5, 0x40, 0x18, 0x65, 0x2D, 0x85, 0x40, 0x90, 0xE5, 0xE6, 0x41, 0xB0,
		0xE1, 0x20, 0x6E, 0xFA, 0xC9, 0xFF, 0xF0, 0xBD, 0x48, 0x20, 0x6D, 0xFA, 0xCA, 0xC9, 0xFF, 0xD0,
		0x02, 0x68, 0x60, 0x85, 0x44, 0x68, 0x0A, 0x0A, 0x0A, 0x0A, 0x65, 0x44, 0x60, 0xE8, 0xB5, 0x0F,
		0x49, 0x30, 0xC9, 0x0A, 0x90, 0x08, 0x69, 0x88, 0xC9, 0xFA, 0x90, 0x03, 0x29, 0x0F, 0x60, 0xA9,
		0xFF, 0x60, 0x48, 0x20, 0xD6, 0xFB, 0x20, 0x8A, 0xFA, 0x68, 0x29, 0x0F, 0x09, 0x30, 0xC9, 0x3A,
		0x90, 0x02, 0x69, 0x06, 0x99, 0x0F, 0x00, 0xC8, 0x60, 0xBD, 0xE9, 0xFB, 0x85, 0x56, 0xBD, 0x21,
		0xFC, 0x85, 0x57, 0xA2, 0x00, 0xA9, 0x00, 0xA0, 0x05, 0x06, 0x57, 0x26, 0x56, 0x2A, 0x88, 0xD0,
		0xF8, 0x69, 0x40, 0x95, 0x0B, 0xA4, 0x5B, 0xF0, 0x03, 0x20, 0xEF, 0xFF, 0xE8, 0xE0, 0x03, 0xD0,
		0xE4, 0x60, 0x20, 0xAB, 0xF0, 0xF0, 0x03, 0x20, 0xC3, 0xF0, 0x20, 0xDC, 0xFA, 0x20, 0x81, 0xFB,
		0x85, 0xF5, 0x84, 0xF6, 0xAD, 0x11, 0xD0, 0x10, 0xF1, 0xAD, 0x10, 0xD0, 0x20, 0x6E, 0xFB, 0xA1,
		0xF5, 0xA8, 0x4A, 0x90, 0x09, 0x6A, 0xB0, 0x14, 0xC9, 0xA2, 0xF0, 0x10, 0x29, 0x87, 0x4A, 0xAA,
		0xBD, 0xB8, 0xFC, 0x90, 0x03, 0x20, 0xD6, 0xFB, 0x29, 0x0F, 0xD0, 0x04, 0xA0, 0x80, 0xA9, 0x00,
		0xAA, 0xBD, 0xFC, 0xFC, 0x85, 0x29, 0x29, 0x03, 0x85, 0x2A, 0x98, 0x20, 0x90, 0xFB, 0xA0, 0x00,
		0x48, 0xB1, 0xF5, 0x20, 0xDC, 0xFF, 0xA2, 0x01, 0x20, 0x7A, 0xFB, 0xC4, 0x2A, 0xC8, 0x90, 0xF1,
		0xA2, 0x03, 0x86, 0x5B, 0xC0, 0x04, 0x90, 0xF0, 0x68, 0xAA, 0x20, 0x99, 0xFA, 0x20, 0x78, 0xFB,
		0xA4, 0x2A, 0xA2, 0x06, 0xE0, 0x03, 0xF0, 0x1E, 0x06, 0x29, 0x90, 0x0E, 0xBD, 0x09, 0xFD, 0x20,
		0xEF, 0xFF, 0xBD, 0x0F, 0xFD, 0xF0, 0x03, 0x20, 0xEF, 0xFF, 0xCA, 0xD0, 0xE7, 0x86, 0x5B, 0x60,
		0x88, 0x30, 0xE5, 0x20, 0xDC, 0xFF, 0xA5, 0x29, 0xC9, 0xE8, 0xB1, 0xF5, 0x90, 0xF2, 0x20, 0x84,
		0xFB, 0xAA, 0xE8, 0xD0, 0x01, 0xC8, 0x98, 0x20, 0xDC, 0xFF, 0x8A, 0x4C, 0xDC, 0xFF, 0x20, 0xE5,
		0xFE, 0xA5, 0xF6, 0xA6, 0xF5, 0x20, 0x67, 0xFB, 0xA2, 0x03, 0x20, 0xE0, 0xFE, 0xCA, 0xD0, 0xFA,
		0x60, 0x38, 0xA5, 0x2A, 0xA4, 0xF6, 0xAA, 0x10, 0x01, 0x88, 0x65, 0xF5, 0x90, 0x01, 0xC8, 0x60,
		0x85, 0x54, 0x29, 0x8F, 0xC9, 0x8A, 0xF0, 0x43, 0x0A, 0xC9, 0x10, 0xF0, 0x37, 0xA5, 0x54, 0x0A,
		0x69, 0x80, 0x2A, 0x0A, 0x29, 0x1F, 0x69, 0x20, 0x48, 0xA5, 0x54, 0x29, 0x9F, 0xF0, 0x1B, 0x0A,
		0xC9, 0x20, 0xF0, 0x10, 0x29, 0x06, 0xD0, 0x2F, 0x68, 0x29, 0x07, 0xC9, 0x03, 0x10, 0x02, 0x69,
		0x02, 0x69, 0x1F, 0x60, 0x68, 0x29, 0x07, 0x69, 0x18, 0x60, 0x68, 0xAA, 0xBD, 0xB0, 0xFB, 0x60,
		0x16, 0x21, 0x17, 0x18, 0xA5, 0x54, 0x4A, 0x4A, 0x4A, 0x4A, 0x60, 0x20, 0xD4, 0xFB, 0xC9, 0x0E,
		0xD0, 0x02, 0x69, 0xFD, 0x69, 0x08, 0x60, 0x68, 0x60, 0x82, 0x1B, 0x83, 0x99, 0x82, 0x1B, 0x83,
		0x99, 0x21, 0xA6, 0xA0, 0x1B, 0x4B, 0x1B, 0x4B, 0x99, 0xA6, 0xA6, 0xA0, 0xA4, 0x21, 0x73, 0x14,
		0x95, 0x95, 0x14, 0x13, 0x15, 0x15, 0x10, 0x10, 0x13, 0x11, 0x54, 0x12, 0x53, 0x9D, 0x61, 0x1C,
		0x1C, 0x7C, 0x0B, 0x2B, 0x09, 0x9D, 0x61, 0x1B, 0x98, 0x0C, 0x93, 0x64, 0x93, 0x9D, 0x61, 0x21,
		0x4B, 0x20, 0x06, 0x20, 0x46, 0x02, 0x12, 0x02, 0x52, 0x72, 0x42, 0x72, 0x2C, 0xB2, 0x08, 0xB0,
		0x48, 0x02, 0x26, 0x70, 0xF0, 0x70, 0xE0, 0x96, 0x12, 0x26, 0x18, 0x52, 0x86, 0xA6, 0xC6, 0xE6,
		0x8A, 0x62, 0xE4, 0x68, 0x60, 0x32, 0x32, 0x32, 0x30, 0x82, 0x88, 0xE4, 0x06, 0x02, 0x02, 0x60,
		0x86, 0xD8, 0xD8, 0xE4, 0xE4, 0x30, 0x30, 0x46, 0x86, 0x00, 0x30, 0x25, 0x19, 0x24, 0x28, 0x34,
		0x28, 0x28, 0x21, 0x28, 0x28, 0x23, 0x19, 0x34, 0x30, 0x21, 0x38, 0x34, 0x36, 0x30, 0x30, 0x38,
		0x34, 0x30, 0x24, 0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x88, 0x98, 0xA8, 0xB8, 0xC8,
		0xD8, 0xE8, 0xF8, 0x8A, 0x9A, 0xAA, 0xBA, 0xCA, 0xEA, 0x00, 0x40, 0x60, 0x10, 0x30, 0x50, 0x70,
		0x90, 0xB0, 0xD0, 0xF0, 0x14, 0x20, 0x40, 0x80, 0xA0, 0xC0, 0xE0, 0x01, 0x21, 0x41, 0x61, 0x81,
		0xA1, 0xC1, 0xE1, 0x02, 0x22, 0x42, 0x62, 0x82, 0xA2, 0xC2, 0xE2, 0x00, 0x08, 0x00, 0x00, 0x04,
		0x14, 0x14, 0x00, 0x10, 0x0C, 0x1C, 0x18, 0x2C, 0x04, 0x20, 0x54, 0x30, 0x0D, 0x80, 0x04, 0x90,
		0x03, 0x22, 0x54, 0x33, 0x0D, 0x80, 0x04, 0x90, 0x04, 0x20, 0x54, 0x33, 0x0D, 0x80, 0x04, 0x90,
		0x04, 0x20, 0x54, 0x3B, 0x0D, 0x80, 0x04, 0x90, 0x00, 0x22, 0x44, 0x33, 0x0D, 0xC8, 0x44, 0x00,
		0x11, 0x22, 0x44, 0x33, 0x0D, 0xC8, 0x44, 0xA9, 0x01, 0x22, 0x44, 0x33, 0x0D, 0x80, 0x04, 0x90,
		0x01, 0x22, 0x44, 0x33, 0x0D, 0x80, 0x04, 0x90, 0x26, 0x31, 0x87, 0x9A, 0x00, 0x21, 0x81, 0x82,
		0x00, 0x00, 0x59, 0x4D, 0x91, 0x92, 0x86, 0x4A, 0x85, 0x9D, 0x2C, 0x29, 0x2C, 0x23, 0x28, 0x24,
		0x59, 0x00, 0x58, 0x24, 0x24, 0x00, 0x22, 0x24, 0x25, 0x35, 0x36, 0x37, 0x04, 0x05, 0x05, 0x02,
		0x05, 0x05, 0x04, 0x05, 0x0A, 0x0B, 0x0A, 0x0A, 0x4E, 0x4C, 0x58, 0x45, 0x4D, 0x52, 0x44, 0x49,
		0x21, 0x24, 0x41, 0x56, 0x50, 0xF0, 0xF0, 0xF1, 0xF1, 0xF1, 0xF0, 0xFA, 0xF1, 0xFF, 0xFF, 0xF4,
		0xF0, 0xF0, 0xDE, 0xE4, 0xC6, 0x4E, 0x04, 0x9F, 0xC1, 0x57, 0x1A, 0x0E, 0x4A, 0x90, 0x84, 0x42,
		0x57, 0x53, 0x3D, 0x4D, 0x20, 0x3A, 0x52, 0x52, 0x45, 0x4D, 0x4E, 0x45, 0x41, 0x44, 0x44, 0x53,
		0x59, 0x4E, 0x4F, 0x56, 0x46, 0x53, 0x59, 0x4D, 0x4E, 0x45, 0x53, 0x53, 0x45, 0x57, 0x20, 0x4E,
		0x45, 0x4B, 0x20, 0x59, 0x42, 0x20, 0x33, 0x2E, 0x31, 0x20, 0x52, 0x45, 0x44, 0x41, 0x53, 0x55,
		0x52, 0x4B, 0x0D, 0x50, 0x53, 0x59, 0x58, 0x41, 0x4C, 0x48, 0x43, 0x5A, 0x49, 0x44, 0x42, 0x00,
		0x56, 0x4E, 0x20, 0xE5, 0xFE, 0x20, 0xB9, 0xFE, 0x20, 0xBE, 0xFE, 0xA0, 0x07, 0xD9, 0x82, 0xFD,
		0xF0, 0x5C, 0x88, 0xD0, 0xF8, 0xC9, 0x52, 0xD0, 0x06, 0x20, 0x9F, 0xFE, 0x6C, 0xF5, 0x00, 0xC9,
		0x54, 0xD0, 0x3A, 0xA2, 0x08, 0xBD, 0x96, 0xFE, 0x95, 0xE0, 0xCA, 0xD0, 0xF8, 0xA1, 0xF5, 0xF0,
		0x5D, 0xA4, 0x2A, 0xC9, 0x20, 0xF0, 0x75, 0xC9, 0x60, 0xF0, 0x63, 0xC9, 0x4C, 0xF0, 0x78, 0xC9,
		0x6C, 0xF0, 0x75, 0xC9, 0x40, 0xF0, 0x53, 0x29, 0x1F, 0x49, 0x14, 0xC9, 0x04, 0xF0, 0x02, 0xB1,
		0xF5, 0x99, 0xE0, 0x00, 0x88, 0x10, 0xF8, 0x20, 0x9F, 0xFE, 0x4C, 0xE0, 0x00, 0xC9, 0x21, 0xD0,
		0x06, 0x20, 0x1B, 0xFF, 0x4C, 0x1E, 0xFE, 0xC9, 0x24, 0xD0, 0x97, 0x4C, 0x0F, 0xFF, 0xA2, 0xFE,
		0x20, 0xBE, 0xFE, 0x95, 0x11, 0xE8, 0xD0, 0xF8, 0x20, 0x51, 0xFA, 0x99, 0xEF, 0x00, 0xA6, 0xF1,
		0x9A, 0x4C, 0x1E, 0xFE, 0x28, 0x20, 0xAA, 0xFE, 0x68, 0x85, 0xF5, 0x68, 0x85, 0xF6, 0xBA, 0x86,
		0xF1, 0x20, 0x57, 0xFE, 0x20, 0xDC, 0xFA, 0x4C, 0x92, 0xFD, 0x18, 0x68, 0x85, 0xF0, 0x68, 0x85,
		0xF5, 0x68, 0x85, 0xF6, 0x20, 0x82, 0xFB, 0x84, 0xF6, 0x18, 0x90, 0x14, 0x18, 0x20, 0x82, 0xFB,
		0xAA, 0x98, 0x48, 0x8A, 0x48, 0xA0, 0x02, 0x18, 0xB1, 0xF5, 0xAA, 0x88, 0xB1, 0xF5, 0x86, 0xF6,
		0x85, 0xF5, 0xB0, 0xF3, 0x4C, 0x1E, 0xFE, 0x20, 0xE5, 0xFE, 0xA2, 0x05, 0xBD, 0x82, 0xFD, 0x20,
		0xEF, 0xFF, 0x20, 0xB9, 0xFE, 0xB5, 0xEF, 0x20, 0xDC, 0xFF, 0x20, 0xE0, 0xFE, 0xCA, 0xD0, 0xEC,
		0xA5, 0xF0, 0xA2, 0x08, 0x0A, 0x90, 0x08, 0x48, 0xBD, 0x89, 0xFD, 0x20, 0xEF, 0xFF, 0x68, 0xCA,
		0xD0, 0xF2, 0x60, 0x18, 0xA0, 0x01, 0xB1, 0xF5, 0x20, 0x84, 0xFB, 0x85, 0xF5, 0x98, 0x38, 0xB0,
		0xA1, 0x20, 0xAA, 0xFE, 0x38, 0xB0, 0x9D, 0xEA, 0xEA, 0x4C, 0x91, 0xFE, 0x4C, 0x83, 0xFE, 0xA5,
		0xF0, 0x48, 0xA5, 0xF4, 0xA6, 0xF3, 0xA4, 0xF2, 0x28, 0x60, 0x85, 0xF4, 0x86, 0xF3, 0x84, 0xF2,
		0x08, 0x68, 0x85, 0xF0, 0xBA, 0x86, 0xF1, 0xD8, 0x60, 0xA9, 0x2D, 0x4C, 0xEF, 0xFF, 0x20, 0xEA,
		0xFE, 0x4C, 0xEF, 0xFF, 0x20, 0xE5, 0xFE, 0xA2, 0x00, 0xB5, 0x04, 0x20, 0xEF, 0xFF, 0xE8, 0xE0,
		0x06, 0xD0, 0xF6, 0x20, 0xE0, 0xFE, 0xA5, 0xFB, 0x20, 0xE5, 0xFF, 0xA5, 0xFA, 0x20, 0xDC, 0xFF,
		0xA9, 0x20, 0x4C, 0xEF, 0xFF, 0xA9, 0x0D, 0x4C, 0xEF, 0xFF, 0xAD, 0x11, 0xD0, 0x10, 0xFB, 0xAD,
		0x10, 0xD0, 0x29, 0x7F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	];
	this.length = assembler.length;
	this.read = function(address) {
		return assembler[address - this.start];
	},
	this.write = function(value, address) {
		return true;
	}
	return this;
};

