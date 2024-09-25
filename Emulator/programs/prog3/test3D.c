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

#define DR 0.0174533 // 1 degree in radians
#define PI M_PI

_Bool redrawa = NG_TRUE;
struct gm_Camera* prog3_camera;
const float PROG3_FOV = 90.0f;
const float PROG3_SPEED = 20.0f;
_Bool w,s,a,d,up,down;
int mousex, mousey;
struct gm_Quad dots = {.dots={ {240.0,160.0,200.0}, {720.0,160.0,200.0}, {720.0,480.0,200.0}, {240.0,480.0,200.0} }};

void prog3_init(void){
    redrawa = NG_TRUE;
    gm_ortho((float)__ngScreenWidth/2.0, (float)__ngScreenHeight/2.0, (float)__ngScreenWidth*tanf(DR*(float)PROG3_FOV/2.0)/2.0, 1000.0);
    gm_set_clip_z(0.0f);
    prog3_camera = gm_camera(-140, -100.0, 300);
    prog3_camera->angA = -40.0f;
    ngUpdateMousePos();
    mousex = ngGetMousePosX();
    mousey = ngGetMousePosY();
}

void prog3_updatePos(void){
    float angle = PI*gm_get_rotateA()/180.0f;
    float dz = (float)(s-w)*PROG3_SPEED;
    float dx = (float)(d-a)*PROG3_SPEED;
    float dy = (float)(down-up)*PROG3_SPEED;
    
    prog3_camera->z -= (dz*cos(angle) + dx*cos(fmod(angle+PI/2.0f, 2.0f*PI)));
    prog3_camera->x += (dz*sin(angle) + dx*sin(fmod(angle+PI/2.0f, 2.0f*PI)));
    prog3_camera->y += dy;
}
void prog3_updateRotate(void){
    int dx = ngGetMousePosX() - mousex;
    int dy = ngGetMousePosY() - mousey;
    
    mousex = ngGetMousePosX();
    mousey = ngGetMousePosY();
    
    gm_rotateA(gm_get_rotateA() - (float)dx);
    gm_rotateB(gm_get_rotateB() + (float)dy);
    
    ngUpdateMousePos();
}
void prog3_main(void){
    if (ngGetKey(GLFW_KEY_ESCAPE)) emuExit(0);
    if (ngGetKey(GLFW_KEY_LEFT)) gm_rotateA(gm_get_rotateA() + 1.0);
    if (ngGetKey(GLFW_KEY_RIGHT)) gm_rotateA(gm_get_rotateA() - 1.0);
    if (ngGetKey(GLFW_KEY_UP)) gm_rotateB(gm_get_rotateB() - 1.0);
    if (ngGetKey(GLFW_KEY_DOWN)) gm_rotateB(gm_get_rotateB() + 1.0);
    if (ngGetKey(GLFW_KEY_R)) gm_camera(0.0, 0.0, 0.0);
//    if (ngGetKey(GLFW_KEY_G)) printf("x: %.0f y: def z: %.0f,  a: %.0f\n", prog3_camera->x, prog3_camera->z, prog3_camera->angA);
    w = ngGetKey(GLFW_KEY_W)!=0;
    s = ngGetKey(GLFW_KEY_S)!=0;
    a = ngGetKey(GLFW_KEY_A)!=0;
    d = ngGetKey(GLFW_KEY_D)!=0;
    up = ngGetKey(GLFW_KEY_SPACE)!=0;
    down = ngGetKey(GLFW_KEY_LEFT_SHIFT)!=0;
    prog3_updatePos();
    prog3_updateRotate();
    
    if (redrawa){
        ngColor(0, 0, 0);
        ngClear();
        ngColor(255,0,0);

        struct gm_Quad2D sq = gm_screen_quad(&dots);
        
//        if (gmGet(GM_OBJECT_VISIBLE)){
            NG_POINT points[4];
            for (int i=0; i<4; i++){
                points[i].x = (int) sq.dots[i].x;
                points[i].y = (int) sq.dots[i].y;
            }
            ngDrawQuad2D(points, NG_TRIANGLE_FAN);
//        }
        
//        redrawa = 0;
    }
}
