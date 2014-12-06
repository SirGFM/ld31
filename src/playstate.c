/**
 * @file src/playstate.c
 */
#include <GFraMe/GFraMe_controller.h>
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_event.h>
#include <GFraMe/GFraMe_keys.h>
#include <GFraMe/GFraMe_sprite.h>
#include <GFraMe/GFraMe_spriteset.h>

GFraMe_event_setup();

#include "global.h"
#include "playstate.h"

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
    SPR_MAX
};

//GFraMe_sprite sprs[SPR_MAX];
static int pos;
static float fpos;

GFraMe_ret ps_init() {
    int i;
    GFraMe_ret rv = GFraMe_ret_ok;
    
//    i = 0;
//    while (i < SPR_MAX)
//        GFraMe_sprite(init(
    
    pos = 0;
    fpos = 0;
    
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
      if (GFraMe_controller_max > 0 && GFraMe_controllers[0].home)
        gl_running = 0;
    GFraMe_event_on_key_down();
      if (GFraMe_keys.esc)
        gl_running = 0;
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
    pos = (int)fpos;
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
    
    #define ps_draw_bg(tile, offx) \
      do { \
        GFraMe_spriteset_draw(gl_sset64x32, tile, pos + offx, 24, 0); \
        if (pos + offx + 40 - SCRW > 0) \
          GFraMe_spriteset_draw(gl_sset64x32, tile, pos + offx - SCRW, 24, 0); \
      } while (0)
    #define ps_draw_fl(tile, offx) \
      do { \
        GFraMe_spriteset_draw(gl_sset64x16, tile, pos + offx, 40, 0); \
        if (pos + offx + 46 - SCRW > 0) \
          GFraMe_spriteset_draw(gl_sset64x16, tile, pos + offx - SCRW, 40, 0); \
      } while (0)
    
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

