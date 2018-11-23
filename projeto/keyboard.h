#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

int kbd_subscribe(void);
int kbd_unsubscribe();
int kbd_handler(unsigned long *code2);

#endif
