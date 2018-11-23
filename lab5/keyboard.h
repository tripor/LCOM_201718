#ifndef __KEYBOARD_H
#define __KEYBOARD_H

int counter_system(unsigned long port,unsigned long *byte);
int kbd_get_code(unsigned long *code2);
int kbd_subscribe(void);
int kbd_unsubscribe();
int kbd_handler();

int keyboard();

#endif /* __TEST5_H */
