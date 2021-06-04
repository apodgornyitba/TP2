#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H

void initVideo();
int changeScreen(int screen);
int getCurrentScreen();
int putChar(char c, unsigned int color);
void cleanScreen();
void newLine();
void backspace();
#endif