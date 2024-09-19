//
//  ng_font.h
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#ifndef ng_font_h
#define ng_font_h

#define NG_FONT_PACMAN  0x0
#define NG_FONT_GRUB    0x1

#define NG_FONT0_FORMAT 0x1
#define NG_FONT0_XLEN 0x10
#define NG_FONT0_YLEN 0x4
#define NG_FONT0_WIDTH 0x8
#define NG_FONT0_HEIGHT 0x8
#define NG_FONT0_MAP_WIDTH 128
#define NG_FONT0_MAP_HEIGHT 32
#define NG_FONT0_MAP_WIDTHm1 120 // last tile in row is not used, so the font map width is actually 120 instead of 128

extern const int __NG_FONT0_DATA[];

#define NG_FONT1_XLEN 0x20
#define NG_FONT1_YLEN 0x8
#define NG_FONT1_WIDTH 0x9
#define NG_FONT1_HEIGHT 0x10
#define NG_FONT1_MAP_WIDTH 288
#define NG_FONT1_MAP_HEIGHT 128

// escape sequence characters
#define NG_FONT1_SMILEY0    1
#define NG_FONT1_SMILEY1    2
#define NG_FONT1_HEART      3
#define NG_FONT1_DIAMOND    4
#define NG_FONT1_TREF       5
#define NG_FONT1_PIC        6
#define NG_FONT1_DOT0       7
#define NG_FONT1_DOT1       8
#define NG_FONT1_RING0      9
#define NG_FONT1_RING1      10
#define NG_FONT1_GEN_MEN    11
#define NG_FONT1_GEN_WMN    12
#define NG_FONT1_NOTE_SINGL 13
#define NG_FONT1_NOTE_DOUBL 14
#define NG_FONT1_UNDEFINED0 15
#define NG_FONT1_UNDEFINED1 16
#define NG_FONT1_UNDEFINED2 17
#define NG_FONT1_UPDOWN_AR0 18
#define NG_FONT1_BRILLIANT  19
#define NG_FONT1_UNDEFINED3 20
#define NG_FONT1_WIGLEWIGLE 21
#define NG_FONT1_DOWN3RDBLK 22
#define NG_FONT1_UPDOWN_AR1 23
#define NG_FONT1_UP_ARW     24
#define NG_FONT1_DOWN_ARW   25
#define NG_FONT1_RIGHT_ARW  26
#define NG_FONT1_LEFT_ARW   27
#define NG_FONT1_UNDEFINED4 28
#define NG_FONT1_LR_ARW     29
#define NG_FONT1_TANGL_UP   30
#define NG_FONT1_TANGL_DOWN 31

extern const int __NG_FONT1_DATA[];

extern int NG_FONT_WIDTH;
extern int NG_FONT_HEIGHT;
extern int NG_FONT_XLEN;
extern int NG_FONT_YLEN;
extern int NG_FONT_CHARS;
extern int NG_FONT_MAP_WIDTH;
extern const int *__NG_ALL_FONT_DATA[];
extern unsigned int __NG_FONT;

extern const unsigned char __NG_FONT0[];
extern const unsigned char __NG_FONT1[];
extern const unsigned char *__NG_FONTS[];

extern unsigned int __ngMaxTextColoring;

#define NG_TEXT_RESET   0x0
#define NG_TEXT_RED     0x1
#define NG_TEXT_GREEN   0x2
#define NG_TEXT_BLUE    0x3

extern unsigned char __NG_TEXT_COLORS[];

void ngSetFont(unsigned int _font);
void ngInitTextColoring(unsigned int);
void ngAddTColor(int pos, int color);
int ngGetCurrentTColor(int cpos);
void ngResetTextColoring(void);

void ngDrawChar(int xpos, int ypos, char chr);
void ngDrawText(int xpos, int ypos, const char* str, unsigned long len);

void ngDestroyFont(void);

#endif /* ng_font_h */
