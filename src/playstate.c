/**
 * @file src/playstate.c
 */
#include <GFraMe/GFraMe_animation.h>
#include <GFraMe/GFraMe_audio_player.h>
#include <GFraMe/GFraMe_controller.h>
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_event.h>
#include <GFraMe/GFraMe_log.h>
#include <GFraMe/GFraMe_keys.h>
#include <GFraMe/GFraMe_sprite.h>
#include <GFraMe/GFraMe_spriteset.h>
#include <GFraMe/GFraMe_util.h>

GFraMe_event_setup();

#include "clouds.h"
#include "global.h"
#include "particles.h"
#include "playstate.h"
#include "text.h"
#include "upper_text.h"

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
    kid,
    particles,
    clouds,
    SPR_MAX
};

static float speed;
static int pos;
static float fpos;
static int new_snow;
static int new_leaf;
static int state;
static int facing_left;
static int do_change_state;
static int wait_for_input;
static int snow_harsher;

static int rise[6] = {37,36,35,34,33,32};
static int down[6] = {32,33,34,35,36,37};
static int jump[2] = {34,33};
static int fall[2] = {34,32};

static GFraMe_animation sm_rise;
static GFraMe_animation sm_down;
static GFraMe_animation sm_jump;
static GFraMe_animation sm_fall;
static GFraMe_animation *sm_cur;

static int enable_movement;
static int sm_frame;
static int sm_invert;
static int sm_speed;
static int sm_pos;
static float sm_fpos;

static float kid_pos;
static int kid_time;
static int kid_frame;

GFraMe_ret ps_init() {
    int i;
    GFraMe_ret rv = GFraMe_ret_ok;
    
    rv = p_init();
    ASSERT(rv == GFraMe_ret_ok);
    rv = c_init(128);
    ASSERT(rv == GFraMe_ret_ok);
    
    GFraMe_animation_init
        (
        /* anim */&sm_rise,
        /* fps  */4,
        /*frames*/rise,
        /*num_fr*/sizeof(rise)/sizeof(int),
        /*loop? */0
        );
    GFraMe_animation_init
        (
        /* anim */&sm_down,
        /* fps  */4,
        /*frames*/down,
        /*num_fr*/sizeof(down)/sizeof(int),
        /*loop? */0
        );
    GFraMe_animation_init
        (
        /* anim */&sm_jump,
        /* fps  */8,
        /*frames*/jump,
        /*num_fr*/sizeof(jump)/sizeof(int),
        /*loop? */0
        );
    GFraMe_animation_init
        (
        /* anim */&sm_fall,
        /* fps  */8,
        /*frames*/fall,
        /*num_fr*/sizeof(fall)/sizeof(int),
        /*loop? */0
        );
    
    sm_cur = 0;
    sm_frame = sm_rise.frames[0];
    sm_invert = 0;
    sm_pos = 0;
    sm_fpos = 0;
    
    kid_pos = 0;
    kid_time = 250;
    kid_frame = 41;
    
    txt_init();
    utxt_init();
    
#if !defined(GFRAME_MOBILE)
    utxt_set_text("  COLD REMEMBRANCE   -- PRESS ANY KEY --");
#else
    utxt_set_text("  COLD REMEMBRANCE  -- TOUCH TO START --");
#endif
    
    pos = 0;
    fpos = 0;
    new_snow = 0;
    new_leaf = 0;
    state = -1;
    enable_movement = 0;
    facing_left = 0;
    do_change_state = 1;
    wait_for_input = 0;
    snow_harsher = 0;
    
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
      if (wait_for_input)
        enable_movement = 1;
      
      if (GFraMe_controller_max > 0) {
        if (GFraMe_controllers[0].home)
          gl_running = 0;
        else if (state == -1 && utxt_is_complete()
          && (GFraMe_controllers[0].left || GFraMe_controllers[0].lx < -0.3)) {
          state = 0;
          utxt_set_text("                                        ");
        }
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
      // Make the game wait for the first input after 'reviving' the snowman
      if (wait_for_input)
        enable_movement = 1;
      
      if (GFraMe_keys.esc)
        gl_running = 0;
      else if (state == -1 && utxt_is_complete()) {
        state = 0;
        utxt_set_text("                                        ");
      }
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

static void ps_move_world();
static void ps_do_particles();
static void ps_do_anim();
static void ps_do_tip();
static void ps_do_text();
static void ps_do_sm();

void ps_update() {
  int skip;
  
  skip = 0;
  if (GFraMe_keys.one)
    skip += 1;
  if (GFraMe_keys.two)
    skip += 2;
  if (GFraMe_keys.three)
    skip += 3;

  GFraMe_event_update_begin();
    do {
      ps_do_anim();
      ps_move_world();
      ps_do_particles();
      ps_do_tip();
      ps_do_text();
      if (enable_movement)
        ps_do_sm();
      if (state == 10) {
         if (kid_pos < 22)
            kid_pos += 10 * GFraMe_event_elapsed / 1000.0f;
         else {
            state++;
            kid_time = 250;
         }
      }
      else if (state == 11) {
            if (kid_time > 0)
                kid_time -= GFraMe_event_elapsed;
            else {
            #if !defined(DEBUG)
                GFraMe_audio_player_push(gl_melt, 0.3);
            #endif
                sm_frame++;
                if (sm_frame == 38)
                    state = 13;
                else
                    state++;
                kid_time = 250;
            }
      }
      else if (state == 12) {
            if (kid_time > 0)
                kid_time -= GFraMe_event_elapsed;
            else {
            #if !defined(DEBUG)
                GFraMe_audio_player_push(gl_melt, 0.3);
            #endif
                sm_frame++;
                if (sm_frame == 38)
                    state = 13;
                else
                    state--;
                kid_time = 250;
           }
      }
      else if (state == 13) {
         if (kid_pos > -18)
            kid_pos -= 10 * GFraMe_event_elapsed / 1000.0f;
         else {
            do_change_state |= 1;
            state++;
         }
      }
      // Animate kids movement)
      if (state == 10 || state == 13) {
        if (kid_time > 0) {
            kid_time -= GFraMe_event_elapsed;
        }
        else {
            if (kid_frame == 41)
                kid_frame = 42;
            else
                kid_frame = 41;
            kid_time += 125;
        }
      }
    } while (skip-- > 0);
  GFraMe_event_update_end();
}

void ps_render(GFraMe_spriteset *ss, int tile, int offx, int y, int w) {
    GFraMe_spriteset_draw(ss, tile, pos + offx, y, facing_left);
    if (pos + offx + w - SCRW >= 0)
        GFraMe_spriteset_draw(ss, tile, pos + offx - SCRW, y, facing_left);
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
            //ps_render(gl_sset16x8, 1, 112, 37, 16);
        break;
        case fl_autumn:
            ps_render(gl_sset64x16, 15, -40, 40, 46);
            ps_render(gl_sset64x16, 15, 120, 40, 46);
        break;
        case snowman: {
            int tmp;
            
            //GFraMe_new_log("%i", sm_pos - pos);
            tmp = pos;
            pos = sm_pos;
            
            if (sm_invert)
                facing_left = 1;
            ps_render(gl_sset16x16, sm_frame, 20/*pos*/, 32/*y*/, 16/*w*/);
            facing_left = 0;
            
            pos = tmp;
        }
        break;
        case kid:
            if (state == 10) {
                ps_render(gl_sset16x16, kid_frame, kid_pos/*pos*/, 32/*y*/, 2*SCRW+16/*w*/);
            }
            else if (state == 11) {
                ps_render(gl_sset16x16, 39, kid_pos/*pos*/, 32/*y*/, 2*SCRW+16/*w*/);
            }
            else if (state == 12) {
                ps_render(gl_sset16x16, 40, kid_pos/*pos*/, 32/*y*/, 2*SCRW+16/*w*/);
            }
            else if (state == 13) {
                facing_left = 1;
                ps_render(gl_sset16x16, kid_frame, kid_pos/*pos*/, 32/*y*/, 2*SCRW+16/*w*/);
                facing_left = 0;
            }
        break;
        case particles:
            p_draw(pos);
        break;
        case clouds:
            c_draw(pos);
        break;
        default: {}
      }
      i++;
    }
    
    i = 0;
    while (i < SCRW/8) {
        GFraMe_spriteset_draw(gl_sset8x8, 4, i*8, 16, 0);
        GFraMe_spriteset_draw(gl_sset8x8, 5, i*8, 56, 0);
        i++;
    }
    
    txt_draw();
    utxt_draw();
  GFraMe_event_draw_end();
}

void ps_clean() {
    p_clear();
    c_clear();
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

static void ps_move_world() {
    int last;
    
    if (speed != 0)
        fpos += speed * GFraMe_event_elapsed / 1000.0f;
    if (fpos > SCRW)
        fpos -= SCRW;
    else if (fpos < 0)
        fpos += SCRW;
    pos = (int)fpos;
    
    last = sm_pos;
    if (sm_speed != 0)
        sm_fpos += sm_speed * GFraMe_event_elapsed / 1000.0f;
    if (sm_fpos > SCRW)
        sm_fpos -= SCRW;
    else if (sm_fpos < 0) {
        sm_fpos += SCRW;
        do_change_state |= 1;
    }
    sm_pos = (int)sm_fpos;
    
}

static void ps_do_particles() {
    // TODO spawn particles
    new_snow -= GFraMe_event_elapsed;
    if (new_snow <= 0) {
        int vx = 0;
        int speed = 0;
        if (snow_harsher) {
            new_snow += 20 + 5 * (1 + GFraMe_util_randomi() % 10);
            vx = 5;
            speed = -1;
        }
        else
            new_snow += 50 + 5 * (1 + GFraMe_util_randomi() % 20);
        p_spawn
            (
            /*y*/23,
            /*offx*/8 + (GFraMe_util_randomi() % 38),
            /*vx*/vx,
            /*tile*/0,
            /*speed*/speed
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
    c_update(GFraMe_event_elapsed);
}

static void ps_do_anim() {
    if (sm_cur) {
        GFraMe_ret rv;
        rv = GFraMe_animation_update(sm_cur, GFraMe_event_elapsed);
        
        if (rv == GFraMe_ret_anim_new_frame && sm_cur->index < sm_cur->num_frames) {
            sm_frame = sm_cur->frames[sm_cur->index];
            if (sm_cur == &sm_rise) {
            #if !defined(DEBUG)
                GFraMe_audio_player_push(gl_build, 0.3);
            #endif
            }
        }
        else if (rv == GFraMe_ret_anim_finished) {
            if (state == 4) {
                wait_for_input = 1;
            }
            if (sm_speed != 0 && sm_cur == &sm_jump) {
                sm_cur = &sm_fall;
                sm_frame = sm_cur->frames[0];
            }
            else {
                sm_cur->num_finished = 0;
                sm_cur->index = 0;
	            sm_cur->acc = sm_cur->frame_duration;
                sm_cur = 0;
            }
        }
    }
    else if (sm_speed != 0) {
        sm_cur = &sm_jump;
        sm_frame = sm_cur->frames[0];
    #if !defined(DEBUG)
        double delta = (GFraMe_util_randomi() % 11) * 0.0025;
        GFraMe_audio_player_push(gl_smwalk, 0.3 - delta);
    #endif
    }
}

static void ps_do_tip() {
    utxt_upd(GFraMe_event_elapsed);
    if (utxt_is_complete()) {
        if (state == 5 && enable_movement) {
            utxt_set_text("                                        ");
        }
        else if (state == 4 && wait_for_input) {
        #if !defined(GFRAME_MOBILE)
            utxt_set_text("PRESS 'LEFT' TO     MOVE BACK");
        #else
            utxt_set_text("TOUCH TOWARD THE    LEFT TO MOVE BACK");
        #endif
        }
    }
}

static void ps_do_text() {
    if (state == -1)
        return;
    
    txt_upd(GFraMe_event_elapsed);
    
    if (txt_is_complete())
      do_change_state |= 2;
    if (do_change_state == 3 || state == -2) {
      do_change_state = 0;
      switch (state) {
        case -2:
            txt_set_text("BUT THE SEASONS     CONTINUED TO CHANGE");
            state = -3;
            break;
        case 0:
            txt_set_text("IN THE END...");
            do_change_state = 1;
            state++;
            break;
        case 1:
            txt_set_text("I COULDN'T MEET HIM AGAIN...");
            do_change_state = 1;
            state++;
            break;
        case 2:
            txt_set_text("IT'S BEEN SO LONG...");
            do_change_state = 1;
            state++;
            break;
        case 3:
            txt_set_text("I WENT TO MANY      PLACES");
            sm_cur = &sm_rise;
            state++;
            break;
        case 4:
            txt_set_text("DID MY BEST TO KEEP FROM MELTING");
            state++;
            break;
        case 5:
            txt_set_text("THOSE MANY WINTERS  WERE HARSH, AT TIMES");
            snow_harsher = 1;
            state++;
            break;
        case 6:
            txt_set_text("ANY MISSTEP AND I   WOULD BE NO MORE");
            state++;
            break;
        case 7:
            txt_set_text("BUT EVEN HERE,      WHENCE I CAME FROM");
            state++;
            break;
        case 8:
            txt_set_text("I FOUND NOTHING...");
            state++;
            break;
        case 9:
            txt_set_text("AND COULDN'T HANDLE ANYMORE");
            //sm_cur = &sm_down;
            snow_harsher = 0;
            state++;
            break;
        case 10:
            txt_set_text("                                        ");
            break;
        case 14:
            txt_set_text("        THE                  END        ");
            break;
        default: {}
      }
    }
    
}

static void ps_do_sm() {
    int dif;
    
    sm_speed = 0;
    if (state <= -2) {
        return;
    }
    
    // FIXED!!! BOOOYA!!
    dif = (SCRW + sm_pos - pos) % SCRW;
    if (dif < 134 && dif > 28) {
        state = -2;
        sm_cur = &sm_down;
        return;
    }
    
    if (state >= 10) {
        speed = 0;
        return;
    }
    
    if (GFraMe_keys.a
        || GFraMe_keys.left
        || GFraMe_keys.h
        || (GFraMe_controller_max > 0
        && (
            GFraMe_controllers[0].left
         || GFraMe_controllers[0].lx < -0.3
           ))
       )
    {
        sm_invert = 1;
        speed = -SCRW/10;
        sm_speed = -SCRW/10;
    }
    else if (GFraMe_keys.d
        || GFraMe_keys.right
        || GFraMe_keys.l
        || (GFraMe_controller_max > 0
        && (
            GFraMe_controllers[0].right
         || GFraMe_controllers[0].lx > 0.3
           ))
       )
    {
        sm_invert = 0;
        speed = SCRW/10;
        sm_speed = SCRW/8;
    }
    else {
        speed = SCRW/10;
        sm_cur = 0;
        sm_frame = 32;
    }
}

