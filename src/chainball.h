/**
 * @file src/chainball.h
 */
#ifndef __CHAINBALL_H_
#define __CHAINBALL_H_

#include <GFraMe/GFraMe_error.h>

GFraMe_ret cb_init(int num);
void cb_clean();
void cb_update(int ms, int did_move);
void cb_draw();

#endif

