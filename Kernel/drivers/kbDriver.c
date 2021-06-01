#include <kbDriver.h>
#include <lib.h>
#include <syscalls.h>

//Resource: https://www.cs.umd.edu/~hollings/cs412/s98/project/proj1/index.html
#define ESC 27 /* ASCII escape */
/* Scancodes for shift and capslock keys: */
#define LSHIFT_PRESSED 0x2A  /* Scan codes for shift press, */
#define LSHIFT_RELEASED 0xAA /* shift release and capslock keys */
#define RSHIFT_PRESSED 0x36 
#define RSHIFT_RELEASED 0xB6
#define CAPSLOCK 0x3A
#define CAPSLOCK_RELEASED 0xBA
#define CTRL 0x1D
#define BACKSPACE 8 /* Ascii codes for Backspace, Tab and enter keys.*/
#define TAB 9       
#define ENTER_KEY 13

#define TRUE 1
#define FALSE 0

char scanToAscii[58][2] = /* Array containing ascii codes for appropriate scan codes */
    {
        {0, 0},
        {ESC, ESC},
        {'1', '!'},
        {'2', '@'},
        {'3', '#'},
        {'4', '$'},
        {'5', '%'},
        {'6', '^'},
        {'7', '&'},
        {'8', '*'},
        {'9', '('},
        {'0', ')'},
        {'-', '_'},
        {'=', '+'},
        {8, 8},
        {9, 9},
        {'q', 'Q'},
        {'w', 'W'},
        {'e', 'E'},
        {'r', 'R'},
        {'t', 'T'},
        {'y', 'Y'},
        {'u', 'U'},
        {'i', 'I'},
        {'o', 'O'},
        {'p', 'P'},
        {'[', '{'},
        {']', '}'},
        {13, 13},
        {0, 0},
        {'a', 'A'},
        {'s', 'S'},
        {'d', 'D'},
        {'f', 'F'},
        {'g', 'G'},
        {'h', 'H'},
        {'j', 'J'},
        {'k', 'K'},
        {'l', 'L'},
        {';', ':'},
        {39, 34},
        {'`', '~'},
        {0, 0},
        {'\\', '|'},
        {'z', 'Z'},
        {'x', 'X'},
        {'c', 'C'},
        {'v', 'V'},
        {'b', 'B'},
        {'n', 'N'},
        {'m', 'M'},
        {',', '<'},
        {'.', '>'},
        {'/', '?'},
        {0, 0},
        {0, 0},
        {0, 0},
        {' ', ' '},
};

static char buffer[BUFF_SIZE] = {0};
static int rdIdx = 0; //Posicion de escritura
static int wrIdx = 0; //Posicion de lectura
static int activeSize = 0; //Elementos legibles en el buffer

static int isShifted = FALSE;
static int capsEnabled = FALSE;

void initKb(){
    isShifted = capsEnabled= FALSE;
    rdIdx = wrIdx = activeSize = 0;
}

void keyboardHandler(uint64_t rsp)
{
    if (getKey(0x64) != 0)
    {
        unsigned char keyCode = getKey(0x60); 

        if (keyCode==LSHIFT_PRESSED || keyCode==RSHIFT_PRESSED){
            isShifted=TRUE;
            return;
        }
        if (keyCode==LSHIFT_RELEASED || keyCode == RSHIFT_RELEASED){
            isShifted = FALSE;
            return;
        }
        if (keyCode == CAPSLOCK){
            capsEnabled = 1 - capsEnabled;
            return;
        }
        if (keyCode >= 58 || keyCode & 0X80) //No tiene representacion ascii y no es una tecla levantandose
            return;

        int shiftState =  isShifted == capsEnabled ? 0:1; //Uno solo esta encendido
        char c = scanToAscii[keyCode][shiftState];

        loadKey(c);

    }
}

void loadKey(char c){

    if (c == '\r') //Normal\izado de saltos de linea.
        c = '\n';

    if (c != 0)
    {
        buffer[wrIdx] = c;
        wrIdx = (wrIdx + 1) % BUFF_SIZE; //Ciclo circularmente por el buffer

        // Si no llene el buffer, aumento su tamaño, si lo llene, agrego igual pero pierdo su ultimo valor "lectura artificial"
        if (activeSize < BUFF_SIZE)
            activeSize++;
        else
            rdIdx = (rdIdx + 1) % BUFF_SIZE;
    }
}

int dumpChar()
{
    if (activeSize <= 0) 
        return -1;

    int c = buffer[rdIdx];
    rdIdx = (rdIdx + 1) % BUFF_SIZE;
    activeSize--;
    return c;
}

int dumpBuffer(char* destination, int size)
{
    //Solo lee los elementos pertinentes
    if (size<=0 || activeSize<=0)
        return -1;

    int idx=0;
    while (idx<size-1 && activeSize){
        destination[idx] = dumpChar();
        idx++;
    }
    destination[idx]=0;
    return idx;
}