#ifndef __HELP_H
#define __HELP_H



unsigned long get_statreg();
int mouse_subscribe(void);
int mouse_unsubscribe();
int mouse_write_cmd(unsigned long cmd);
unsigned long mouse_get_code();
int timer_subscribe_int(void);
int timer_unsubscribe_int();
unsigned long get_cmd_byte();
int write_cmd_byte(unsigned long cmd);

#endif
