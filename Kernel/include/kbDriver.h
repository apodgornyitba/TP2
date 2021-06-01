#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <defs.h>
#include <stdint.h>

#define BUFF_SIZE 256

void initKb();
void keyboardHandler(uint64_t rsp);
void loadKey(char c);
int dumpChar();
int dumpBuffer(char *destination, int size); // devuelve cuantos chars se pudieron dumpear

#endif 