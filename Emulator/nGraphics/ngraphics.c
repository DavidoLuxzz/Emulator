//
//  ngraphics.c
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#include "ngraphics.h"
#include <string.h>

int __ngflags[16];
GLuint __ngShaderProgram;
GLubyte* __ngScreen;
GLuint __ngScreenVBO, __ngScreenVAO, __ngScreenEBO;
GLuint __ngScreenTexture;

unsigned int __ngScreenWidth, __ngScreenHeight;
unsigned int __ngScreenSize;

NGMAINLOOPFUNC __ngUserMainLoop;

_Bool __ngExitRequested = NG_FALSE;
void ngPerformExit(void){
    __ngExitRequested = NG_TRUE;
}

void ngTerminate(void){
    free(__ngScreen);
    ngDestroyFont();
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
NGMAINLOOPFUNC ngGetMainLoopFunction(void){
    return __ngUserMainLoop;
}

unsigned long __ngMainWait = 0;
void ngWait(unsigned long ticks){
    __ngMainWait = ticks;
}
void ngMainLoop(void){
    double _time_ = glfwGetTime();
    while (!ngWindowShouldClose()){
        if (__ngExitRequested) break;
        // user main loop
        if (__ngflags[NG_USER_MAINLOOP_FUNC_ATTACHED]){
            if (!__ngMainWait){
                __ngUserMainLoop();
            } else __ngMainWait--;
        }
        glBindTexture(GL_TEXTURE_2D, __ngScreenTexture);
        glTexSubImage2D(GL_TEXTURE_2D,0, 0,0, __ngScreenWidth,__ngScreenHeight, GL_RGB, GL_UNSIGNED_BYTE, __ngScreen);
        // clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        if (__ngflags[NG_SHOW_FPS]){
            double _dt = glfwGetTime() - _time_;
            _time_ = glfwGetTime();
            char _dttl[64];
            sprintf(_dttl, "EmuLoader | %.1lf", 1.0/_dt);
            ngSetWindowTitle(_dttl);
        }
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

void ngEnableDepthTest(void){
    glEnable(GL_DEPTH_TEST);
}

int ngGet(int flag){
    return __ngflags[flag];
}

void ngConfig(int flag, int val){
    __ngflags[flag] = val;
}


// -- DRAWING -- //
GLubyte __ngColor[3];
void ngColor(GLubyte r, GLubyte g, GLubyte b){
    __ngColor[0] = r;
    __ngColor[1] = g;
    __ngColor[2] = b;
}
GLuint __ngFontScale = 1;
void ngFontScale(GLuint scale){
    __ngFontScale = scale;
}
GLint __ngTextKerning = 0;
void ngTextKerning(GLint kern){
    __ngTextKerning = kern;
}

GLintptr _ngPosition(int x, int y){
    GLintptr xpos, ypos, pos;
    
    if (__ngFlipHorizontaly) xpos = __ngScreenWidth - x;
    else xpos = x;
    if (__ngFlipVerticaly) ypos = __ngScreenWidth*(__ngScreenHeight - y-1);
    else ypos = __ngScreenWidth*y;
    
    pos = (xpos+ypos)%(__ngScreenSize);
    if (pos < 0) return 0;
    return pos;
}

void ngClear(void){
    for (int i=0; i<__ngScreenSize; i++){
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
_Bool _ptInTriangle(dot p, dot p0, dot p1, dot p2){
    int A = ( 1.0f/2.0f * (-p1.y * p2.x + p0.y*(-p1.x + p2.x) + p0.x*(p1.y - p2.y) + p1.x * p2.y) );
    int sign = A<0 ? -1:1;
    int s = (p0.y * p2.x - p0.x * p2.y + (p2.y-p0.y)*p.x + (p0.x-p2.x)*p.y) * sign;
    int t = (p0.x * p1.y - p0.y * p1.x + (p0.y-p1.y)*p.x + (p1.x-p0.x)*p.y) * sign;
    
    return s>0 && t>0 && (s+t)<(2*A*sign);
}
int min(int a, int b) { if (a<b) return a; return b; }
int max(int a, int b) { if (a>b) return a; return b; }

dot minp(dot p1,dot p2,dot p3){
    dot _min;
    _min.x = min(min(p1.x, p2.x), p3.x);
    _min.y = min(min(p1.y, p2.y), p3.y);
    return _min;
}
dot maxp(dot p1,dot p2,dot p3){
    dot _max;
    _max.x = max(max(p1.x, p2.x), p3.x);
    _max.y = max(max(p1.y, p2.y), p3.y);
    return _max;
}
void _ngDrawTriangle(dot p1, dot p2, dot p3){
    dot bounds[2] = {minp(p1, p2, p3), maxp(p1, p2, p3)};
    int _x0 = bounds[0].x;
    int _y0 = bounds[0].y;
    int _rw = bounds[1].x-bounds[0].x;
    int _rh = bounds[1].y-bounds[0].y;
    dot p;
    for (int y=0; y<_rh; y++){
        for (int x=0; x<_rw; x++){
            p.x = _x0+x;
            p.y = _y0+y;
            if (_ptInTriangle(p, p1, p2, p3)){
                ngDrawPixel(p.x, p.y);
            }
        }
    }
}
void ngDrawTriangle(GLuint x1,GLuint y1,GLuint x2,GLuint y2,GLuint x3,GLuint y3){
    dot p1 = {x1,y1};
    dot p2 = {x2,y2};
    dot p3 = {x3,y3};
    _ngDrawTriangle(p1, p2, p3);
}
