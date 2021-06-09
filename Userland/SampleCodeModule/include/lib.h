#ifndef LIB_H
#define LIB_H

#define TIMER_STOP 0
#define TIMER_START 1

int write(int fd, char* buffer, int length);
int read(int fd, char *buffer, int length);
void scClear();
int DateTime(int selector);
int getMem(unsigned char *buff, unsigned long long address, int bytes);
int getRegs(unsigned long long* arr);
int changeScreen(int screen);
void opCodeExc();

#endif