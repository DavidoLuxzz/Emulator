//
//  -- PONG --
//  prog0.c
//  Emulator
//
//  Created by Luka on 17.9.24..
//

#include "prog.h"
#include "../../nGraphics/ngraphics.h"
#include "../../emu_main.h"

_Bool redraw = NG_TRUE;

int scores[2];
char _sscores[3];
int ballx,bally;
int balldx,balldy;
int p1y,p2y, pv;
const int PADDLE_HALF_LENGTH = 40;

void prog0_input(void){
    if (ngGetKey(GLFW_KEY_ESCAPE)) emuExit(0);
    if (ngGetKey(GLFW_KEY_W)) p1y-=pv;
    if (ngGetKey(GLFW_KEY_S)) p1y+=pv;
    if (ngGetKey(GLFW_KEY_UP)) p2y-=pv;
    if (ngGetKey(GLFW_KEY_DOWN)) p2y+=pv;
}

int prog0_pip(int pid){
    if (!pid) // player 1
        return bally >= (p1y-PADDLE_HALF_LENGTH) && bally <= (p1y+PADDLE_HALF_LENGTH);
    // player 2
    return bally >= (p2y-PADDLE_HALF_LENGTH) && bally <= (p2y+PADDLE_HALF_LENGTH);
}
void prog0_score_up(int pid){
    scores[pid]++;
    ballx = __ngScreenWidth/2;
    bally = __ngScreenHeight/2;
    
    _sscores[2] = scores[0]+'0';
    _sscores[0] = scores[1]+'0';
    
    ngWait(60);
}
void prog0_move_ball(void){
    ballx += balldx;
    bally += balldy;
    
    if (ballx-10 <= 15){
        if (prog0_pip(0))
            balldx = -balldx;
        else prog0_score_up(0);
    }
    if (ballx+25 >= __ngScreenWidth) {
        if (prog0_pip(1))
            balldx = -balldx;
        else prog0_score_up(1);
    }
    if (bally-10 <= 0 || bally+10 >= __ngScreenHeight) balldy = -balldy;
}

void prog0_main(void){
    // jebiga morao sam da radim sudove
    prog0_input();
    if (p1y-PADDLE_HALF_LENGTH < 0) p1y = PADDLE_HALF_LENGTH;
    if (p1y+PADDLE_HALF_LENGTH > __ngScreenHeight) p1y = __ngScreenHeight-PADDLE_HALF_LENGTH;
    if (p2y-PADDLE_HALF_LENGTH < 0) p2y = PADDLE_HALF_LENGTH;
    if (p2y+PADDLE_HALF_LENGTH > __ngScreenHeight) p2y = __ngScreenHeight-PADDLE_HALF_LENGTH;
if (redraw){
    ngColor(0,0,0);
    ngClear(); // obrise ekran
    
    ngFontScale(24);
    ngColor(50,50,50);
    ngDrawText(200,100, _sscores,3);
    ngColor(NG_LAZY_WHITE);

    // crtanje reketa
    ngDrawRectangle(5, p1y-PADDLE_HALF_LENGTH, 10, 2*PADDLE_HALF_LENGTH);
    ngDrawRectangle(__ngScreenWidth-15, p2y-PADDLE_HALF_LENGTH, 10, 2*PADDLE_HALF_LENGTH);
    
    // crtanje loptice
    ngDrawRectangle(ballx-10, bally-10, 21, 21);
}
    prog0_move_ball();
}

void prog0_init(void){
    ballx = __ngScreenWidth/2;
    bally = __ngScreenHeight/2;
    balldx = 4;
    balldy = 4;
    p1y = __ngScreenHeight/2;
    p2y = __ngScreenHeight/2;
    pv = 6; // brzina reketa
    scores[0] = 0;
    scores[1] = 0;
    _sscores[0] = '0';
    _sscores[1] = ':';
    _sscores[2] = '0';
    redraw = NG_TRUE;
}
