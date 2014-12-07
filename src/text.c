/**
 * @file src/text.c
 */
#include <GFraMe/GFraMe_tilemap.h>

#include <string.h>

#include "global.h"
#include "text.h"

static GFraMe_tilemap txt;

static char txt_arr[20*2];
static char txt_bb[50]; // to avoid overflows

static int is_complete;
static int pos;
static int time;

void txt_init() {
    GFraMe_tilemap_init
        (
        &txt,
        /*w in tiles*/20,
        /*h in tiles*/2,
        /*data*/txt_arr,
        gl_sset8x8,
        /*colideable*/NULL,
        /*colideable len*/0
        );
    
    txt.y = 61;
    memset(txt_arr, 0x0, 40);
    
    is_complete = 1;
    time = 0;
    pos = 40;
}

void txt_upd(int ms) {
    if (time > 0)
        time -= ms;
    else if (pos < 40) {
        txt_arr[pos] = txt_bb[pos];
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

void txt_draw() {
    GFraMe_tilemap_draw(&txt);
}

void txt_set_text(char *text) {
    char *tmp;
    
    memset(txt_bb, 0x0, 40);
    
    tmp = txt_bb;
    while (*text) {
        *tmp = *text - '!';
        tmp++;
        text++;
    }
    
    pos = 0;
    is_complete = 0;
    time = 0;
}

void txt_clear() {
    memset(txt_arr, 0x0, 40);
}

int txt_is_complete() {
    return is_complete && time <= 0;
}

