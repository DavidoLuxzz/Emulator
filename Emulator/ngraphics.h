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
#include "window.h"

#define NG_WINDOW_CREATE_SUCCESS 0x0
#define NG_OPENGL_LOAD_SUCCESS 0x1
#define NG_OPENGL_PROGRAM_STATUS 0x2
#define NG_OPENGL_STATUS 0x3 // if opengl messes up somewhere this flag goes off
#define NG_USER_MAINLOOP_FUNC_ATTACHED 0x4

typedef void(* NGMAINLOOPFUNC)(void);

extern int __ngflags[];
extern GLuint __ngShaderProgram;
extern GLubyte* __ngScreen;
extern GLuint __ngScreenVBO, __ngScreenVAO, __ngScreenEBO;
extern GLuint __ngScreenTexture;
extern unsigned int __ngScreenWidth, __ngScreenHeight;

void ngTerminate(void);
void ngMainLoop(void);
void ngSetupScreen(void);
void ngSetMainLoopFunction(NGMAINLOOPFUNC);
GLubyte* ngGetScreenBuffer(void);
void ngFlipScreen(_Bool,_Bool);

// ------ DRAWING ------- //
// drawing variables
extern GLubyte __ngColor[];
// drawing operations
void ngClear(void);
void ngColor(GLubyte, GLubyte, GLubyte);
void ngDrawPixel(int x, int y);
void ngDrawRectangle(GLuint x, GLuint y, GLuint w, GLuint h);

#endif /* ngraphics_h */
