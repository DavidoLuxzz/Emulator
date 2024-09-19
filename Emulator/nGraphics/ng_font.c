//
//  ng_font.c
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#include "ng_font.h"
#include "ngraphics.h"

const int __NG_FONT0_DATA[] = {NG_FONT0_XLEN,NG_FONT0_YLEN,NG_FONT0_WIDTH,NG_FONT0_HEIGHT,NG_FONT0_MAP_WIDTH,NG_FONT0_MAP_HEIGHT};
const int __NG_FONT1_DATA[] = {NG_FONT1_XLEN,NG_FONT1_YLEN,NG_FONT1_WIDTH,NG_FONT1_HEIGHT,NG_FONT1_MAP_WIDTH,NG_FONT1_MAP_HEIGHT};

const int *__NG_ALL_FONT_DATA[] = {__NG_FONT0_DATA, __NG_FONT1_DATA};

int NG_FONT_WIDTH = NG_FONT0_WIDTH;
int NG_FONT_HEIGHT = NG_FONT0_HEIGHT;
int NG_FONT_XLEN = NG_FONT0_XLEN;
int NG_FONT_YLEN = NG_FONT0_YLEN;
int NG_FONT_CHARS = NG_FONT0_YLEN * NG_FONT0_YLEN;
int NG_FONT_MAP_WIDTH = NG_FONT0_MAP_WIDTH;
int NG_FONT_MAP_HEIGHT = NG_FONT0_MAP_HEIGHT;
const unsigned char *__NG_FONTS[] = {__NG_FONT0, __NG_FONT1};
unsigned int __NG_FONT = 0;
// from PACDAT.c (Pac-Man)
const char FONT0_ARRAY[] = "abcdefghijklmno:pqrstuvwxyz!$PTS0123456789/-\"   NAXMCOQ         ";
const char FONT1_ARRAY[] = "\0\1\2\3\4\5\6\7\10\11\12\13\14\15\16\17\20\21\22\23\24\25\26\27\30\31\32\33\34\35\36\37 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~?•???∆?????????????∑?????Ωß?????????????????????????????????????????????????????????????????????????????????????????????????";
const char* FONT_ARRAYS[] = {FONT0_ARRAY, FONT1_ARRAY};
int font_pos(char c){
    int i, _found=0;
    for (i=0; i<NG_FONT_CHARS; i++){
        if (FONT_ARRAYS[__NG_FONT][i] == c) {
            //printf("Found char: %c nfc: %d\n", c, NG_FONT_CHARS);
            _found = 1;
            break;
        }
    }
    if (_found) return i;
    return -1;
}

void ngDrawChar(int xpos, int ypos, char chr){
    int cpos = font_pos(chr) * NG_FONT_WIDTH;
    if (cpos < 0) return;
    int cxpos = cpos % NG_FONT_MAP_WIDTH;
    int cypos = cpos/(NG_FONT_MAP_WIDTH) * NG_FONT_HEIGHT;
    GLubyte r = __ngColor[0];
    GLubyte g = __ngColor[1];
    GLubyte b = __ngColor[2];
    int flag0 = ngGet(NG_DRAW_TEXT_BACKGROUND);
    for (int y=0; y<NG_FONT_HEIGHT; y++){
        for (int x=0; x<NG_FONT_WIDTH; x++){
            unsigned char mul = __NG_FONTS[__NG_FONT][(cypos+y)*NG_FONT_MAP_WIDTH + cxpos+x];
            if (!(flag0 + mul)) continue;
            ngColor(r*mul, g*mul, b*mul);
            ngDrawRectangle(xpos+(__ngFontScale*x), ypos+(__ngFontScale*y), __ngFontScale, __ngFontScale);
        }
    }
    ngColor(r, g, b);
}

void ngDrawText(int xpos, int ypos, const char* str, unsigned long len){
    for (int i=0; i<len; i++) {
        if (ngGet(NG_TEXT_COLORING_DECLARED)){
            int _i = ngGetCurrentTColor(i);
            if (_i >= 0)
                ngColor(__NG_TEXT_COLORS[3*_i], __NG_TEXT_COLORS[3*_i+1], __NG_TEXT_COLORS[3*_i+2]);
        }
        ngDrawChar(xpos + __ngFontScale*NG_FONT_WIDTH*i + __ngTextKerning*i, ypos, str[i]);
    }
}

void ngSetFont(unsigned int _font){
    __NG_FONT = _font;
    NG_FONT_XLEN = *(__NG_ALL_FONT_DATA[_font]);
    NG_FONT_YLEN = *(__NG_ALL_FONT_DATA[_font] + 1);
    NG_FONT_WIDTH = *(__NG_ALL_FONT_DATA[_font] + 2);
    NG_FONT_HEIGHT = *(__NG_ALL_FONT_DATA[_font] + 3);
    NG_FONT_MAP_WIDTH = *(__NG_ALL_FONT_DATA[_font] + 4);
    NG_FONT_MAP_HEIGHT = *(__NG_ALL_FONT_DATA[_font] + 5);

    NG_FONT_CHARS = NG_FONT_XLEN * NG_FONT_YLEN;
}

unsigned char __NG_TEXT_COLORS[] = {255,255,255, 255,0,0, 0,255,0, 0,0,255};
typedef int NG_TCOLOR[2];
NG_TCOLOR *__ngTextColorChanges;
unsigned int __ngMaxTextColoring;
unsigned int __ngNumCurrentTextColors;

void ngInitTextColoring(unsigned int max){
    __ngMaxTextColoring = max;
    __ngNumCurrentTextColors = 0;
    __ngTextColorChanges = (NG_TCOLOR*) malloc(max * sizeof(NG_TCOLOR));
    __ngflags[NG_TEXT_COLORING_DECLARED] = 1;
}
int ngGetCurrentTColor(int CPIT){ // current position in text
    for (int i=0; i<__ngNumCurrentTextColors; i++){
        if ((*(__ngTextColorChanges+i))[0] == CPIT)
            return (*(__ngTextColorChanges+i))[1];
    }
    return -1;
}
void ngAddTColor(int pos, int color){
    if (__ngNumCurrentTextColors >= __ngMaxTextColoring) return;
    (*(__ngTextColorChanges+__ngNumCurrentTextColors))[0] = pos;
    (*(__ngTextColorChanges+__ngNumCurrentTextColors))[1] = color;
    __ngNumCurrentTextColors++;
}
void ngResetTextColoring(void){
    __ngNumCurrentTextColors = 0;
}


void ngDestroyFont(void){
    if (ngGet(NG_TEXT_COLORING_DECLARED)) free(__ngTextColorChanges);
}
