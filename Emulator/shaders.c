//
//  shaders.c
//  Emulator
//
//  Created by Luka on 13.9.24..
//

#include "shaders.h"
#include "ngraphics.h"

int shaderSuccess;
char shaderInfoLog[512];

const char *__VSSRC =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main(){\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "    ourColor = aColor;\n"
    "    TexCoord = aTexCoord;\n"
    "}\0";
const char *__FSSRC =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main(){\n"
    "    FragColor = texture(ourTexture, TexCoord);\n"
    "}\0";


void logShader(GLuint shader){
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
    glGetShaderInfoLog(shader, 512, NULL, shaderInfoLog);
}

GLuint _ng_create_shader(const char *src, GLenum type){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
    
    return shader;
}
GLuint _ng_create_program(void){
    GLuint vert = _ng_create_shader(__VSSRC, GL_VERTEX_SHADER);
    GLuint frag = _ng_create_shader(__FSSRC, GL_FRAGMENT_SHADER);
    // link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    __ngflags[NG_OPENGL_PROGRAM_STATUS] = success;
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("%s\n", infoLog);
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
    
    return shaderProgram;
}
