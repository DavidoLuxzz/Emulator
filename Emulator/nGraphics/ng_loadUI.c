//
//  ng_loadUI.c
//  Emulator
//
//  Created by Luka on 17.9.24..
//

#include "ng_loadUI.h"
#include "ngraphics.h"
#include <string.h>

unsigned int __ngUIx, __ngUIy;
unsigned int __ngUIwidth, __ngUIheight;
unsigned int __ngUImaxPrograms;
unsigned int __ngUIloadedPrograms;
ng_lde* __ngUIentries;
unsigned int __ngUIselectedEntry;

void ngUISetup(unsigned int w, unsigned int h, unsigned int maxPrograms){
    __ngUIwidth = w;
    __ngUIheight = h;
    __ngUImaxPrograms = maxPrograms;
    __ngUIloadedPrograms = 0;
    __ngUIselectedEntry  = 0;
    if (maxPrograms > 0) {
        __ngflags[NG_UI_LOADED] = 1;
        __ngUIentries = (ng_lde*) malloc(maxPrograms * sizeof(ng_lde));
    }
    __ngUIx = (__ngScreenWidth-w)/2;
    __ngUIy = (__ngScreenHeight-h)/2;
}
void ngUIShowEntryText(ng_lde* e){
    if (e->id == __ngUIselectedEntry){
        GLubyte r = __ngColor[0];
        GLubyte g = __ngColor[1];
        GLubyte b = __ngColor[2];
        ngDrawRectangle(__ngUIx+4, __ngUIy+4+(e->id*(__ngFontScale*NG_FONT_HEIGHT)),  __ngUIwidth-8, __ngFontScale*NG_FONT_HEIGHT);
        ngColor(10, 10, 10);
        ngDrawText(__ngUIx+6, __ngUIy+4+(e->id*(__ngFontScale*NG_FONT_HEIGHT)), e->name, strlen(e->name));
        ngColor(r,g,b);
    } else
        ngDrawText(__ngUIx+6, __ngUIy+4+(e->id*(__ngFontScale+NG_FONT_HEIGHT)), e->name, strlen(e->name));
}
void ngUIShow(void){
    ngDrawText(__ngUIx+20, 40, "EmuLoader v1.0.3 for Assembly", 29);
    ngDrawText(__ngUIx+10, __ngUIy+__ngUIheight+40, "[F1] Quit    [ENTER] Select", 27);
    ngDrawText(5, __ngScreenHeight-20, "v1.0.3", 6);
    // -- BORDERS -- //
    // up-down
    ngDrawRectangle(__ngUIx, __ngUIy, __ngUIwidth, 2);
    ngDrawRectangle(__ngUIx, __ngUIy+__ngUIheight, __ngUIwidth, 2);
    // left-right
    ngDrawRectangle(__ngUIx, __ngUIy, 2, __ngUIheight);
    ngDrawRectangle(__ngUIx+__ngUIwidth, __ngUIy, 2, __ngUIheight);
    
    for (int i=0; i<__ngUIloadedPrograms; i++){
        ngUIShowEntryText(__ngUIentries+i);
    }
}

void ngUISelectNext(void){
    __ngUIselectedEntry = (__ngUIselectedEntry+1)%__ngUIloadedPrograms;
}
void ngUISelectPrevious(void){
    __ngUIselectedEntry = (__ngUIselectedEntry-1)%__ngUIloadedPrograms;
}

void ngUIAddEntry(int id, const char* sname){
    ng_lde _s = {id, sname};
    *(__ngUIentries+id) = _s;
    __ngUIloadedPrograms = max(id, __ngUIloadedPrograms)+1;
}


void ngUIDestroy(void){
    free(__ngUIentries);
}
