#include <graphicRenderer.h>
#include <screenDriver.h>
#include <font.h>

#define ENTER_KEY 13

static int cursorPosX = 0;
static int cursorPosY = 0;
static int maxX, maxY;

void initVideo(){
    initRenderer(0x5C00);
    //Determino los limites redondeados del posicionamiento de caracteres (para saber donde volver despues de un \b)
    maxX = (getWidth()/ABS_WIDTH)*ABS_WIDTH;
    maxY = (getHeight() / ABS_HEIGHT) * ABS_HEIGHT;
    cleanScreen();
}


int putChar(char c,unsigned int color){
    if (cursorPosX+ABS_WIDTH>maxX) //Me paso de ancho con esta insercion? Si es asi salto
        newLine();

    if (c=='\n'||c==ENTER_KEY){ //newline
        newLine();
        return 1; 
    }
    if (c=='\b') //backspace
    {
        backspace();
        return 1;
    }
    int res = renderChar(c,cursorPosX,cursorPosY,color);
    cursorPosX+=ABS_WIDTH;
    return res;
}

void newLine(){
    int scrollDist = ABS_HEIGHT;
    cursorPosX=0;
    cursorPosY += scrollDist;
    if (cursorPosY>=maxY){
        scrollUp(scrollDist);
        cursorPosY = maxY - scrollDist;
    }

}

void cleanScreen(){
    clearAll();
    cursorPosX = 0;
    cursorPosY = 0;
}

void backspace(){
    if (cursorPosX==0){
        if (cursorPosY==0)
            return;
        cursorPosY-=ABS_HEIGHT;
        cursorPosX = maxX;
 
    }
    cursorPosX -= ABS_WIDTH;
    renderArea(cursorPosX, cursorPosY, 
        cursorPosX + ABS_WIDTH, cursorPosY + ABS_HEIGHT, 0x000000);
}
