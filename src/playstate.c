/**
 * @file src/playstate.c
 */
#include <GFraMe/GFraMe_controller.h>
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_event.h>
#include <GFraMe/GFraMe_keys.h>
#include <GFraMe/GFraMe_sprite.h>
#include <GFraMe/GFraMe_spriteset.h>
#include <GFraMe/GFraMe_util.h>

GFraMe_event_setup();

#include "global.h"
#include "particles.h"
#include "playstate.h"

static int is_song_playing = 1;

enum {
    bg_autumn,
    bg_winter,
    bg_spring,
    bg_summer,
    fl_autumn,
    fl_winter,
    fl_spring,
    fl_summer,
    snowman,
    particles,
    SPR_MAX
};

static int pos;
static float fpos;
static int new_snow;
static int new_leaf;

GFraMe_ret ps_init() {
    int i;
    GFraMe_ret rv = GFraMe_ret_ok;
    
    rv = p_init();
    
    pos = 0;
    fpos = 0;
    new_snow = 0;
    new_leaf = 0;
    
    GFraMe_event_init(UPS, DPS);
__ret:
    return rv;
}

void ps_event() {
  GFraMe_event_begin();
    GFraMe_event_on_timer();
//    GFraMe_event_on_mouse_up();
//    GFraMe_event_on_mouse_down();
//    GFraMe_event_on_mouse_moved();
//    GFraMe_event_on_finger_down();
//    GFraMe_event_on_finger_up();
    GFraMe_event_on_controller();
      if (GFraMe_controller_max > 0) {
        if (GFraMe_controllers[0].home)
          gl_running = 0;
        else if (GFraMe_controllers[0].select) {
          if (is_song_playing) {
            GFraMe_audio_player_pause();
            is_song_playing = 0;
          }
          else {
            GFraMe_audio_player_play();
            is_song_playing = 1;
          }
        }
      }
    GFraMe_event_on_key_down();
      if (GFraMe_keys.esc)
        gl_running = 0;
      else if (GFraMe_keys.m || GFraMe_keys.zero || GFraMe_keys.n0) {
        if (is_song_playing) {
          GFraMe_audio_player_pause();
          is_song_playing = 0;
        }
        else {
          GFraMe_audio_player_play();
          is_song_playing = 1;
        }
      }
    GFraMe_event_on_key_up();
    GFraMe_event_on_quit();
      gl_running = 0;
  GFraMe_event_end();
}

void ps_update() {
  GFraMe_event_update_begin();
    fpos += SCRW/10 * GFraMe_event_elapsed / 1000.0f;
    if (fpos > SCRW)
        fpos -= SCRW;
    else if (fpos < 0)
        fpos += SCRW;
    pos = (int)fpos;
    
    // TODO spawn particles
    new_snow -= GFraMe_event_elapsed;
    if (new_snow <= 0) {
        new_snow += 50 + 5 * (1 + GFraMe_util_randomi() % 20);
        p_spawn
            (
            /*y*/23,
            /*offx*/8 + (GFraMe_util_randomi() % 38),
            /*vx*/0,
            /*tile*/0,
            /*speed*/0
            );
    }
    new_leaf -= GFraMe_event_elapsed;
    if (new_leaf <= 0) {
        int tile = 1 + GFraMe_util_randomi()%2;
        new_leaf += 70 + 30 * (1 + GFraMe_util_randomi() % 20);
        p_spawn
            (
            /*y*/23,
            /*offx*/128 + (GFraMe_util_randomi() % 28),
            /*vx*/0,
            /*tile*/tile,
            /*speed*/1
            );
    }
    
    p_update(GFraMe_event_elapsed);
  GFraMe_event_update_end();
}

void ps_render(GFraMe_spriteset *ss, int tile, int offx, int y, int w) {
    GFraMe_spriteset_draw(ss, tile, pos + offx, y, 0);
    if (pos + offx + w - SCRW >= 0)
        GFraMe_spriteset_draw(ss, tile, pos + offx - SCRW, y, 0);
}

void ps_draw() {
  GFraMe_event_draw_begin();
    int i;
    
    i = 0;
    while (i < SPR_MAX) {
      switch (i) {
        case bg_winter:
            ps_render(gl_sset64x32, 8, 6, 24, 40);
        break;
        case bg_spring:
            ps_render(gl_sset64x32, 9, 46, 24, 40);
        break;
        case bg_summer:
            ps_render(gl_sset64x32, 10, 86, 24, 40);
        break;
        case bg_autumn:
            ps_render(gl_sset64x32, 11, -34, 24, 40);
            ps_render(gl_sset64x32, 11, 126, 24, 40);
        break;
        case fl_winter:
            ps_render(gl_sset64x16, 12, 0, 40, 46);
        break;
        case fl_spring:
            ps_render(gl_sset64x16, 13, 40, 40, 46);
        break;
        case fl_summer:
            ps_render(gl_sset64x16, 14, 80, 40, 46);
            ps_render(gl_sset16x8, 32, 112, 37, 16);
        break;
        case fl_autumn:
            ps_render(gl_sset64x16, 15, -40, 40, 46);
            ps_render(gl_sset64x16, 15, 120, 40, 46);
        break;
        case snowman:
            ps_render(gl_sset16x16, 32, 20/*pos*/, 32, 16);
        break;
        case particles:
            p_draw(pos);
        break;
        default: {}
      }
      i++;
    }
    
    i = 0;
    while (i < SCRW/8) {
        GFraMe_spriteset_draw(gl_sset8x8, 66, i*8, 16, 0);
        GFraMe_spriteset_draw(gl_sset8x8, 67, i*8, 56, 0);
        i++;
    }
  GFraMe_event_draw_end();
}

void ps_clean() {
    p_clear();
}

void playstate() {
    GFraMe_ret rv;
    
    rv = ps_init();
    ASSERT(rv == GFraMe_ret_ok);
    
    while (gl_running) {
        ps_event();
        ps_update();
        ps_draw();
    }
__ret:
    ps_clean();
    return;
}

