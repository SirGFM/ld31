/**
 * @file src/playstate.c
 */
#include <GFraMe/GFraMe_event.h>
#include <GFraMe/GFraMe_keys.h>
#include <GFraMe/GFraMe_controller.h>

#include "global.h"
#include "player.h"
#include "playstate.h"

GFraMe_event_setup();

static int ps_init() {
    int rv;
    
    rv = pl_init();
    ASSERT(rv == 0);
    
    GFraMe_event_init(UFPS, DFPS);
    
    rv = 0;
__ret:
    return rv;
}

static void ps_clean() {
    pl_clean();
}

static void ps_event() {
    GFraMe_event_begin();
        GFraMe_event_on_timer();
        GFraMe_event_on_mouse_up();
        GFraMe_event_on_mouse_down();
        GFraMe_event_on_mouse_moved();
//        GFraMe_event_on_finger_down();
//        GFraMe_event_on_finger_up();
        GFraMe_event_on_key_down();
            if (GFraMe_keys.esc)
                gl_running = 0;
        GFraMe_event_on_key_up();
        GFraMe_event_on_controller();
            if (GFraMe_controller_max > 0 && GFraMe_controllers[0].home)
                gl_running = 0;
//        GFraMe_event_on_bg();
//        GFraMe_event_on_fg();
        GFraMe_event_on_quit();
            gl_running = 0;
    GFraMe_event_end();
}

static void ps_update() {
    GFraMe_event_update_begin();
        pl_update(GFraMe_event_elapsed);
    GFraMe_event_update_end();
}

static void ps_draw() {
    GFraMe_event_draw_begin();
        pl_draw();
    GFraMe_event_draw_end();
}

void playstate() {
    ASSERT(ps_init() == 0);
    while (gl_running) {
        ps_event();
        ps_update();
        ps_draw();
    }
__ret:
    ps_clean();
    return;
}

