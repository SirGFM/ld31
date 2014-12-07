/**
 * @file src/text.c
 */
#include <GFraMe/GFraMe_tilemap.h>

#include <string.h>

#include "global.h"
#include "upper_text.h"

static GFraMe_tilemap utxt;

static char utxt_arr[20*2];
static char utxt_bb[50]; // to avoid overflows

static int is_complete;
static int pos;
static int time;

void utxt_init() {
    GFraMe_tilemap_init
        (
        &utxt,
        /*w in tiles*/20,
        /*h in tiles*/2,
        /*data*/utxt_arr,
        gl_sset8x8,
        /*colideable*/NULL,
        /*colideable len*/0
        );
    
    utxt.y = 3;
    memset(utxt_arr, 0x0, 40);
    
    is_complete = 1;
    time = 0;
    pos = 40;
}

void utxt_upd(int ms) {
    if (time > 0)
        time -= ms;
    else if (pos < 40) {
        utxt_arr[pos] = utxt_bb[pos];
        pos++;
        
        // TODO RNG and SFX
        
        if (pos == 40) {
            is_complete = 1;
            time += 300;
        }
        else
            time += 90;
    }
}

void utxt_draw() {
    GFraMe_tilemap_draw(&utxt);
}

void utxt_set_text(char *text) {
    char *tmp;
    
    memset(utxt_bb, 0x0, 40);
    
    tmp = utxt_bb;
    while (*text) {
        *tmp = *text - '!';
        tmp++;
        text++;
    }
    
    pos = 0;
    is_complete = 0;
    time = 0;
}

void utxt_clear() {
    memset(utxt_arr, 0x0, 40);
}

int utxt_is_complete() {
    return is_complete && time <= 0;
}

