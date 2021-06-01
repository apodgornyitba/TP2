/* sampleCodeModule.c */
#include <libc.h>
#include <shell.h>
#include <lib.h>

int main() {
	scClear();
    printf("Que modulo desea correr? Para conocer los comandos habilitados, escriba HELP\n");
	shellMain();
	scClear();
	return 0;
}