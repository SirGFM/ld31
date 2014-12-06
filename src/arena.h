/**
 * @file src/arena.h
 */
#ifndef __ARENA_H_
#define __ARENA_H_

#include <GFraMe/GFraMe_error.h>

GFraMe_ret ar_init();
void ar_clean();
void ar_update(int ms);
void ar_draw();

#endif

