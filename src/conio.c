// monitor.c -- Defines functions for writing to the monitor.
//             heavily based on Bran's kernel development tutorials,
//             but rewritten for JamesM's kernel tutorials.

#include "conio.h"

// The VGA framebuffer starts at 0xB8000.
u16int *video_memory = (u16int *)0xB8000;

// Stores the cursor position.
u8int cursor_x = 0;
u8int cursor_y = 0;

// Text color attributes
u8int foreColor = WHITE;
u8int backColor = BLACK;
u8int attrib = 0x0F;

// Clears the screen, by copying lots of spaces to the framebuffer.
void clrscr()
{
    // Make an attribute byte for the default colours
    //u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 | (attrib << 8); //(attributeByte << 8);

    int i;
    for (i = 0; i < 80*25; i++)
        video_memory[i] = blank;

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void setForeColor(unsigned char forecolor)
{
	foreColor = forecolor;
	attrib = (backColor << 4) | (foreColor & 0x0F);
}

void setBackColor(unsigned char backcolor)
{
	backColor = backcolor;
	attrib = (backColor << 4) | (foreColor & 0x0F);
}

// Updates the hardware cursor.
static void move_cursor()
{
    // The screen is 80 characters wide...
    u16int cursorLocation = cursor_y * 80 + cursor_x;
    outportb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outportb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outportb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outportb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
{

    // Get a space character with the default colour attributes.
    u16int blank = 0x20 | (attrib << 8);

    // Row 25 is the end, this means we need to scroll up
    if(cursor_y >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24*80; i < 25*80; i++)
        {
            video_memory[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = 24;
    }
}

// Writes a single character out to the screen.
void putch(unsigned char c)
{
    // The background colour is black (0), the foreground is white (15).
    //u8int backColour = 0;
    //u8int foreColour = 15;

    // The attribute byte is made up of two nibbles - the lower being the 
    // foreground colour, and the upper the background colour.
    //u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    u16int attribute = attrib << 8;
    u16int *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09)
    {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    // Handle any other printable character.
    else if(c >= ' ')
    {
        location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y ++;
    }

    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();

}



// Outputs a null-terminated ASCII string to the monitor.
void puts(unsigned char *text)
{
    int i = 0;
    while (text[i])
    {
        putch(text[i++]);
    }
}

void puthex(u32int n)
{
    s32int tmp;

    puts("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            putch(tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            putch( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        putch(tmp-0xA+'a');
    }
    else
    {
        putch(tmp+'0');
    }

}

void putdec(u32int n)
{

    if (n == 0)
    {
        putch('0');
        return;
    }

    s32int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    puts(c2);

}
