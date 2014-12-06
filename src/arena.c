/**
 * @file src/arena.c
 */
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_tilemap.h>

#include "arena.h"
#include "global.h"

static GFraMe_tilemap arena;
static char ar_data[ARL];

GFraMe_ret ar_init() {
    GFraMe_ret rv;
    int i;
    
    rv = GFraMe_tilemap_init
        (
        &arena,
        ARW,
        ARH,
        ar_data,
        gl_sset16x16,
        NULL,
        0
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    i = 0;
    while (i < ARL) {
        ar_data[i] = SS16INI;
        i++;
    }
    
__ret:
    return rv;
}

void ar_update(int ms) {
}

void ar_draw() {
    GFraMe_tilemap_draw(&arena);
}

void ar_clean() {
    GFraMe_tilemap_clear(&arena);
}

