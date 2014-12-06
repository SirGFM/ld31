/**
 * @file src/global.c
 */
#include <GFraMe/GFraMe_assets.h>
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_spriteset.h>
#include <GFraMe/GFraMe_texture.h>

#include <stdio.h>
#include <stdlib.h>

#include "global.h"

int gl_running = 0;

static int is_init = 0;
static GFraMe_texture tex;
//static GFraMe_spriteset sset;

GFraMe_ret gl_init() {
    GFraMe_ret rv;
    unsigned char *data = NULL;
    
    rv = GFraMe_assets_buffer_image
        (
        TEX,
        TEXW,
        TEXH,
        (char**)&data
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    GFraMe_texture_init(&tex);
    rv = GFraMe_texture_load
        (
        &tex,
        TEXW,
        TEXH,
        data
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    gl_running = 1;
    is_init = 1;
    rv = GFraMe_ret_ok;
__ret:
    if (data)
        free(data);
    return rv;
}

void gl_clean() {
    if (is_init) {
        GFraMe_texture_clear(&tex);
    }
    is_init = 0;
}

