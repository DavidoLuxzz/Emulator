#include "ngraphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// settings
unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;

void main_loop(void){
    ngColor(0, 0, 0);
    ngClear();
    ngColor(255, 255, 255);
    ngDrawPixel(202, 100);
    ngDrawRectangle(0, 0, 100, 100);
}

int main(){
    ngCreateWindow(WIDTH, HEIGHT, "OPENGL");
    ngLoadOpenGL();
    ngSetupScreen();
    
    ngFlipScreen(false,true);
    
    ngSetMainLoopFunction(main_loop);
    ngMainLoop();

    ngTerminate();
    return 0;
}
