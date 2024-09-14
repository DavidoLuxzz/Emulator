#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders.h"

const float screenBounds[] = {0.0,0.0,0.0, 1.0,0.0,0.0, 1.0,1.0,0.0, 0.0,1.0,0.0};
const unsigned int screenIndices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};
int WIDTH, HEIGHT;
int COLOR_MODE, DATA_SIZE;
GLFWwindow* window = NULL;
void createWindow(int _width, int _height, const char* _title){
    WIDTH = _width;
    HEIGHT = _height;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
}

GLuint vertexShaderID, fragmentShaderID;
GLuint shaderProgramID;

int createShaders(void){
    vertexShaderID = createShader(DEFAULT_VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER);
    printf("%s\n", shaderInfoLog);
    if (!shaderSuccess){
        return -1;
    }
    fragmentShaderID = createShader(DEFAULT_FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER);
    printf("%s\n", shaderInfoLog);
    if (!shaderSuccess){
        return -1;
    }
    return 1;
}
int createShaderProgram(void){
    shaderProgramID = glCreateProgram();
    
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
    printf("%s\n", infoLog);
    if(!success) {
        return -1;
    }
    return 1;
}

GLuint pixelBuffer;
GLubyte* currentScreenBuffer;
void createPixelBuffer(int colMode){
    COLOR_MODE = colMode;
    DATA_SIZE = WIDTH*HEIGHT*colMode;
    glGenBuffers(1, &pixelBuffer);
    glBindBuffer(pixelBuffer, GL_PIXEL_UNPACK_BUFFER);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, NULL, GL_STREAM_DRAW);
}
void startPBO(void){
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBuffer);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, NULL, GL_STREAM_DRAW);
    currentScreenBuffer = (GLubyte*) glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
}
void endPBO(void){
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    currentScreenBuffer = NULL;
}
GLuint screenTextureID;
void createScreenTexture(void){
    glGenTextures(1, &screenTextureID);
    glBindTexture(GL_TEXTURE_2D, screenTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLint format = GL_RED;
    if (COLOR_MODE == 3) format = GL_RGB;
    else if (COLOR_MODE == 4) format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
}
void startScreenTexture(void){
    glBindTexture(GL_TEXTURE_2D, screenTextureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, 0);
}
void setPixel(int x, int y, const unsigned char *color){
    if (currentScreenBuffer){
        GLintptr pos = COLOR_MODE * (y*WIDTH + x);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBuffer);
        currentScreenBuffer[pos] = color[0];
        currentScreenBuffer[pos+1] = color[1];
        currentScreenBuffer[pos+2] = color[2];
        for (int i=0; i<128*3; i++){
            currentScreenBuffer[i] = 255;
        }
        glBindTexture(GL_TEXTURE_2D, screenTextureID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, currentScreenBuffer);
    } else {
        printf("klinac\n");
    }
}

GLuint screenVAO, screenVBO, screenEBO;
void createScreenVABEO(void){
    glGenVertexArrays(1, &screenVAO);
    glBindVertexArray(screenVAO);
    
    glGenBuffers(1, &screenVBO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenBounds), screenBounds, GL_STREAM_DRAW);
    
    glGenBuffers(1, &screenEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screenIndices), screenIndices, GL_STREAM_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

int main(void) {
    if (!glfwInit()){
        fprintf(stderr, "GLFW wasn't initialized\n");
        return -1;
    }
    
    createWindow(800, 600, "OpenGL Library");
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }
    glViewport(0,0, WIDTH,HEIGHT);
    
    if (!createShaders())       { glfwTerminate(); return -1;};
    if (!createShaderProgram()) { glfwTerminate(); return -1;};
    createPixelBuffer(3); // 3 components (RGB)
    createScreenTexture();
    
    const unsigned char color_red[3] = {255,0,0};
    
    startPBO();
    startScreenTexture();
    // potrebna je texturaza crtanje !!
        glBindBuffer(pixelBuffer, GL_PIXEL_UNPACK_BUFFER);
        setPixel(0, 0, color_red);
//        glDrawBuffer(GL_BACK);
    endPBO();
    
    glfwShowWindow(window);
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgramID);
        glBindTexture(GL_TEXTURE_2D, screenTextureID);
        glBindVertexArray(screenVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }
    
    glDeleteBuffers(1, &pixelBuffer);
    glDeleteTextures(1, &screenTextureID);
    glDeleteProgram(shaderProgramID);
    glfwTerminate();
    return 0;
}
