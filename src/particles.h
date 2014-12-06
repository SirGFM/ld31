/**
 * @file src/particles
 */
#ifndef __PARTICLES_H_
#define __PARTICLES_H_

#include <GFraMe/GFraMe_error.h>

GFraMe_ret p_init();
void p_clear();
void p_spawn(float y, float offx, int vx, int tile, int speed);
void p_update(int ms);
void p_draw();

/*
typedef struct {
    float y;
    float offx;
    int vx;
    int tile;
    int active;
} particle;
*/

#endif

