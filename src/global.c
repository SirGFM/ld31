/**
 * @file src/global.c
 */
#include <GFraMe/GFraMe_assets.h>
#include <GFraMe/GFraMe_audio.h>
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_spriteset.h>
#include <GFraMe/GFraMe_texture.h>

#include <stdio.h>
#include <stdlib.h>

#include "global.h"

GFraMe_audio *gl_bgm;
GFraMe_audio bgm;
GFraMe_audio *gl_typing;
GFraMe_audio typing;
GFraMe_audio *gl_melt;
GFraMe_audio melt;
GFraMe_audio *gl_build;
GFraMe_audio build;
GFraMe_audio *gl_smwalk;
GFraMe_audio smwalk;

#define DEF_SPRSET(W, H) \
  GFraMe_spriteset *gl_sset##W##x##H; \
  static GFraMe_spriteset sset##W##x##H

int gl_running = 0;
DEF_SPRSET(4, 4);
DEF_SPRSET(8, 4);
DEF_SPRSET(8, 8);
DEF_SPRSET(16, 8);
DEF_SPRSET(16, 16);
DEF_SPRSET(64, 16);
DEF_SPRSET(64, 32);

static int is_init = 0;
static GFraMe_texture tex;

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
    
    gl_bgm = &bgm;
    rv = GFraMe_audio_init
        (
        gl_bgm,
        "song",
        1,      // loop?
        0,      // looppos
        0       // stereo?
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    gl_typing = &typing;
    rv = GFraMe_audio_init
        (
        gl_typing,
        "typewriter",
        0,      // loop?
        0,      // looppos
        0       // stereo?
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    gl_melt = &melt;
    rv = GFraMe_audio_init
        (
        gl_melt,
        "disassemble",
        0,      // loop?
        0,      // looppos
        0       // stereo?
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    gl_build = &build;
    rv = GFraMe_audio_init
        (
        gl_build,
        "build",
        0,      // loop?
        0,      // looppos
        0       // stereo?
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    gl_smwalk = &smwalk;
    rv = GFraMe_audio_init
        (
        gl_smwalk,
        "sm_walk",
        0,      // loop?
        0,      // looppos
        0       // stereo?
        );
    ASSERT(rv == GFraMe_ret_ok);
    
  #define INIT_SPRSET(W, H) \
    gl_sset##W##x##H = &sset##W##x##H; \
    GFraMe_spriteset_init \
        ( \
        gl_sset##W##x##H, \
        &tex, \
        W, \
        H \
        )
    
    INIT_SPRSET(4, 4);
    INIT_SPRSET(8, 4);
    INIT_SPRSET(8, 8);
    INIT_SPRSET(16, 8);
    INIT_SPRSET(16, 16);
    INIT_SPRSET(64, 16);
    INIT_SPRSET(64, 32);
    
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
        GFraMe_audio_clear(&bgm);
        GFraMe_audio_clear(&typing);
        GFraMe_audio_clear(&melt);
        GFraMe_audio_clear(&build);
        GFraMe_audio_clear(&smwalk);
    }
    is_init = 0;
}

