#include <shell.h>
#include <lib.h>
#include <libc.h>

static const char *registers[] = {"RAX:", "RBX:", "RCX:", "RDX:", "RBP:", "RDI:", "RSI:", "R8 :", "R9 :", "R10:", "R11:", "R12:", "R13:", "R14:", "R15:"};

void getHelp();
void printMem(char *hexa);
void printRegisters();
void printDateTime();
void divExc();

void initialize() {
    scClear();
    printf("Que modulo desea correr? Para conocer los comandos habilitados, escriba HELP\n");
	printf("Presionar TAB y enter para cambiar de pantalla.\n");
}

void shellMain(char *command, char *param, int * esc){

        if (strcmp(command,"HELP") == 0)
            getHelp();
        else if (strcmp(command,"DATETIME")==0)
            printDateTime();
        else if(strcmp(command,"CLEAR")==0)
            scClear();
        else if (strcmp(command, "PRINTMEM") == 0)
        {
            if (param[0]=='0'&& param[1]=='x')
                printMem(param);
            else printf("Ingrese un parametro valido en hexa.\n");
        }
        else if (strcmp(command, "INFOREG") == 0)
            printRegisters();
        else if (strcmp(command,"DIVEX") == 0)
            divExc();
        else if(strcmp(command,"OPEX")==0)
            opCodeExc();
        else if (strcmp(command, "EXIT") == 0){
            *esc = 1;
            return;
        }
        else printf("Comando invalido.\n Escriba HELP para mas informacion.\n");  
    return;
}

void printMem(char* hexa){
    long long address = hexaToInt(hexa);
    unsigned char buff[33];
    char byte[10];
    getMem(buff,address,32);
    printf("\n");
    printf("Memoria en address %s :\n\n",hexa);
    for (int i=0;i<32;i++){
        if (i == 16)
            printf("\n");
        intToHexa((char)buff[i], byte, 1);
        printf("%s ", byte);
    }
    printf("\n");
}

void printRegisters(){
    unsigned long long buff[17];
    char hexa[20];
    printf("\n");
    getRegs(buff);
    for (int i = 0; i <= 14; i++){
        intToHexa((long long)buff[14-i],hexa,8);
        printf("%s",registers[i]);
        printf("%s\n",hexa);
    }
    printf("\n");
}

void getHelp() {
    printf("\n~ HELP: Informacion de comandos.\n");
    printf("~ DATETIME: Se imprime el tiempo y la fecha actual.\n");
    printf("~ CLEAR: Se limpia la pantalla.\n");
    printf("~ PRINTMEM: Volcado de memoria de 32 bytes desde la direccion dada.\n");
    printf("~ INFOREG: Se imprimen los valores de los registros.\n");
    printf("~ DIVEX: Para lanzar una excepcion por division por cero.\n");
    printf("~ OPEX: Para lanzar una excepción por operador de codigo invalido.\n");
    printf("~ EXIT: Abortar la ejecucion.\n");
    printf("\n");
}

void printDateTime() {
   printf("%d:%d:%d\n%d\\%d\\%d\n", DateTime(2), DateTime(1), DateTime(0),DateTime(3),DateTime(4),DateTime(5));
}



void divExc(){
    int a = 1, b = 0;
    a = a/b;
}
