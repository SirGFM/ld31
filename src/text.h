/**
 * @file src/text.c
 */
#ifndef __TEXT_H_
#define __TEXT_H_

void txt_init();
void txt_upd(int ms);
void txt_draw();
void txt_set_text(char *text);
void txt_clear();
int txt_is_complete();

#endif

