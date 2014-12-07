/**
 * @file src/particles
 */
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_spriteset.h>
#include <GFraMe/GFraMe_util.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "particles.h"

typedef struct {
    float y;
    float offx;
    int vx;
    int tile;
    int speed;
    int active;
} particle;

static particle **p_arr = NULL;
static int p_max = 0;
static int p_len = 0;

static GFraMe_ret p_realloc(int num);
static particle *p_get_particle();

GFraMe_ret p_init() {
    GFraMe_ret rv = GFraMe_ret_failed;
    
    rv = p_realloc(32);
    ASSERT(rv == GFraMe_ret_ok);
    
    rv = GFraMe_ret_ok;
__ret:
    return rv;
}

void p_clear() {
    int i;
    
    i = 0;
    while (i < p_len) {
        free(p_arr[i]);
        i++;
    }
    p_len = 0;
    
    if (p_arr)
        free(p_arr);
    p_arr = NULL;
    p_max = 0;
}

void p_spawn(float y, float offx, int vx, int tile, int speed) {
    particle *p;
    
    p = p_get_particle();
    
    p->active = 1;
    p->y = y;
    p->offx = offx;
    p->vx = vx;
    p->tile = tile;
    if (speed >= -1 && speed < 3)
        p->speed = speed;
    else
        p->speed = 0;
}

void p_update(int ms) {
    int i;
    float elapsed;
    
    elapsed = (float)ms / 1000.0f;
    i = 0;
    while (i < p_len) {
        float vy;
        particle *p;
        
        p = p_arr[i++];
        if (p->speed == -1)
            vy = 20.0f + (int)(GFraMe_util_randomi() % 40);
        else if (p->speed == 0)
            vy = 10.0f + (int)(GFraMe_util_randomi() % 30);
        else if (p->speed == 1)
            vy = 5.0f + (int)(GFraMe_util_randomi() % 15);
        else if (p->speed == 2)
            vy = 2.5f + (int)(GFraMe_util_randomi() % 10);
        
        p->y += vy * elapsed;
        if (p->y > 47) {
            p->active = 0;
            continue;
        }
        
        if (p->vx != 0)
            p->offx += p->vx * elapsed;
    }
}

void p_draw(int pos) {
    int i;
    
    i = 0;
    while (i < p_len) {
        particle *p;
        
        p = p_arr[i++];
        
        if (p->active) {
            GFraMe_spriteset_draw
                (
                gl_sset4x4,
                p->tile,
                pos + p->offx,
                p->y,
                0
                );
            if (pos + p->offx + 4 - SCRW >= 0) {
                GFraMe_spriteset_draw
                    (
                    gl_sset4x4,
                    p->tile,
                    pos + p->offx - SCRW,
                    p->y,
                    0
                    );
            }
        }
    }
}

static GFraMe_ret p_realloc(int num) {
    GFraMe_ret rv = GFraMe_ret_failed;
    particle **tmp = NULL;
    
    tmp = (particle**)malloc(sizeof(particle*)*num);
    ASSERT(tmp);
    
    memset(tmp, 0x0, sizeof(particle*)*num);
    
    if (p_arr) {
        memcpy(tmp, p_arr, sizeof(particle*)*p_max);
        
        free(p_arr);
    }
    p_arr = tmp;
    
    p_max = num;
    
    rv = GFraMe_ret_ok;
__ret:
    return rv;
}

static particle *p_get_particle() {
    int i;
    particle *p;
    
    i = 0;
    while (i < p_len) {
        p = p_arr[i];
        if (!p->active)
            return p;
        i++;
    }
    
    // yeah, I should probably check, but whatever
    if (i >= p_max)
        p_realloc(p_max*2);
    
    p = (particle*)malloc(sizeof(particle));
    p_arr[i] = p;
    p_len++;
    
    return p;
}

