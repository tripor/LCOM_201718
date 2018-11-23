#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "data_storage.h"

mouse_data mouse_handler(unsigned long packet[], mouse_data dados);
unsigned long get_statreg();
unsigned long mouse_get_code();
int mouse_subscribe(void);
int mouse_unsubscribe();

#endif
