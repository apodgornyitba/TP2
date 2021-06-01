#include <stdint.h>
#include <graphicRenderer.h>
#include <font.h>

//LINK DE APOYO: https://wiki.osdev.org/User:Omarrx024/VESA_Tutorial
struct vbe_mode_info_structure
{
	uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;	  // deprecated
	uint8_t window_b;	  // deprecated
	uint16_t granularity; // deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;		   // number of bytes per horizontal line
	uint16_t width;		   // width in pixels
	uint16_t height;	   // height in pixels
	uint8_t w_char;		   // unused...
	uint8_t y_char;		   // ...
	uint8_t planes;
	uint8_t bpp;   // bits per pixel in this mode
	uint8_t banks; // deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__((packed));

struct vbe_mode_info_structure *screenData;

static int width = 0;
static int height = 0;

void initRenderer(long int infoLoc){
	screenData = (void *)infoLoc;
	height = screenData->height;
	width = screenData->width;
}

int getHeight()
{
	return height;
}

int getWidth()
{
	return width;
}

int renderPixel(unsigned int x, unsigned int y, unsigned int color)
{
	if (x < 0 || y < 0 || x > width || y > height)
		return -1;
	char *pos = (char *)((uint64_t)screenData->framebuffer + (x + y * width) * 3);
	*pos = (color & 0x0000FF);
	*(pos+1) = (color & 0x00FF00) >> 8;
	*(pos+2) = (color & 0xFF0000) >> 16;
	return 0;
}

int renderArea(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color){
	if (x2<x1 || y2<y1)
		return -1;
	if (x1 < 0 || y1 < 0 || x2 > width || y2 > height)
		return -2;
	for (int i=x1;i<=x2;i++){
		for (int j=y1;j<=y2;j++)
			renderPixel(i,j,color);
	}
	return 0;
}

//LINK DE APOYO: https://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
int renderChar(unsigned char c, unsigned int x, unsigned int y, unsigned int color)
{
	if (x < 0 || y < 0 || x + (ABS_WIDTH) > width || y + (ABS_HEIGHT) > height)
		return -1;

	// 'font' es un arreglo multidimensional de [96][FONT_WIDTH]
	// que es un arreglo 1D de tamano 96*FONT_WIDTH.
	const uint8_t *chr = charBitmap(c);

	// Draw pixels
    for (int j = 0; j < FONT_HEIGHT; j++)
    {
        for (int i = 0; i < FONT_WIDTH; i++){
			if (chr[j] & (1 << i)) //En esta posicion hay un bit encendido
                renderArea(x + ABS_WIDTH - (i + 1) * FONT_SCALING, y + j * FONT_SCALING, 
                ABS_WIDTH + x - i * FONT_SCALING, y + (j + 1) * FONT_SCALING, color);
        }
    }
    return 0;
}

void clearAll(){
	char *pos = (char *)((uint64_t)screenData->framebuffer);
	for (int i=0;i<height*width*3;i++)
		pos[i]=0;

}

int scrollUp(int pixels){
	if (pixels>height){
		clearAll();
		return -1;
	}
	char *pos = (char *)((uint64_t)screenData->framebuffer);
	for (int i = 0; i < width; i++){
		for (int j=0;j<height-pixels;j++){
			for (int k=0;k<3;k++){ // per color
				*pos = *(pos + (pixels * width) * 3);
				pos++;
			}
		}
	}
	renderArea(0,height-pixels,width,height,0x000000);
	return 0;
}