/**
 * @file src/main.c
 */

#include <GFraMe/GFraMe.h>
#include <GFraMe/GFraMe_audio_player.h>
#include <GFraMe/GFraMe_controller.h>
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_screen.h>

#include "global.h"
#include "playstate.h"

int main(int argc, char* argv[]) {
    GFraMe_ret rv;
    GFraMe_wndext ext;
    
    ext.atlas = ATLASFN;
    ext.atlasWidth = ATLASW;
    ext.atlasHeight = ATLASH;
    ext.flags = GFraMe_wndext_none;
    
    rv = GFraMe_init
        (
        SCRW,
        SCRH,
        WNDW,
        WNDH,
        "com.wordpress.gfmgamecorner.tametherainbow",
        "TameTheRainbow",
        GFraMe_window_none,
        &ext,
        FPS,
        LOGTOFILE,
        0
        );
    ASSERT(rv == GFraMe_ret_ok);
    
    rv = GFraMe_audio_player_init();
    GFraMe_controller_init(1);
    
    rv = gl_init();
    ASSERT(rv == GFraMe_ret_ok);
    
    GFraMe_audio_player_play_bgm(gl_bgm, 1.0f);
    gl_running = 1;
    while (gl_running) {
        playstate();
    }
    
__ret:
    GFraMe_audio_player_pause();
    GFraMe_audio_player_clear();
    gl_clean();
    GFraMe_controller_close();
    GFraMe_quit();
    return rv;
}

