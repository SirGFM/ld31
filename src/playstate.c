/**
 * @file src/playstate.c
 */
#include <GFraMe/GFraMe_controller.h>
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_event.h>
#include <GFraMe/GFraMe_keys.h>

GFraMe_event_setup();

#include "arena.h"
#include "chainball.h"
#include "global.h"
#include "playstate.h"

static int did_move;

GFraMe_ret ps_init() {
    GFraMe_ret rv = GFraMe_ret_ok;
    
    //rv = ar_init();
    //ASSERT(rv == GFraMe_ret_ok);
    rv = cb_init(32);
    ASSERT(rv == GFraMe_ret_ok);
    
    did_move = 0;
    
    GFraMe_event_init(UPS, DPS);
__ret:
    return rv;
}

void ps_event() {
  GFraMe_event_begin();
    GFraMe_event_on_timer();
//    GFraMe_event_on_mouse_up();
//    GFraMe_event_on_mouse_down();
    GFraMe_event_on_mouse_moved();
      did_move = 1;
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
    //ar_update(GFraMe_event_elapsed);
    cb_update(GFraMe_event_elapsed, did_move);
  GFraMe_event_update_end();
  did_move = 0;
}

void ps_draw() {
  GFraMe_event_draw_begin();
    //ar_draw();
    cb_draw();
  GFraMe_event_draw_end();
}

void ps_clean() {
    // ar_clean();
    cb_clean();
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

