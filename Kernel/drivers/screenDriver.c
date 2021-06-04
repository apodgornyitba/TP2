#include <graphicRenderer.h>
#include <screenDriver.h>
#include <font.h>

#define ENTER_KEY 13

static int Pos1X = 0;
static int Pos1Y = 0;
static int Pos2X = 515;
static int Pos2Y = 0;
static int max1X, max2X, maxY;

static int currentScreen = 1;

void initVideo(){
    initRenderer(0x5C00);
    //Determino los limites redondeados del posicionamiento de caracteres (para saber donde volver despues de un \b)
    max1X = (getWidth()/2) - 2;
    max2X = getWidth();
    maxY = getHeight();
    cleanScreen();
    // separateMainScreen();
}

int changeScreen(int screen) { 
    if (screen == 1 || screen == 2) {
        currentScreen = screen; 
        return 1;
    }
    return 0;
}

int getCurrentScreen() {
    return currentScreen; 
}

int putChar(char c,unsigned int color){
    if (c=='\n'||c==ENTER_KEY){ //newline
        newLine();
        return 1; 
    }
    if (c=='\b'){ //backspace
        backspace();
        return 1;
    }
    int res = 0;
    if(currentScreen == 1){
        if (Pos1X + ABS_WIDTH > max1X) //Me paso de ancho con esta insercion? Si es asi salto
            newLine();
        res = renderChar(c,Pos1X, Pos1Y, color);
        Pos1X += ABS_WIDTH;
    }else if(currentScreen == 2){
         if (Pos2X + ABS_WIDTH > max2X) //Me paso de ancho con esta insercion? Si es asi salto
            newLine();
        res = renderChar(c,Pos2X, Pos2Y, color);
        Pos2X += ABS_WIDTH;
    }
    return res;
}

void newLine(){
    int scrollDist = ABS_HEIGHT;

    if (currentScreen == 1) {
        Pos1X=0;
        Pos1Y += scrollDist;
        if (Pos1Y >= maxY) {
            scrollUp(scrollDist);
            Pos1Y = maxY - scrollDist;
        }
    }else if(currentScreen == 2){
        Pos2X = 515;
        currentScreen += scrollDist;
        if (Pos2Y >= maxY) {
            scrollUp(scrollDist);
            Pos2Y = maxY - scrollDist;
        }
    }
}

void cleanScreen(){
    if(currentScreen == 1){
        clearAll();
        Pos1X = 0;
        Pos1Y = 0;
    }else if (currentScreen == 2){
        clearAll();
        Pos2X = 515;
        Pos2Y = 0;
    }
}

void backspace(){
    if(currentScreen == 1){
        if (Pos1X == 0){
            if (Pos1Y == 0)
                return;
            Pos1Y -= ABS_HEIGHT;
            Pos1X = max1X;
        }
        Pos1X -= ABS_WIDTH;
        renderArea(Pos1X, Pos1Y, Pos1X + ABS_WIDTH, Pos1Y + ABS_HEIGHT, 0x000000);
    }

    if(currentScreen == 2){
        if (Pos2X == 515){
            if (Pos2Y == 0){
                return;
            }
            Pos2Y -= ABS_HEIGHT;
            Pos2X = max2X;
        }
        Pos2X -= ABS_WIDTH;
        renderArea(Pos2X, Pos2Y, Pos2X + ABS_WIDTH, Pos2Y + ABS_HEIGHT, 0x000000);
    }
}
