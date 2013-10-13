#include "conio.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"
#include "syscall.h"
#include "keyboard.h"
#include "vga.h"


extern u32int placement_address;
u32int initial_esp;

int sys_init(struct multiboot *mboot_ptr)
{
	// Initialise all the ISRs and segmentation
    init_descriptor_tables();
    	
	// Initialise the PIT to 100Hz
	puts("Initializing PIT...\n");
    initialise_timer(50);

    // Find the location of our initial ramdisk.
    puts("Calculating RAMDisk...\n");
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    // Start paging.
    puts("Initializing Memory Paging...\n");
    //initialise_paging();

    // Start multitasking.
    puts("Initializing Multitasking...\n");
    //initialise_tasking();

    // Initialise the initial ramdisk, and set it as the filesystem root.
    puts("Initializing RAMDisk as root filesystem...\n");
    fs_root = initialise_initrd(initrd_location);

	puts("Initializing SYSCALLS...\n");
    initialise_syscalls();

	puts("Initializing keyboard handler...\n");
	initialise_keyboard();

	// Start interrupts
	asm volatile("sti");
	
	//puts("Switching to user mode...\n");
    //switch_to_user_mode();	
    //syscall_puts("Hello, user world!\n");
	
	return 0;
}


int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
	unsigned char *prompt = "\nREADY.\n";
	unsigned char *header1 = "\n                **** COMMODORE 2000 OPERATING SYSTEM V1.0 ****\n";
	unsigned char *header2 = "                       128GB SYSTEM RAM   UBASIC v2.0\n";

	
    initial_esp = initial_stack;
    sys_init(mboot_ptr);
    
    setForeColor(CYAN);
    setBackColor(BLUE);
    clrscr();
 
	puts(header1);
	puts(header2);

	puts(prompt);
	
int32_test();

	
    while(1)
    {
		char c = getch();
		
		if(c)
		{
			if(c == 0x0A)
			{
				puts(prompt);
			}
			else
				putch(c);
		}
	}; 
    
    return 0;
}
