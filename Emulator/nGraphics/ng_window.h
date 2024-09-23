//
//  window.h
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#ifndef window_h
#define window_h

#include <stdio.h>

#define NG_RELEASE 0 // GLFW_RELEASE
#define NG_PRESS 1 // GLFW_PRESS
#define NG_REPEAT 2 // GLFW_REPEAT

void ngCreateWindow(int, int, const char*);
int ngWindowShouldClose(void);
void ngSwapBuffers(void);
void ngPollEvents(void);
int ngGetMousePosX(void);
int ngGetMousePosY(void);
void ngUpdateMousePos(void);
int ngGetKey(int key);
void ngLockKey(int key);
void ngUnlockKey(int key);
_Bool ngIsKeyLocked(int key);
void ngSetWindowTitle(const char*);
void _ng_create_window_screen_buffers(void);
void _ng_create_screen_texture(void);

#endif /* window_h */
