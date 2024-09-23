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
    int counter = 0;
    double _sum_time_ = 0.0;
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
            
            _sum_time_ += _dt;
            counter++;
            if (counter>=60){
                double tavg = _sum_time_ / (double)counter;
                _sum_time_ = 0.0;
                counter = 0;
                printf("%ms: %lf\n", 1.0/tavg);
            }
            
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
    if (!__ngflags[NG_ALL_GRAY]){
        __ngColor[0] = r;
        __ngColor[1] = g;
        __ngColor[2] = b;
    } else {
        GLubyte avg = (r+g+b)/3;
        memset(__ngColor, avg, 3);
    }
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
    if (!__ngflags[NG_VIEW_OUT_SCREEN]){
        if (x<0 || x>__ngScreenWidth-1) return 0;
        if (y<0 || y>__ngScreenHeight-1) return 0;
    }
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
    if (!__ngflags[NG_ALL_GRAY]){
        for (int i=0; i<__ngScreenSize; i++){
            __ngScreen[3*i]   = __ngColor[0];
            __ngScreen[3*i+1] = __ngColor[1];
            __ngScreen[3*i+2] = __ngColor[2];
        }
    } else {
        memset(__ngScreen, 0, 3*__ngScreenSize);
    }
}

void ngDrawPixel(int x, int y){
    GLintptr pos = 3*_ngPosition(x, y);
    if (!__ngflags[NG_ALL_GRAY]){
        __ngScreen[pos]   = __ngColor[0];
        __ngScreen[pos+1] = __ngColor[1];
        __ngScreen[pos+2] = __ngColor[2];
    } else {
        memset(__ngScreen+pos, __ngColor[0], 3);
    }
}
void ngDrawRectangle(GLuint x, GLuint y, GLuint w, GLuint h){
    for (int xa=0; xa<w; xa++){
        for (int ya=0; ya<h; ya++){
            ngDrawPixel(x+xa, y+ya);
        }
    }
}
_Bool _ptInTriangle(NG_DOT p, NG_DOT p0, NG_DOT p1, NG_DOT p2){
    int A = ( 1.0f/2.0f * (-p1.y * p2.x + p0.y*(-p1.x + p2.x) + p0.x*(p1.y - p2.y) + p1.x * p2.y) );
    int sign = A<0 ? -1:1;
    int s = (p0.y * p2.x - p0.x * p2.y + (p2.y-p0.y)*p.x + (p0.x-p2.x)*p.y) * sign;
    int t = (p0.x * p1.y - p0.y * p1.x + (p0.y-p1.y)*p.x + (p1.x-p0.x)*p.y) * sign;
    
    return s>0 && t>0 && (s+t)<(2*A*sign);
}
int min(int a, int b) { if (a<b) return a; return b; }
int max(int a, int b) { if (a>b) return a; return b; }
uint umin(uint a, uint b) { if (a<b) return a; return b; }
uint umax(uint a, uint b) { if (a>b) return a; return b; }

NG_DOT minp(NG_DOT p1,NG_DOT p2,NG_DOT p3){
    NG_DOT _min;
    _min.x = min(min(p1.x, p2.x), p3.x);
    _min.y = min(min(p1.y, p2.y), p3.y);
    return _min;
}
NG_DOT maxp(NG_DOT p1,NG_DOT p2,NG_DOT p3){
    NG_DOT _max;
    _max.x = max(max(p1.x, p2.x), p3.x);
    _max.y = max(max(p1.y, p2.y), p3.y);
    return _max;
}
/**
 * Crta trougao odredjen sa tri tacke
 * Sto veci trougao to sporije crta, crtanje optimizovano koliko moze. Racunanje pravi problem...
 */
void _ngDrawTriangle(NG_DOT p1, NG_DOT p2, NG_DOT p3){
    NG_DOT bounds[2] = {minp(p1, p2, p3), maxp(p1, p2, p3)};
    NG_DOT p;
    for (int y=max(bounds[0].y,0); y<=min(bounds[1].y, __ngScreenHeight); y++){
        int minx=-1, xw=0;
        for (int x=max(bounds[0].x,0); x<=min(bounds[1].x, __ngScreenWidth); x++){
            p.x = x;
            p.y = y;
            if (_ptInTriangle(p, p1, p2, p3)){
                if (!__ngflags[NG_ALL_GRAY])
                    ngDrawPixel(p.x, p.y);
                else {
                    if (minx<0) minx = x;
                    xw++;
                }
            }
        }
        if (__ngflags[NG_ALL_GRAY] && xw) {
            // optimizacija
            long lpos = 3L*_ngPosition(minx, y);
            memset(__ngScreen+lpos, __ngColor[0], 3*xw-3); // memset pravi probleme
        }
    }
}
void ngDrawTriangle(GLint x1,GLint y1,GLint x2,GLint y2,GLint x3,GLint y3){
    NG_DOT p1 = {x1,y1};
    NG_DOT p2 = {x2,y2};
    NG_DOT p3 = {x3,y3};
    _ngDrawTriangle(p1, p2, p3);
}

void ngDrawQuad2D(NG_POINT points[4], int type){
    if (type == NG_TRIANGLE_FAN){
        _ngDrawTriangle(points[0], points[1], points[2]);
        _ngDrawTriangle(points[0], points[2], points[3]);
    } else if (type == NG_TRIANGLE_Z){
        _ngDrawTriangle(points[0], points[1], points[3]);
        _ngDrawTriangle(points[0], points[3], points[2]);
    }
}
// x bigger
void _ngDrawLineXb(int x1, int y1, int x2, int y2){
    int m_new = 2 * abs(y2 - y1);
    int slope_error_new = m_new - abs(x2 - x1);
    int w = x2-x1;
    int aw = abs(w);
    int inc, incy;
    if (w<0) inc = -1;
    else inc = 1;
    if (y2-y1 < 0) incy=-1;
    else incy = 1;
    int x = x1, y = y1;
    for (int i=0; i <= aw; i++) {
        ngDrawPixel(x, y);
  
        // Add slope to increment angle formed
        slope_error_new += m_new;
  
        // Slope error reached limit, time to
        // increment y and update slope error.
        if (slope_error_new >= 0) {
            y+=incy;
            slope_error_new -= 2 * abs(x2 - x1);
        }
        x+=inc;
    }
}
// y bigger
void _ngDrawLineYb(int x1, int y1, int x2, int y2){
    int m_new = 2 * abs(x2 - x1);
    int slope_error_new = m_new - abs(y2 - y1);
    int h = y2-y1;
    int ah = abs(h);
    int inc, incx;
    if (h<0) inc = -1;
    else inc = 1;
    if (x2-x1 < 0) incx=-1;
    else incx = 1;
    int x = x1, y = y1;
    for (int i=0; i <= ah; i++) {
        ngDrawPixel(x, y);
  
        // Add slope to increment angle formed
        slope_error_new += m_new;
  
        // Slope error reached limit, time to
        // increment y and update slope error.
        if (slope_error_new >= 0) {
            x+=incx;
            slope_error_new -= 2 * abs(y2 - y1);
        }
        y+=inc;
    }
}
void ngDrawLine(GLint x1, GLint y1, GLint x2, GLint y2){
//    printf("%d %d %d %d\n",x1,y1,x2,y2);
    GLint w = abs(x2-x1);
    GLint h = abs(y2-y1);
    if (w>h) _ngDrawLineXb(x1, y1, x2, y2);
    else _ngDrawLineYb(x1, y1, x2, y2);
}

void ngDrawLines(GLuint nlines, GLint points[], _Bool closeShape){
    if (nlines < 1) return;
    GLuint i;
    for (i=0; i<nlines; i++){
        ngDrawLine(points[2*i], points[2*i+1], points[2*i+2], points[2*i+3]);
    }
    if (closeShape) ngDrawLine(points[2*i], points[2*i+1], points[0], points[1]);
}

void ngDrawSprite_GRAY(GLuint x, GLuint y, GLubyte* sprite, unsigned int sinfo[5]){
    int w = sinfo[0];
    int h = sinfo[1];
    int mul = sinfo[2];
    int pxsz = sinfo[3];
    int transparent = sinfo[4];
    GLubyte r = __ngColor[0];
    GLubyte g = __ngColor[1];
    GLubyte b = __ngColor[2];
    for (int j=0; j<h; j++){
        for (int i=0; i<w; i++){
            GLubyte _col = sprite[j*w + i] * mul;
            if (_col<10 && transparent) continue;
            ngColor(_col, _col, _col);
            ngDrawRectangle(x+pxsz*i, y+pxsz*j, pxsz, pxsz);
        }
    }
    ngColor(r,g,b);
}

// ---- Slede funkcije za crtanje trougla ---- //
// -- stari kôd je bio prespor

int maxyp(NG_POINT p[3], int del){
    int i0, i1;
    if (!del){
        i0 = 1;
        i1 = 2;
    } else {
        i0 = 0;
        i1 = 3-del;
    }
    if (p[i1].x > p[i0].y) return i0;
    return i1;
}
int minyp(NG_POINT p[3]){
    if (p[0].y < p[1].y){
        if (p[0].y < p[2].y) return 0;
        return 2;
    }
    if (p[1].y < p[2].y) return 1;
    return 2;
}
int maxxp2(NG_POINT p[3], int del){
    int i0, i1;
    if (!del){
        i0 = 1;
        i1 = 2;
    } else {
        i0 = 0;
        i1 = 3-del;
    }
    if (p[i1].x < p[i0].x) return i0;
    if (p[i1].x == p[i0].x)
        if (p[i0].y < p[i1].y) return i0;
    return i1;
}
void _lukaTriangCalcMinxs(NG_POINT p1, NG_POINT p2, int duz[]){
    duz[0] = p1.x;
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;
    GLint w = abs(x2-x1);
    GLint h = abs(y2-y1);
    if (w>h) {
        int m_new = 2 * abs(y2 - y1);
        int slope_error_new = m_new - abs(x2 - x1);
        int w = x2-x1;
        int aw = abs(w);
        int inc, incy;
        if (w<0) inc = -1;
        else inc = 1;
        if (y2-y1 < 0) incy=-1;
        else incy = 1;
        int x = x1, y = y1;
        for (int i=0; i <= aw; i++) {
            // Add slope to increment angle formed
            slope_error_new += m_new;
            
            // Slope error reached limit, time to
            // increment y and update slope error.
            if (slope_error_new >= 0) {
                y+=incy;
                duz[abs(y-y1)] = x;
                slope_error_new -= 2 * abs(x2 - x1);
            }
            x+=inc;
        }
    } else {
        int m_new = 2 * abs(x2 - x1);
        int slope_error_new = m_new - abs(y2 - y1);
        int h = y2-y1;
        int ah = abs(h);
        int inc, incx;
        if (h<0) inc = -1;
        else inc = 1;
        if (x2-x1 < 0) incx=-1;
        else incx = 1;
        int x = x1, y = y1;
        for (int i=0; i <= ah; i++) {
            // Add slope to increment angle formed
            slope_error_new += m_new;
      
            // Slope error reached limit, time to
            // increment y and update slope error.
            if (slope_error_new >= 0) {
                x+=incx;
                slope_error_new -= 2 * abs(y2 - y1);
            }
            y+=inc;
            duz[abs(y)-y1] = x;
        }
    }
}
void _lukaDrawTriangle(NG_POINT points[3]){
    int _p1i = minyp(points);
    int _p3i = maxxp2(points, _p1i);
    int _h2i = maxyp(points, _p1i);
    int _hfi = 3-_p1i-_h2i;
    int _p2i = 3-_p1i-_p3i;
    NG_POINT p1 = points[_p1i];
    NG_POINT p2 = points[_p2i];
    NG_POINT p3 = points[_p3i];
    
    int size12 = abs(p2.y-p1.y) + 1;
    int size23 = abs(p3.y-p2.y) + 1;
    int size13 = abs(p3.y-p1.y) + 1;
    
    int duz12[size12];
    int duz23[size23];
    int duz13[size13];
    _lukaTriangCalcMinxs(p1, p2, duz12);
    _lukaTriangCalcMinxs(p3, p2, duz23);
    _lukaTriangCalcMinxs(p1, p3, duz13);
    
    NG_POINT _pmax = maxp(p1, p2, p3);
    int halfy = points[_hfi].y;
    for (int y=max(p1.y,0); y<min(_pmax.y, __ngScreenHeight); y++){
        int i1 = y-p1.y;
        if (y<halfy){
            int _imin = min(duz12[i1], duz13[i1]);
            int _imax = max(duz12[i1], duz13[i1]);
            for (int i=max(_imin, 0); i<min(_imax, __ngScreenWidth); i++) ngDrawPixel(i, y);
        } else {
            int i2 = y-halfy;
            int _imin = min(duz12[i1], duz23[i2]);
            int _imax = max(duz12[i1], duz23[i2]);
            for (int i=max(_imin, 0); i<min(_imax, __ngScreenWidth); i++) ngDrawPixel(i, y);
        }
    }
}

void lukaDrawTriangle(NG_POINT _d1, NG_POINT _d2, NG_POINT _d3){
    NG_POINT points[3] = {_d1, _d2, _d3};
    _lukaDrawTriangle(points);
}

void lukaDrawQuad2D(NG_POINT points[4], int type){
    if (type == NG_TRIANGLE_FAN){
        lukaDrawTriangle(points[0], points[1], points[2]);
        lukaDrawTriangle(points[0], points[2], points[3]);
    } else if (type == NG_TRIANGLE_Z){
        lukaDrawTriangle(points[0], points[1], points[3]);
        lukaDrawTriangle(points[0], points[3], points[2]);
    }
}
