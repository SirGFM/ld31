/**
 * @file src/global.h
 */
#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <GFraMe/GFraMe_audio.h>
#include <GFraMe/GFraMe_error.h>
#include <GFraMe/GFraMe_spriteset.h>

#define SCRW    320
#define SCRH    240
#define WNDW    640
#define WNDH    480
#define FPS     60
#define UFPS    60
#define DFPS    60
#define LOGTOFILE   0
#define ATLASFN "atlas"
#define ATLASW  128
#define ATLASH  128
#define RAINBOWTILE 256
#define PI  3.1514f

#define ASSERT(stmt) \
  do { \
    if (!(stmt)) \
        goto __ret; \
  } while(0)

extern int gl_running;

extern GFraMe_spriteset *gl_sset4;
extern GFraMe_spriteset *gl_sset8;
extern GFraMe_audio *gl_bgm;

GFraMe_ret gl_init();
void gl_clean();

#endif

