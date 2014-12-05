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
GFraMe_spriteset *gl_sset8;
GFraMe_spriteset *gl_sset4;

static int gl_isInit = 0;
static GFraMe_spriteset gl_sset8_st;
static GFraMe_spriteset gl_sset4_st;

GFraMe_texture gl_tex;

GFraMe_ret gl_init() {
    GFraMe_ret rv;
    unsigned char *data = NULL;
    
    GFraMe_texture_init(&gl_tex);
    
    rv = GFraMe_assets_buffer_image
        (
        ATLASFN,
        ATLASW,
        ATLASH,
        (char**)&data
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    rv = GFraMe_texture_load
        (
        &gl_tex,
        ATLASW,
        ATLASH,
        data
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    GFraMe_spriteset_init
        (
        &gl_sset4_st,
        &gl_tex,
        4,
        4
        );
    gl_sset4 = &gl_sset4_st;
    
    GFraMe_spriteset_init
        (
        &gl_sset8_st,
        &gl_tex,
        8,
        8
        );
    gl_sset8 = &gl_sset8_st;
    
    gl_isInit = 1;
__ret:
    if (data)
        free(data);
    
    return rv;
}

void gl_clean() {
    if (gl_isInit) {
        GFraMe_texture_clear(&gl_tex);
    }
}

