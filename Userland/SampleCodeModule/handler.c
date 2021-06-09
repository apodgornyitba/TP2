#include <libc.h>
#include <shell.h>
#include <lib.h>

static int currentScreen = 1;
static int esc = 0;

#define LEFT 1
#define RIGHT 2

void handler(){

	changeScreen(RIGHT);
	initialize();
	changeScreen(LEFT);
	initialize();

    char command[120] = {0};
    char param[120] = {0};

    while(!esc){

        command[0]=0;
        param[0]=0;
        printf("$ > ");
        scanf("%s %s",command,param);
        
        if(command[0] == '\t'){
            if (currentScreen == LEFT){
                currentScreen = RIGHT;
                changeScreen(2);
            } else if (currentScreen == RIGHT){
                currentScreen = LEFT;
                changeScreen(1);
            }
        } else {
            if (currentScreen == LEFT) {
                shellMain(command, param, &esc);
            } else {
                shellMain(command, param, &esc);
            }
        }
    }
    
    changeScreen(RIGHT);
    scClear();
    changeScreen(LEFT);
    scClear();
    
    return;
}