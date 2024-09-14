//
//  window.c
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#include "window.h"
#include "ngraphics.h"

GLFWwindow* __ngWindow = NULL;

void ngCreateWindow(int width, int height, const char* title){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    __ngScreenWidth = width;
    __ngScreenHeight = height;
    // glfw window creation
    __ngWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (__ngWindow == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        __ngflags[NG_WINDOW_CREATE_SUCCESS] = 0;
        return;
    }
    glfwMakeContextCurrent(__ngWindow);
    
    __ngScreen = (GLubyte*) malloc(width*height*3);
    
    __ngflags[NG_WINDOW_CREATE_SUCCESS] = 1;
}

int ngWindowShouldClose(void){
    return glfwWindowShouldClose(__ngWindow);
}

void ngSwapBuffers(void){
    glfwSwapBuffers(__ngWindow);
}
void ngPollEvents(void){
    glfwPollEvents();
}

void _ng_create_window_screen_buffers(void){
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &__ngScreenVAO);
    glGenBuffers(1, &__ngScreenVBO);
    glGenBuffers(1, &__ngScreenEBO);

    glBindVertexArray(__ngScreenVAO);

    glBindBuffer(GL_ARRAY_BUFFER, __ngScreenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __ngScreenEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void _ng_create_screen_texture(void){
    glGenTextures(1, &__ngScreenTexture);
    glBindTexture(GL_TEXTURE_2D, __ngScreenTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB,__ngScreenWidth,__ngScreenHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
}

int ngGet(int flag){
    return __ngflags[flag];
}
