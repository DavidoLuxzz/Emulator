//
//  gm.c
//  Emulator
//
//  Created by Luka on 22.9.24..
//

#include "gm.h"
#include <stdio.h>
#include <math.h>

#define PI M_PI

float gmZnear, gmZfar;
float gmCenterX, gmCenterY;
struct gm_Camera gmCamera;
float gmClipZ;

float sign1f(float a){
    return (float)a>0.0;
}

int __gmflags[GM_NUM_FLAGS];
int gmGet(int flag){
    return __gmflags[flag];
}
void gmResetFlags(void){
    for (int i=0; i<GM_NUM_FLAGS; i++) __gmflags[i] = 0;
}

struct gm_dot gm_rotate_vector(struct gm_dot a, float angle, int axis){
    struct gm_dot res;
    float ang = PI*angle/180.0;
    if (axis == GM_Y_AXIS){
        res.x = a.x*cosf(ang) + a.z*sinf(ang);
        res.y = a.y;
        res.z = -a.x*sinf(ang) + a.z*cosf(ang);
    } else if (axis == GM_X_AXIS){
        res.x = a.x;
        res.y = a.y*cosf(ang) - a.z*sinf(ang);
        res.z = a.y*sinf(ang) + a.z*cosf(ang);
    } else if (axis == GM_Z_AXIS){
        res.x = a.x*cosf(ang) - a.y*sinf(ang);
        res.y = a.x*sinf(ang) + a.y*cosf(ang);
        res.z = a.z;
    } else return a;
    return res;
}
struct gm_dot gm_transformR(struct gm_dot a){
    struct gm_dot res;
    res.x = a.x - gmCamera.x - gmCenterX;
    res.y = a.y - gmCamera.y - gmCenterY;
    // made exactly for drawing quad lines  (original code two lines below)
    // res.z = a.z - fmod(gmCamera.z, 50.0) + gmZnear - 200.0;
    // original:
    res.z = a.z - gmCamera.z + gmZnear;
    res = gm_rotate_vector(res, gmCamera.angA, GM_Y_AXIS);
    res = gm_rotate_vector(res, gmCamera.angB, GM_X_AXIS);
    return res;
}

struct gm_dot2 gm_screen_dot(struct gm_dot t){
    struct gm_dot d = gm_transformR(t);
    struct gm_dot2 S;
    if (d.z < gmClipZ) {
        __gmflags[GM_POINT_VISIBLE] = 0;
        __gmflags[GM_POINT2D_VISIBLE] = 0;
    }
    else {
        __gmflags[GM_POINT_VISIBLE] = 1;
        __gmflags[GM_POINT2D_VISIBLE] = 1;
    }
    if (d.z <= 0.0){
        S.x = d.x*gmZnear/1.0 + gmCenterX;
        S.y = d.y*gmZnear/1.0 + gmCenterY;
    } else {
        S.x = d.x*gmZnear/d.z + gmCenterX;
        S.y = d.y*gmZnear/d.z + gmCenterY;
        if (S.x>=0 && S.x<2*gmCenterX &&
            S.y>=0 && S.y<2*gmCenterY)
                __gmflags[GM_POINT2D_VISIBLE] = 1;
        else __gmflags[GM_POINT2D_VISIBLE] = 0;
    }
    return S;
}

struct gm_Triang2D gm_screen_triangle(struct gm_dot p1, struct gm_dot p2, struct gm_dot p3){
    struct gm_Triang2D ret = {.dots = {0}};
    ret.dots[0] = gm_screen_dot(p1);
    ret.dots[1] = gm_screen_dot(p2);
    ret.dots[2] = gm_screen_dot(p3);
    return ret;
}

struct gm_Quad2D gm_screen_quad(struct gm_Quad *q){
    struct gm_Quad2D ret = {.dots = {0}};
    int vres = 0;
    for (int i=0; i<4; i++){
        ret.dots[i] = gm_screen_dot(q->dots[i]);
        vres += __gmflags[GM_POINT_VISIBLE] & __gmflags[GM_POINT2D_VISIBLE];
    }
    if (vres > 1) __gmflags[GM_OBJECT_VISIBLE] = 1;
    else __gmflags[GM_OBJECT_VISIBLE] = 0;
    return ret;
}
struct gm_Quadf2D gm_screen_quadf(struct gm_Quad *q){
    struct gm_Quadf2D ret = {.verts = {0.0f}};
    for (int i=0; i<4; i++){
        struct gm_dot2 d = gm_screen_dot(q->dots[i]);
        ret.verts[2*i] = d.x;
        ret.verts[2*i +1] = d.y;
    }
    return ret;
}

void gm_ortho(float cx, float cy, float znear, float zfar){
    gmCenterX = cx;
    gmCenterY = cy;
    gmZnear = znear;
    gmZfar = zfar;
    gmClipZ = znear/8.0f;
}
void gm_set_clip_z(float clip){
    gmClipZ = clip;
}
struct gm_Camera* gm_camera(float x, float y, float z){
    gmCamera.x = x;
    gmCamera.y = y;
    gmCamera.z = z;
    gmCamera.angA = 0.0;
    gmCamera.angB = 0.0;
    gmCamera.angC = 0.0;
    return &gmCamera;
}
void gm_rotateA(float angA){
    gmCamera.angA = fmod(angA, 360.0);
}
float gm_get_rotateA(void){
    return gmCamera.angA;
}
void gm_rotateB(float ang){
    gmCamera.angB = fmod(ang, 360.0);
}
float gm_get_rotateB(void){
    return gmCamera.angB;
}
void gm_rotateC(float ang){
    gmCamera.angC = fmod(ang, 360.0);
}
float gm_get_rotateC(void){
    return gmCamera.angC;
}
