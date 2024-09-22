//
//  gm.h
//  Emulator
//
//  Created by Luka on 22.9.24..
//

#ifndef __graphics_math
#define __graphics_math

enum GM_AXIS {
    GM_X_AXIS,
    GM_Y_AXIS,
    GM_Z_AXIS
};

struct gm_dot { // 3d
    float x,y,z;
};
struct gm_Quad { // 3d
    struct gm_dot dots[4];
};

struct gm_Camera {
    float x,y,z;
    float angA, angB, angC;
};

struct gm_dot2 { // 2d
    float x,y;
};
struct gm_Quad2D { // 2d
    struct gm_dot2 dots[4];
};
struct gm_Quadf2D { // 2d uses float[] instead of dot2
    float verts[8];
};

// from gm.c
void gm_ortho(float cx, float cy, float znear, float zfar);
struct gm_Camera* gm_camera(float x, float y, float z);
void gm_rotateA(float);
float gm_get_rotateA(void);
void gm_rotateB(float);
float gm_get_rotateB(void);
void gm_rotateC(float);
float gm_get_rotateC(void);
struct gm_dot gm_rotate_vector(struct gm_dot, float, int axis);
struct gm_dot gm_transformR(struct gm_dot);
struct gm_dot2 gm_screen_dot(struct gm_dot);
struct gm_Quad2D gm_screen_quad(struct gm_Quad*);
struct gm_Quadf2D gm_screen_quadf(struct gm_Quad*);

#endif

