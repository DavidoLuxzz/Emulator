//
//  window.h
//  Emulator
//
//  Created by Luka on 14.9.24..
//

#ifndef window_h
#define window_h

#include <stdio.h>

void ngCreateWindow(int, int, const char*);
int ngWindowShouldClose(void);
void ngSwapBuffers(void);
void ngPollEvents(void);
void _ng_create_window_screen_buffers(void);
void _ng_create_screen_texture(void);

int ngGet(int);

#endif /* window_h */
