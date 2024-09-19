#include "emu_main.h"
#define NG_INCLUDE_LOAD_UI
#include "nGraphics/ngraphics.h"

#include "programs/prog0.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// settings
unsigned int WIDTH = 1024;
unsigned int HEIGHT = 600;

_Bool emu_redraw = NG_TRUE;
_Bool emu_progActive = NG_FALSE;
int emu_progExitCode;

void emu_input(void){
    if (ngGetKey(GLFW_KEY_ENTER)){
        emu_progActive = NG_TRUE;
        prog0_init();
        ngSetMainLoopFunction(prog0_main);
    } else if (ngGetKey(GLFW_KEY_UP)){
        ngUISelectPrevious();
        emu_redraw = NG_TRUE;
        ngLockKey(GLFW_KEY_UP); // lock key to prevent multiple clicks
    } else if (ngGetKey(GLFW_KEY_DOWN)){
        ngUISelectNext();
        emu_redraw = NG_TRUE;
        ngLockKey(GLFW_KEY_DOWN); // lock key to prevent multiple clicks
    } else if (ngGetKey(GLFW_KEY_ESCAPE)) {
        ngPerformExit();
    }
}

void emu_main(void){
    emu_input();
    if (emu_progActive) return;
    // -- drawin -- //
    if (emu_redraw){
        ngColor(0, 0, 0);
        ngClear();
        ngColor(NG_LAZY_WHITE);
        
        ngUIShow();
        
        emu_redraw = NG_FALSE;
    }
}

void emu_init(void){
    ngFontScale(1);
    ngSetFont(NG_FONT_GRUB);
    ngConfig(NG_DRAW_TEXT_BACKGROUND, NG_FALSE);
}
int main(){
    ngCreateWindow(WIDTH, HEIGHT, "EmuLoader");
    ngLoadOpenGL();
    ngSetupScreen();
    
    ngUISetup(720, 376, 2);
    ngUIAddEntry(0, "Lukin program\0");
    ngUIAddEntry(1, "PAC-MAN\0");
    
    emu_init();
    ngInitTextColoring(2); // max 2 color changes per text
    ngSetMainLoopFunction(emu_main);
    ngMainLoop();
    
    ngUIDestroy();
    ngTerminate();
    return 0;
}
void emu_after_prog(void){
    emu_init();
    ngSetMainLoopFunction(emu_main);
}


// API for external programs
void emuExit(int code){
    emu_progExitCode = code;
    emu_progActive = NG_FALSE;
    emu_redraw = NG_TRUE;
    ngLockKey(GLFW_KEY_ESCAPE); // lock escape key to prevent quitting application accidentaly
    ngSetMainLoopFunction(emu_after_prog);
}
