//
//  shaders.h
//  Emulator
//
//  Created by Luka on 13.9.24..
//

#ifndef shaders_h
#define shaders_h

#include <glad/glad.h>

extern const char *__VSSRC;
#define DEFAULT_VERTEX_SHADER_SOURCE __VSSRC
extern const char *__FSSRC;
#define DEFAULT_FRAGMENT_SHADER_SOURCE __FSSRC
extern int shaderSuccess;
extern char shaderInfoLog[];

void logShader(GLuint);
GLuint createShader(const char*, GLenum);

#endif /* shaders_h */
