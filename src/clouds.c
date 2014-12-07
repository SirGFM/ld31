/**
 * @file src/clouds.c
 */
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_spriteset.h>
#include <GFraMe/GFraMe_util.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "clouds.h"

typedef struct {
    int y;
    float offx;
    int vx;
    int active;
} cloud;

static cloud **p_arr = NULL;
static int p_max = 0;
static int p_len = 0;

static GFraMe_ret c_realloc(int num);
static cloud *c_get_particle();

GFraMe_ret c_init(int num) {
    int i;
    GFraMe_ret rv = GFraMe_ret_failed;
    
    rv = c_realloc(num);
    ASSERT(rv == GFraMe_ret_ok);
    
    i = 0;
    while (i < num) {
        int y, x, vx;
        
        y = 21 + GFraMe_util_randomi() % 7;
        x = GFraMe_util_randomi() % SCRW;
        vx = 3 + GFraMe_util_randomi() % 8;
        
        c_spawn(y, (float)x, vx);
        
        i++;
    }
    
    rv = GFraMe_ret_ok;
__ret:
    return rv;
}

void c_clear() {
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

void c_spawn(int y, float offx, int vx) {
    cloud *p;
    
    p = c_get_particle();
    
    p->active = 1;
    p->y = y;
    p->offx = offx;
    p->vx = vx;
}

void c_update(int ms) {
    int i;
    float elapsed;
    
    elapsed = (float)ms / 1000.0f;
    i = 0;
    while (i < p_len) {
        float vy;
        cloud *p;
        
        p = p_arr[i++];
        
        p->offx += p->vx * elapsed;
        
        if (p->offx > SCRW)
            p->offx -= SCRW;
    }
}

void c_draw(int pos) {
    int i;
    
    i = 0;
    while (i < p_len) {
        cloud *p;
        
        p = p_arr[i++];
        
        if (p->active) {
            int tile;
            
            if (p->offx < 4)
                tile = 35;
            else if (p->offx < 44)
                tile = 32;
            else if (p->offx < 84)
                tile = 33;
            else if (p->offx < 124)
                tile = 34;
            else
                tile = 35;
            
            GFraMe_spriteset_draw
                (
                gl_sset8x4,
                tile,
                pos + p->offx,
                p->y,
                0
                );
            if (pos + p->offx + 8 - SCRW >= 0) {
                GFraMe_spriteset_draw
                    (
                    gl_sset8x4,
                    tile,
                    pos + p->offx - SCRW,
                    p->y,
                    0
                    );
            }
        }
    }
}

static GFraMe_ret c_realloc(int num) {
    GFraMe_ret rv = GFraMe_ret_failed;
    cloud **tmp = NULL;
    
    tmp = (cloud**)malloc(sizeof(cloud*)*num);
    ASSERT(tmp);
    
    memset(tmp, 0x0, sizeof(cloud*)*num);
    
    if (p_arr) {
        memcpy(tmp, p_arr, sizeof(cloud*)*p_max);
        
        free(p_arr);
    }
    p_arr = tmp;
    
    p_max = num;
    
    rv = GFraMe_ret_ok;
__ret:
    return rv;
}

static cloud*c_get_particle() {
    int i;
    cloud*p;
    
    i = 0;
    while (i < p_len) {
        p = p_arr[i];
        if (!p->active)
            return p;
        i++;
    }
    
    // yeah, I should probably check, but whatever
    if (i >= p_max)
        c_realloc(p_max*2);
    
    p = (cloud*)malloc(sizeof(cloud));
    p_arr[i] = p;
    p_len++;
    
    return p;
}

