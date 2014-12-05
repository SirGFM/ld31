/**
 * @file src/player.c
 */
#include <GFraMe/GFraMe_controller.h>
//#include <GFraMe/GFraMe_log.h>
#include <GFraMe/GFraMe_sprite.h>
#include <GFraMe/GFraMe_util.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "player.h"

static GFraMe_sprite pl;
// Best var evur
static GFraMe_sprite **rainbow = NULL;
static int r_len = 0;
static int r_max = 0;

static float cur_ang = 0.0f;
static float next_ang = 0.0f;
static int switch_time = 0;
static int next_switch = 500;

static GFraMe_sprite *pl_getRainbow();
static void pl_initRainbow(GFraMe_sprite *spr, int color, int x, int y, float vx, float vy);
static int pl_doubleRainbow();

int pl_init() {
	int i;
	int rv = 1;
    
    GFraMe_sprite_init
        (
        &pl,
        SCRW/2,
        SCRH/2,
        8,
        8,
        gl_sset8,
        0,
        0
        );
    pl.cur_tile = '*' - '!';
	
	r_max = 64;
	rainbow = (GFraMe_sprite**)malloc(sizeof(GFraMe_sprite*)*r_max);
	ASSERT(rainbow);
	
	r_len = 0;
	i = 0;
	while (i < r_max) {
        GFraMe_sprite *spr;
        
		spr = (GFraMe_sprite*)malloc(sizeof(GFraMe_sprite));
		ASSERT(spr);
        
        spr->is_active = 0;
        spr->is_visible = 0;
        
        rainbow[i] = spr;
        
		r_len++;
		i++;
	}
	
	rv = 0;
__ret:
	return rv;
}

void pl_clean() {
	int i;
	
	i = 0;
	while (i < r_len) free(rainbow[i++]);
	r_len = 0;
	
	if (rainbow) free(rainbow);
	rainbow = NULL;
}

void pl_update(int ms) {
	int i;
    float ang, vx, vy;
    GFraMe_sprite *spr;
    
    if (switch_time > 0) {
        switch_time -= ms;
        cur_ang += next_ang * ((float)ms / 1000.0f);
    }
    else {
        next_switch -= ms;
        if (next_switch <= 0) {
            next_switch += 100 * (10 + GFraMe_util_randomi() % 20);
//            GFraMe_new_log("time: %02.f\n", next_switch / 1000.0f);
            next_ang = (4*(GFraMe_util_randomi() % 90) - 180) * PI / 180.0f;
            next_ang -= cur_ang;
            switch_time += 500;
        }
    }
    while (cur_ang > 2*PI)
        cur_ang -= 2*PI;
    
    i = 0;
    ang = cur_ang;
    while (i < 7) {
        float c = (float)cos(ang);
        float s = (float)sin(ang);
        
        if (i == 4) {
            vx = -c*100;
            vy = -s*100;
        }
        
        spr = pl_getRainbow();
        pl_initRainbow
            (
            spr,
            i,
            pl.obj.x + pl.obj.hitbox.cx,
            pl.obj.y + pl.obj.hitbox.cy,
            c * 100,
            s * 100
            );
        
        ang += 2.0f * PI / 180.0f;
        i++;
    }
    
    if (GFraMe_controller_max > 0) {
        vx += GFraMe_controllers[0].lx * 150;
        vy += GFraMe_controllers[0].ly * 150;
    }
    pl.obj.vx = vx;
    pl.obj.vy = vy;
    
    GFraMe_sprite_update(&pl, ms);
        
    i = 0;
	while (i < r_len) {
		GFraMe_sprite *spr;
		
		spr = rainbow[i];
		if (spr->is_active) GFraMe_sprite_update(spr, ms);
		
		if (
			spr->obj.x < -32
			|| spr->obj.x > SCRW+32
			|| spr->obj.y < -32
			|| spr->obj.y > SCRW+32
			)
			spr->is_active = 0;
		
		i++;
	}
}

void pl_draw() {
	int i;
	
    i = 0;
	while (i < r_len) {
        GFraMe_sprite *spr;
        
		spr = rainbow[i];
		if (spr->is_visible) GFraMe_sprite_draw(spr);
		
		i++;
	}
    GFraMe_sprite_draw(&pl);
}

static GFraMe_sprite *pl_getRainbow() {
	GFraMe_sprite *spr = NULL;
	int i = 0;
	
	while (i < r_len) {
		if (!rainbow[i]->is_active)
			return rainbow[i];
		i++;
	}
	
	if (i >= r_max) ASSERT(pl_doubleRainbow() == 0);
	
	rainbow[i] = (GFraMe_sprite*)malloc(sizeof(GFraMe_sprite));
	ASSERT(rainbow[i]);
	r_len++;
    
	spr = rainbow[i];
__ret:
	return spr;
}

static void pl_initRainbow(GFraMe_sprite *spr, int color, int x, int y, float vx, float vy) {
    GFraMe_sprite_init
        (
        spr,
        x,
        y,
        4,
        4,
        gl_sset4,
        0,
        0
        );
    spr->obj.vx = vx;
    spr->obj.vy = vy;
    spr->cur_tile = RAINBOWTILE + color;
}

static int pl_doubleRainbow() {
	int rv = 1;
	GFraMe_sprite **tmp = NULL;
	
	tmp = (GFraMe_sprite**)malloc(sizeof(GFraMe_sprite*)*r_max*2);
	ASSERT(tmp);
	
    memset(tmp, 0x0, sizeof(GFraMe_sprite**)*r_max*2);
	memcpy(tmp, rainbow, sizeof(GFraMe_sprite**)*r_max);
	free(rainbow);
	rainbow = tmp;
	r_max *= 2;
	
	rv = 0;
__ret:
	return rv;
}

