/**
 * @file src/global.h
 */
#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_spriteset.h>

#define SCRW    160
#define SCRH    80
#define WNDW    480
#define WNDH    240
#define TITLE   "Snowman"
#define TEX     "atlas"
#define TEXW    256
#define TEXH    128
#define FPS     60
#define UPS     60  // updates per second
#define DPS     60  // draws per second
#define LOGTOFILE   0

#define SS4INI  0  // First valid (non char) tile on a 8x8 spriteset
#define TEXTINI 16
#define SS8INI  80  // First valid (non char) tile on a 8x8 spriteset
#define SS16INI 32  // First valid (non-char) tile on a 16x16 spriteset

#define ABSI(val) \
    ((val >= 0)?(val):(-(val)))

#define ASSERT(stmt) \
  do { \
    if (!(stmt)) \
        goto __ret; \
  } while (0)

extern int gl_running;
extern GFraMe_spriteset *gl_sset8x8;
extern GFraMe_spriteset *gl_sset16x8;
extern GFraMe_spriteset *gl_sset16x16;
extern GFraMe_spriteset *gl_sset64x16;
extern GFraMe_spriteset *gl_sset64x32;

GFraMe_ret gl_init();
void gl_clean();

#endif

