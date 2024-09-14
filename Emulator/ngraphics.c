//
//  ngraphics.c
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#include "ngraphics.h"

int __ngflags[5];
GLuint __ngShaderProgram;
GLubyte* __ngScreen;
GLuint __ngScreenVBO, __ngScreenVAO, __ngScreenEBO;
GLuint __ngScreenTexture;

unsigned int __ngScreenWidth, __ngScreenHeight;

NGMAINLOOPFUNC __ngUserMainLoop;

void ngTerminate(void){
    free(__ngScreen);
    glDeleteProgram(__ngShaderProgram);
    glDeleteVertexArrays(1, &__ngScreenVAO);
    glDeleteBuffers(1, &__ngScreenVBO);
    glDeleteBuffers(1, &__ngScreenEBO);
    glDeleteTextures(1, &__ngScreenTexture);
    glfwTerminate();
}

void ngSetupScreen(void){
    _ng_create_window_screen_buffers();
    _ng_create_screen_texture();
}

void ngSetMainLoopFunction(NGMAINLOOPFUNC userMainLoopFun){
    __ngUserMainLoop = userMainLoopFun;
    if (userMainLoopFun){
        __ngflags[NG_USER_MAINLOOP_FUNC_ATTACHED] = 1;
    } else {
        __ngflags[NG_USER_MAINLOOP_FUNC_ATTACHED] = 0;
    }
}

void ngMainLoop(void){
    while (!ngWindowShouldClose()){
        // user main loop
        if (__ngflags[NG_USER_MAINLOOP_FUNC_ATTACHED])
            __ngUserMainLoop();
        glBindTexture(GL_TEXTURE_2D, __ngScreenTexture);
        glTexSubImage2D(GL_TEXTURE_2D,0, 0,0, __ngScreenWidth,__ngScreenHeight, GL_RGB, GL_UNSIGNED_BYTE, __ngScreen);
        // clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // bind Texture
        glBindTexture(GL_TEXTURE_2D, __ngScreenTexture);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __ngScreenEBO);
        // enable vertex attributes
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        // draw
        glUseProgram(__ngShaderProgram);
        glBindVertexArray(__ngScreenVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // disable vertex attributes
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        ngSwapBuffers();
        ngPollEvents();
    }
}

GLubyte* ngGetScreenBuffer(void){
    return __ngScreen;
}

_Bool __ngFlipHorizontaly, __ngFlipVerticaly;
void ngFlipScreen(_Bool horizontaly, _Bool verticaly){
    __ngFlipHorizontaly = horizontaly;
    __ngFlipVerticaly = verticaly;
}


// -- DRAWING -- //
GLubyte __ngColor[3];
void ngColor(GLubyte r, GLubyte g, GLubyte b){
    __ngColor[0] = r;
    __ngColor[1] = g;
    __ngColor[2] = b;
}

GLintptr _ngPosition(int x, int y){
    GLintptr xpos, ypos;
    
    if (__ngFlipHorizontaly) xpos = __ngScreenWidth - x;
    else xpos = x;
    if (__ngFlipVerticaly) ypos = __ngScreenWidth*(__ngScreenHeight - y-1);
    else ypos = __ngScreenWidth*y;
    
    return ypos+xpos;
}

void ngClear(void){
    for (int i=0; i<__ngScreenWidth*__ngScreenHeight; i++){
        __ngScreen[3*i]   = __ngColor[0];
        __ngScreen[3*i+1] = __ngColor[1];
        __ngScreen[3*i+2] = __ngColor[2];
    }
}

void ngDrawPixel(int x, int y){
    GLintptr pos = 3*_ngPosition(x, y);
    __ngScreen[pos]   = __ngColor[0];
    __ngScreen[pos+1] = __ngColor[1];
    __ngScreen[pos+2] = __ngColor[2];
}
void ngDrawRectangle(GLuint x, GLuint y, GLuint w, GLuint h){
    for (int xa=0; xa<w; xa++){
        for (int ya=0; ya<h; ya++){
            ngDrawPixel(x+xa, y+ya);
        }
    }
}
