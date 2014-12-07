/**
 * @file src/text.c
 */
#ifndef __UPPERTEXT_H_
#define __UPPERTEXT_H_

void utxt_init();
void utxt_upd(int ms);
void utxt_draw();
void utxt_set_text(char *text);
void utxt_clear();
int utxt_is_complete();

#endif

