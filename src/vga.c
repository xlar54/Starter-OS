#include "vga.h"

// define our structure
typedef struct __attribute__ ((packed)) {
    unsigned short di, si, bp, sp, bx, dx, cx, ax;
    unsigned short gs, fs, es, ds, eflags;
} regs16_t;
 
// tell compiler our int32 function is external
extern void int32(unsigned char intnum, regs16_t *regs);
 
// int32 test
void int32_test()
{
	char *screen = 0xA0000;
	char mode = 0x0013; //0x0013;
    int y;
    regs16_t regs;
     
    // switch to 320x200x256 graphics mode
    regs.ax = mode;
    int32(0x10, &regs);
    
    setcolor(0, 0, 0, 0);		// Black
    setcolor(1, 255,255,255);	// White
    setcolor(2, 123,72,144);	// Magenta
    setcolor(3, 96,255,255);	// Cyan
    setcolor(4, 224,64,64);		// Red
    setcolor(5, 64,224,64);		// Green
    setcolor(6, 64,64,224);		// Blue
    setcolor(7, 255,255,64);	// Yellow
    setcolor(8, 255,165,0);		// Orange
    setcolor(9, 184,134,11);	// Brown???
    setcolor(10, 123,91,47);	// Pink
    setcolor(11, 136,136,136);	// DGrey
    setcolor(12, 84,84,84);		// Grey
    setcolor(13, 160,255,160);	// LGreen
    setcolor(14, 160,160,255);	// LBlue
    setcolor(15, 211,211,211);	// LGrey
    
    ClearVideo(screen, 12);   

    PutsAt("0123456789012345678901234567890123456789", 0,0,3,7);
    PutsAt("0123456789012345678901234567890123456789", 0,8,3,7);
    
    PutsAt("                                        ", 0,16,3,0);
	PutsAt("                                        ", 0,24,3,1);
	PutsAt("                                        ", 0,32,3,2);
	PutsAt("                                        ", 0,40,3,3);
	PutsAt("                                        ", 0,48,3,4);
	PutsAt("                                        ", 0,56,3,5);
	PutsAt("                                        ", 0,64,3,6);
	PutsAt("                                        ", 0,72,3,7);
	PutsAt("                                        ", 0,80,3,8);
	PutsAt("                                        ", 0,88,3,9);
	PutsAt("                                        ", 0,96,3,10);
	PutsAt("                                        ", 0,104,3,11);
	PutsAt("                                        ", 0,112,3,12);
	PutsAt("                                        ", 0,120,3,13);
	PutsAt("                                        ", 0,128,3,14);
	PutsAt("                                        ", 0,136,3,15);
	
    DrawLine(10,10,50,50,4);
    DrawRectangle(30,60,120,180, 5);
    DrawFilledRectangle(90,20,110,120, 15);
    DrawCircle(30,100, 10, 8);
    DrawCharacterAt('A', 60,60,3,7);    
    
    // wait for key
    regs.ax = 0x0000;
    int32(0x16, &regs);
     
    // switch to 80x25x16 text mode
    regs.ax = 0x0003;
    int32(0x10, &regs);
}

int abs(int x)
{
	int t = x >> 31; 
	// t is -1 if x is negative otherwise it is 0
	return t ^ (x + t);
}

void setcolor(int color, int red, int green, int blue)
{
  outportb(0x3C8, color);
  outportb(0x3C9, red);
  outportb(0x3C9, green);
  outportb(0x3C9, blue);
}


inline void ClearVideo(char *screen, char color)
{
    memset(screen, color, (320*200));
}

inline void DrawPixel(unsigned int x, unsigned int y, char color)
{
	char *screen = (char *)0xA0000;
	//screen[(y<<8)+(y<<6)+x]=color;  (this does work)
	unsigned int addr = 320 * y + x;
	screen[addr] = color;
}

void DrawLine(int x0, int y0, int x1, int y1, char color) 
{
 
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    DrawPixel(x0,y0, color);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void DrawRectangle(int x0, int y0, int x1, int y1,  char color)
{
	DrawLine(x0, y0, x1, y0, color);
	DrawLine(x0, y0, x0, y1, color);
	DrawLine(x0, y1, x1, y1, color);
	DrawLine(x1, y1, x1, y0, color);
}

void DrawFilledRectangle(int x0, int y0, int x1, int y1, char color)
{
	while(x0 <= x1 && y0 <= y1)
		DrawRectangle(x0++, y0++, x1, y1, color);
}

void DrawCircle(int x0, int y0, int radius,  char color)
{
  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    DrawPixel(x + x0, y + y0, color);
    DrawPixel(y + x0, x + y0, color);
    DrawPixel(-x + x0, y + y0, color);
    DrawPixel(-y + x0, x + y0, color);
    DrawPixel(-x + x0, -y + y0, color);
    DrawPixel(-y + x0, -x + y0, color);
    DrawPixel(x + x0, -y + y0, color);
    DrawPixel(y + x0, -x + y0, color);
 
    y++;
        if(radiusError<0)
                radiusError+=2*y+1;
        else
        {
                x--;
                radiusError+=2*(y-x+1);
        }
  }
}

void DrawCharacterAt(unsigned int ch, unsigned int x, unsigned int y,  char fcolor, char bcolor)
{
	// Ensure valid char table lookup
	ch = ch < 32 ? 0 : ch > 127 ? 0 : ch - 32;
	
	int col;
	unsigned int row;
	for(row = 0; row < CHAR_HEIGHT; row++)
	{
		unsigned int i = 0;
		for(col = CHAR_HEIGHT; col >= 0 ; col--)
		{
			if(row < CHAR_HEIGHT && (customfont[ch][row] & (1 << col)))
			{
				DrawPixel(x + i, y + row, fcolor);
			}
			else
			{
				DrawPixel(x + i, y + row, bcolor);
			}
			i++;
		}
	}
}

void PutsAt(const char *s, unsigned int x, unsigned int y, char fcolor, char bcolor)
{
	unsigned int i = 0;
	unsigned int c = 0;
	
	while(s[i] !=0)
	{
		c = s[i++];
		DrawCharacterAt(c, x,y, fcolor, bcolor);
		x += 8;
	};
	
}
