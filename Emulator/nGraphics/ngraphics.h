//
//  ngraphics.h
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#ifndef ngraphics_h
#define ngraphics_h

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ng_gl.h"
#include "ng_window.h"
#include "ng_font.h"
#ifdef NG_INCLUDE_LOAD_UI
#include "ng_loadUI.h"
#endif

#define NG_WINDOW_CREATE_SUCCESS 0x0
#define NG_OPENGL_LOAD_SUCCESS 0x1
#define NG_OPENGL_PROGRAM_STATUS 0x2
#define NG_OPENGL_STATUS 0x3 // if opengl messes up somewhere this flag goes off
#define NG_USER_MAINLOOP_FUNC_ATTACHED 0x4
#define NG_DRAW_TEXT_BACKGROUND 0x5
#define NG_SCREEN_BUFFER_STATUS 0x6
#define NG_SCREEN_TEXTURE_STATUS 0x7
#define NG_SHOW_FPS 0x8
#define NG_TEXT_COLORING_DECLARED 0x9
#define NG_UI_LOADED 0xa

#define NG_TRUE 0x1
#define NG_FALSE 0x0

typedef void(* NGMAINLOOPFUNC)(void);

extern int __ngflags[];
extern GLuint __ngShaderProgram;
extern GLubyte* __ngScreen;
extern GLuint __ngScreenVBO, __ngScreenVAO, __ngScreenEBO;
extern GLuint __ngScreenTexture;
extern unsigned int __ngScreenWidth, __ngScreenHeight;
extern unsigned int __ngScreenSize;

void ngPerformExit(void);
void ngTerminate(void);
void ngWait(unsigned long);
void ngMainLoop(void);
void ngSetupScreen(void);
void ngSetMainLoopFunction(NGMAINLOOPFUNC);
NGMAINLOOPFUNC ngGetMainLoopFunction(void);
GLubyte* ngGetScreenBuffer(void);
void ngFlipScreen(_Bool,_Bool);

void ngConfig(int, int);
int ngGet(int);

// ------ DRAWING ------- //
#define NG_LAZY_WHITE 200,200,200
// drawing variables
extern GLubyte __ngColor[];
extern GLuint __ngFontScale;
extern GLint __ngTextKerning;

struct point { int x,y; };
typedef struct point point;
typedef struct point dot;
// and functions for them
GLintptr _ngPosition(int x, int y);
void ngFontScale(GLuint);
void ngTextKerning(GLint kern);
void ngColor(GLubyte, GLubyte, GLubyte);
int min(int a, int b);
int max(int a, int b);
dot minp(dot p1,dot p2,dot p3);
dot maxp(dot p1,dot p2,dot p3);
_Bool _ptInTriangle(dot p, dot p0, dot p1, dot p2);
// drawing operations
void ngClear(void);
void ngDrawPixel(int x, int y);
void ngDrawRectangle(GLuint x, GLuint y, GLuint w, GLuint h);
void _ngDrawTriangle(dot p1, dot p2, dot p3);
void ngDrawTriangle(GLuint x1,GLuint y1,GLuint x2,GLuint y2,GLuint x3,GLuint y3);
void ngDrawLine(GLuint x1, GLuint y1, GLuint x2, GLuint y2);
void ngDrawSprite_GRAY(GLuint x, GLuint y, unsigned char* sprite, unsigned int size[2]);

#endif /* ngraphics_h */
