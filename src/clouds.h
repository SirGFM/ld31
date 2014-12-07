/**
 * @file src/particles
 */
#ifndef __CLOUDS_H_
#define __CLOUDS_H_

#include <GFraMe/GFraMe_error.h>

GFraMe_ret c_init(int num);
void c_clear();
void c_spawn(int y, float offx, int vx);
void c_update(int ms);
void c_draw();

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

