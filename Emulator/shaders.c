//
//  shaders.c
//  Emulator
//
//  Created by Luka on 13.9.24..
//

#include "shaders.h"

int shaderSuccess;
char shaderInfoLog[512];

// vertex shader source
const char *__VSSRC =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
// fragment shader source
const char *__FSSRC =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void logShader(GLuint shader){
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
    glGetShaderInfoLog(shader, 512, NULL, shaderInfoLog);
}

GLuint createShader(const char *src, GLenum type){
    GLuint shader;
    shader = glCreateShader(type);
    
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    logShader(shader);
    
    return shader;
}
