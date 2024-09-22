//
//  test3D.c
//  Emulator
//
//  Created by Luka on 22.9.24..
//

#include "ngraphics.h"
#include "gm.h"
#include "emu_main.h"
#include <math.h>

#define PI M_PI

_Bool redrawa = NG_TRUE;
struct gm_Camera* prog3_camera;
const float PROG3_FOV = 90.0f;
const float PROG3_SPEED = 20.0f;
_Bool w,s,a,d,up,down;

void prog3_init(void){
    redrawa = NG_TRUE;
    gm_ortho((float)__ngScreenWidth/2.0, (float)__ngScreenHeight/2.0, (float)__ngScreenWidth*tanf((float)PROG3_FOV/2.0)/2.0, 1000.0);
    prog3_camera = gm_camera(0.0, -100.0, 0.0);
}

void prog3_updatePos(void){
    float angle = PI*gm_get_rotateA()/180.0;
    float dz = (float)(s-w)*PROG3_SPEED;
    float dx = (float)(d-a)*PROG3_SPEED;
    float dy = (float)(down-up)*PROG3_SPEED;
    
    prog3_camera->z -= (dz*cos(angle) + dx*cos(fmod(angle+PI/2.0, 2.0*PI)));
    prog3_camera->x += (dz*sin(angle) + dx*sin(fmod(angle+PI/2.0, 2.0*PI)));
    prog3_camera->y += dy;
}

void prog3_main(void){
    if (ngGetKey(GLFW_KEY_ESCAPE)) emuExit(0);
    if (ngGetKey(GLFW_KEY_LEFT)) gm_rotateA(gm_get_rotateA() + 1.0);
    if (ngGetKey(GLFW_KEY_RIGHT)) gm_rotateA(gm_get_rotateA() - 1.0);
    if (ngGetKey(GLFW_KEY_UP)) gm_rotateB(gm_get_rotateB() - 1.0);
    if (ngGetKey(GLFW_KEY_DOWN)) gm_rotateB(gm_get_rotateB() + 1.0);
    w = ngGetKey(GLFW_KEY_W)!=0;
    s = ngGetKey(GLFW_KEY_S)!=0;
    a = ngGetKey(GLFW_KEY_A)!=0;
    d = ngGetKey(GLFW_KEY_D)!=0;
    up = ngGetKey(GLFW_KEY_SPACE)!=0;
    down = ngGetKey(GLFW_KEY_LEFT_SHIFT)!=0;
    prog3_updatePos();
    
    if (redrawa){
        ngColor(0, 0, 0);
        ngClear();
        ngColor(100, 100, 100);
        struct gm_dot dots[4] = {240.0,160.0,200.0, 720.0,160.0,200.0, 720.0,480.0,200.0, 240.0,480.0,200.0};
        struct gm_dot2 sdots[4];
        
        sdots[0] = gm_screen_dot(dots[0]);
        sdots[1] = gm_screen_dot(dots[1]);
        sdots[2] = gm_screen_dot(dots[2]);
        sdots[3] = gm_screen_dot(dots[3]);
        
        NG_POINT points[4];
        for (int i=0; i<4; i++){
            points[i].x = (int) sdots[i].x;
            points[i].y = (int) sdots[i].y;
        }
        // ngDrawLines(3, points, 1);
        ngDrawQuad2D(points, NG_TRIANGLE_FAN);
//        redrawa = 0;
    }
}
