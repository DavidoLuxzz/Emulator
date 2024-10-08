#include "emu_main.h"
#define NG_INCLUDE_LOAD_UI
#include "nGraphics/ngraphics.h"

#include "programs/prog.h"
NGMAINLOOPFUNC EMU_ALL_PROG[] = {prog0_init,prog0_main, prog1_init,prog1_main, prog2_init,prog2_main, prog3_init,prog3_main};

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// settings
const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 600;

_Bool emu_redraw = NG_TRUE;
_Bool emu_cfg_redraw = NG_TRUE;
int emu_cfg_selected = 0;
_Bool emu_progActive = NG_FALSE;
int emu_progExitCode;

void emu_after_prog(void);
void emu_cfg_input(void){
    if (ngGetKey(GLFW_KEY_UP)){
        emu_cfg_selected-=1;
        emu_cfg_redraw = NG_TRUE;
        ngLockKey(GLFW_KEY_UP); // lock key to prevent multiple clicks
    } else if (ngGetKey(GLFW_KEY_DOWN)){
        emu_cfg_selected+=1;
        emu_cfg_redraw = NG_TRUE;
        ngLockKey(GLFW_KEY_DOWN); // lock key to prevent multiple clicks
    }
    
    if (ngGetKey(GLFW_KEY_RIGHT)){
        ngIncProperty(emu_cfg_selected);
        emu_cfg_redraw = NG_TRUE;
        ngLockKey(GLFW_KEY_RIGHT);
    } else if (ngGetKey(GLFW_KEY_LEFT)){
        ngDecProperty(emu_cfg_selected);
        emu_cfg_redraw = NG_TRUE;
        ngLockKey(GLFW_KEY_LEFT);
    }
    
    if (ngGetKey(GLFW_KEY_ESCAPE)) {
        emu_redraw = NG_TRUE;
        ngLockKey(GLFW_KEY_ESCAPE);
        ngSetMainLoopFunction(emu_after_prog);
    }
}

void emu_config(void){
    emu_cfg_input();
    if (emu_cfg_redraw){
        ngColor(0, 0, 0);
        ngClear();
        ngColor(NG_LAZY_WHITE);
        
        ngDrawText(2, 2, "Configuration Menu", 18);
        ngDrawChar(2, 30+emu_cfg_selected*__ngFontScale*NG_FONT_HEIGHT, NG_FONT1_RIGHT_ARW);
        
        ngDrawText(4+__ngFontScale*NG_FONT_WIDTH, 30, "Pixel scale:", 12);
        unsigned char _psChar = __ngPixelScale;
        if (_psChar < 10) _psChar += '0';
        else _psChar += 'A'-0xA;
        ngDrawChar(4+__ngFontScale*NG_FONT_WIDTH*14, 30, _psChar);
        
        emu_cfg_redraw = 0;
    }
}

void emu_input(void){
    if (ngGetKey(GLFW_KEY_ENTER) || ngGetKey(GLFW_KEY_SPACE)){
        emu_progActive = NG_TRUE;
        EMU_ALL_PROG[2*ngUIGetSelectedEntry()]();
        ngSetMainLoopFunction(EMU_ALL_PROG[2*ngUIGetSelectedEntry() + 1]);
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
    } else if (ngGetKey(GLFW_KEY_F1)){
        emu_cfg_redraw = NG_TRUE;
        ngSetMainLoopFunction(emu_config);
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
//        ngUIShowError();
        
        emu_redraw = NG_FALSE;
    }
}

void emu_init(void){
    ngFontScale(1);
    ngSetFont(NG_FONT_GRUB);
    ngConfig(NG_DRAW_TEXT_BACKGROUND, NG_FALSE);
    ngUnlockKey(GLFW_KEY_UP);
    ngUnlockKey(GLFW_KEY_DOWN);
}
int main(){
    ngCreateWindow(WIDTH, HEIGHT, "EmuLoader");
    ngLoadOpenGL();
    ngSetupScreen();
    
    // Experimental
    ngConfig(NG_ALL_GRAY, NG_TRUE);
    
//    ngConfig(NG_SHOW_FPS, NG_TRUE);
    ngConfig(NG_PRINT_LOG, NG_TRUE);
    ngConfig(NG_DEFAULT_TRIANGLE_ALGORITHM, NG_TRI_ALGO_SOFN);
    
    ngUISetup(720, 376, 4);
    ngUIAddEntry("PONG (C)\0");
    ngUIAddEntry("PONG (asm)\0");
    ngUIAddEntry("Test program\0");
    ngUIAddEntry("3D test program\0");
    
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

// NASM API functions for external assembly programs
#include <unistd.h>
void asmEmuExit(int code){
    emuExit(code);
}

int asmEmuLog(int n){
    printf("%d\n", n);
    return n;
}
