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
#define NG_PRINT_LOG 0xb
#define NG_VIEW_OUT_SCREEN 0xc
#define NG_ALL_GRAY 0xd
#define NG_DEFAULT_TRIANGLE_ALGORITHM 0xe

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

#define NG_TRIANGLE_Z 0x0
#define NG_TRIANGLE_FAN 0x1

#define NG_TRI_ALGO_SOFN 0x0 // default
#define NG_TRI_ALGO_LUKA 0x1
#define NG_TRI_ALGO_PIXL 0x2
// drawing variables
extern GLubyte __ngColor[];
extern GLuint __ngFontScale;
extern GLint __ngTextKerning;

struct NG_POINT { int x,y; };
typedef struct NG_POINT NG_POINT;
typedef struct NG_POINT NG_DOT;

struct NG_DINT { int x1,x2; };
typedef struct NG_DINT NG_DINT;
typedef struct NG_DINT ng_dint;

struct NG_LINEAR_FUNCTION { float k; int n; _Bool dxIs0; };
typedef struct NG_LINEAR_FUNCTION ng_fn;
// and functions for them
GLintptr _ngPosition(int x, int y);
void ngFontScale(GLuint);
void ngTextKerning(GLint kern);
void ngColor(GLubyte, GLubyte, GLubyte);
int min(int a, int b);
int max(int a, int b);
NG_DOT minp(NG_DOT p1,NG_DOT p2,NG_DOT p3);
NG_DOT maxp(NG_DOT p1,NG_DOT p2,NG_DOT p3);
NG_DOT _minp(NG_DOT p[3]);
NG_DOT _maxp(NG_DOT p[3]);
_Bool _ptInTriangle(NG_DOT p, NG_DOT p0, NG_DOT p1, NG_DOT p2);
// drawing operations
void ngClear(void);
void ngDrawPixel(int x, int y);
void ngDrawRectangle(GLuint x, GLuint y, GLuint w, GLuint h);

// -- all triangle drawing algorithms -- //
void _ngDrawTriangle(NG_DOT p1, NG_DOT p2, NG_DOT p3);
void ngDrawTriangle(NG_POINT points[3]);
// pixel by pixel (slow)
void _pxDrawTriangle(NG_DOT p1, NG_DOT p2, NG_DOT p3);
void pxDrawTriangle(NG_POINT points[3]);
// luka scanline (doesn't work)
void _lukaDrawTriangle(NG_POINT p1, NG_POINT p2, NG_POINT p3);
void lukaDrawTriangle(NG_POINT points[3]);
// sof fn scanline (fast)
void _sofDrawTriangle(NG_POINT p1, NG_POINT p2, NG_POINT p3);
void sofDrawTriangle(NG_POINT points[3]);
// ------------------------------------- //

void ngDrawQuad2D(NG_POINT points[4], int type);
void sofDrawQuad2D(NG_POINT points[4], int type);
void lukaDrawQuad2D(NG_POINT points[4], int type);
void pxDrawQuad2D(NG_POINT points[4], int type);

void ngDrawLine(GLint x1, GLint y1, GLint x2, GLint y2);
void ngDrawLines(GLuint npoints, GLint* points, _Bool closeShape);

/**
 * Draws a line with given equation
 * @param fn - equation of the line
 * @param bounds - given bounds of the line. If set to NULL then it will draw thru whole screen
 * @see <struct ng_fn>
 */
void ngDrawfn(ng_fn* fn, NG_POINT bounds[2]);

void ngDrawSprite_GRAY(GLuint x, GLuint y, unsigned char* sprite, unsigned int size[2]);

#endif /* ngraphics_h */
