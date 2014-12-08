/**
 * @file src/main.c
 */
#include <GFraMe/GFraMe.h>
#include <GFraMe/GFraMe_audio_player.h>
#include <GFraMe/GFraMe_controller.h>
#include <GFraMe/GFraMe_screen.h>

#include "global.h"
#include "playstate.h"

int main(int argc, char *argv[]) {
    GFraMe_ret rv;
    GFraMe_wndext ext;
    
    ext.atlas = TEX;
    ext.atlasWidth = TEXW;
    ext.atlasHeight = TEXH;
    ext.flags = GFraMe_wndext_scanline;
    
    rv = GFraMe_init
        (
        SCRW,
        SCRH,
        WNDW,
        WNDH,
        "com.wordpress.gfmgamecorner",
        TITLE,
        GFraMe_window_none,
        &ext,
        FPS,
        LOGTOFILE,
        0
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    GFraMe_controller_init(1);
    
    rv = GFraMe_audio_player_init();
    ASSERT(rv == GFraMe_ret_ok);
    
    rv = gl_init();
    ASSERT(rv == GFraMe_ret_ok);
    
    GFraMe_set_bg_color(0x00, 0x00, 0x00, 0xff);
    
#if !defined(DEBUG)
    GFraMe_audio_player_play_bgm(gl_bgm, 0.75);
#endif
    while (gl_running) {
        playstate();
    }
    
__ret:
    GFraMe_audio_player_pause();
    
    gl_clean();
    GFraMe_audio_player_clear();
    GFraMe_controller_close();
    GFraMe_quit();
    return rv;
}

