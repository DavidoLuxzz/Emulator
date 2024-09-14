//
//  ng_gl.c
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#include "ng_gl.h"
#include "ngraphics.h"
#include "shaders.h"

void ngLoadOpenGL(void){
    int gl_load_success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    __ngflags[NG_OPENGL_STATUS] = gl_load_success;
    __ngflags[NG_USER_MAINLOOP_FUNC_ATTACHED] = 0;
    if (gl_load_success){
        __ngShaderProgram = _ng_create_program();
    } else {
        __ngflags[NG_OPENGL_PROGRAM_STATUS] = 0;
    }
}
