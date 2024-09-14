//
//  shaders.h
//  Emulator
//
//  Created by Luka on 13.9.24..
//

#ifndef shaders_h
#define shaders_h

extern const char *__VSSRC;
#define DEFAULT_VERTEX_SHADER_SOURCE __VSSRC
extern const char *__FSSRC;
#define DEFAULT_FRAGMENT_SHADER_SOURCE __FSSRC
extern int shaderSuccess;
extern char shaderInfoLog[];

void logShader(unsigned int);
unsigned int _ng_create_shader(const char*, unsigned int);
unsigned int _ng_create_program(void);

#endif /* shaders_h */
