/**
 * @file src/chainball.c
 */
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_log.h>
#include <GFraMe/GFraMe_pointer.h>
#include <GFraMe/GFraMe_sprite.h>
#include <GFraMe/GFraMe_spriteset.h>
#include <GFraMe/GFraMe_util.h>

#include "global.h"

#include <stdio.h>
#include <stdlib.h>

#define CBX     SCRW/2
#define CBY     SCRH/2
#define CBW     16
#define CBH     16
#define CBOX    0
#define CBOY    0
#define CBALPHA 0.25f
#define DAMP_ACC 500
#define MAXV    400
#define MAXA    300
#define MAXDIST 200

int center_x;
int center_y;

static GFraMe_sprite chainball;
static int chain_num = 0;
static int *chain_lx = NULL;
static int *chain_ly = NULL;
static int *chain_x = NULL;
static int *chain_y = NULL;

GFraMe_ret cb_init(int num) {
    GFraMe_ret rv;
    
    rv = GFraMe_ret_failed;
    
    chain_num = num;
    
    chain_x = (int*)malloc(sizeof(int)*num);
    ASSERT(chain_x);
    chain_lx = (int*)malloc(sizeof(int)*num);
    ASSERT(chain_lx);
    chain_y = (int*)malloc(sizeof(int)*num);
    ASSERT(chain_y);
    chain_ly = (int*)malloc(sizeof(int)*num);
    ASSERT(chain_ly);
    
    GFraMe_sprite_init
        (
        &chainball,
        CBX,
        CBY,
        CBW,
        CBH,
        gl_sset16x16,
        CBOX,
        CBOY
        );
    chainball.cur_tile = 32;
    
    center_x = CBX;
    center_y = CBY;
    
    rv = GFraMe_ret_ok;
__ret:
    return rv;
}

void cb_clean() {
    if (chain_x)
        free(chain_x);
    chain_x = NULL;
    if (chain_y)
        free(chain_y);
    chain_y = NULL;
}

void cb_update(int ms, int did_move) {
    int i;
    float dx, dy;
    double damp_acc;
    
    damp_acc = DAMP_ACC * ((double)ms / 1000.0);
    
    if (did_move) {
        chainball.obj.ax += 0.05 * (GFraMe_pointer_x - chainball.obj.x);
        chainball.obj.ay += 0.05 * (GFraMe_pointer_y - chainball.obj.y);
        chainball.obj.vx += 0.0125 * (GFraMe_pointer_x - chainball.obj.x);
        chainball.obj.vy += 0.0125 * (GFraMe_pointer_y - chainball.obj.y);
        
        center_x = GFraMe_pointer_x;
        center_y = GFraMe_pointer_y;
    }
    chainball.obj.ax *= 0.8;
    chainball.obj.ay *= 0.8;
    if (!did_move && chainball.obj.ax < MAXA / 2) {
        int dist = chainball.obj.x - center_x;
        if (ABSI(dist) > MAXDIST && ABSI(chainball.obj.vx) < 50.0) {
            int sign = ABSI(dist) / dist;
            chainball.obj.ax = -sign*MAXA;
            chainball.obj.vx = -sign*MAXV;
        }
        else
            chainball.obj.vx *= 0.99;
    }
    if (!did_move && chainball.obj.ay < MAXA / 2) {
        int dist = chainball.obj.y - center_y;
        if (ABSI(dist) > MAXDIST && ABSI(chainball.obj.vy) < 50.0) {
            int sign = ABSI(dist) / dist;
            chainball.obj.ay = -sign*MAXA;
            chainball.obj.vy = -sign*MAXV;
        }
        else
            chainball.obj.vy *= 0.99;
    }
    
    if (chainball.obj.ax > MAXA) chainball.obj.ax = MAXA;
    else if (chainball.obj.ax < -MAXA) chainball.obj.ax = -MAXA;
    if (chainball.obj.ay > MAXA) chainball.obj.ay = MAXA;
    else if (chainball.obj.ay < -MAXA) chainball.obj.ay = -MAXA;
    if (chainball.obj.vx > MAXV) chainball.obj.vx = MAXV;
    else if (chainball.obj.vx < -MAXV) chainball.obj.vx = -MAXV;
    if (chainball.obj.vy > MAXV) chainball.obj.vy = MAXV;
    else if (chainball.obj.vy < -MAXV) chainball.obj.vy = -MAXV;
    
    GFraMe_sprite_update(&chainball, ms);
    
    dx = (float)(chainball.obj.x
        + chainball.obj.hitbox.cx/2 - center_x) / (float)chain_num;
    dy = (float)(chainball.obj.y
        + chainball.obj.hitbox.cy/2 - center_y) / (float)chain_num;
    i = 0;
    while (i < chain_num) {
        chain_lx[i] = chain_x[i];
        chain_ly[i] = chain_y[i];
        chain_x[i] = center_x + dx * i + 2;
        chain_y[i] = center_y + dy * i + 2;
        i++;
    }
}

void cb_draw() {
    int i;
    
    i = 0;
    while (i < chain_num) {
        int x, y;
        float alpha;
        
        alpha = i - chain_num / 2; // change interval
        if (alpha < 0) alpha = -alpha; // modulus
        //alpha = chain_num / 2 - alpha; // invert interval
        alpha /= chain_num / 2; // normalize
        alpha = CBALPHA * alpha;
        // TODO polish!!!!
        x = (int)(chain_x[i] * (1.0f - alpha) + chain_lx[i] * alpha);
        y = (int)(chain_y[i] * (1.0f - alpha) + chain_ly[i] * alpha);
        
        GFraMe_spriteset_draw
            (
            gl_sset4x4,
            0,
            x,
            y,
            0
            );
        
        chain_x[i] = x;
        chain_y[i] = y;
        
        i++;
    }
    GFraMe_sprite_draw(&chainball);
}

