/* (c) 2000,2001 Karel 'Clock' Kulhavy <clock@atrey.karlin.mff.cuni.cz> */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PT_EPSON 0
#define PT_HP_QUIETJET 1
#define STRIP_WIDTH 3840
int printer_type=PT_EPSON;
int vquality, hquality; /* vquality: 3=high, 2=middle, 1=low.
                                     3=three-pass, 2=three-pass (not faster but
				     consumes less toner and is quieter, half the
				     vertical resolution), 1=one-pass (third the
				     resolution, thrice as fast, not quieter)
			   hquality: 1=low (480 dots per width)
			             2=middle-low (960 dots per width, some left out)
				     3=middle-high (960 dots per width, all dots present)
				     4=high (1920 dots per width, maybe some left out)
	                  */
int to_skip;
int head_mask=0xff; /* bit 7 is top needle (from the 8), bit 0 bottom needle
	             * 1 means works
		     */
int effective_head_mask;
int head_shift; /* effective_head_mask=head_mask<<head_shift so head_shift&0x80 is always
                   0x80 if at least one needle works.
		 */
unsigned strip[STRIP_WIDTH]; /* Organisation according to strip_masks. Microline #n is in bit
			 masked by strip_masks[n].  1 means dot, 0 means paper.
                            */
unsigned strip_masks[24]=
{
 0x80,0x8000,0x800000,
 0x40,0x4000,0x400000,
 0x20,0x2000,0x200000,
 0x10,0x1000,0x100000,
 0x08,0x0800,0x080000,
 0x04,0x0400,0x040000,
 0x02,0x0200,0x020000,
 0x01,0x0100,0x010000
};
int dots_to_be_printed; /* bit 1 means that dots-to-be-printed wait in this line */
int first_empty_microline;

unsigned inline rotate_slice(unsigned a)
{
	return (a>>8)|((a<<17)&(0xfe0000));
}

void
start_graphics(int length)
{
 unsigned char c[4]={'K','Y','L','Z'};
 unsigned char init_string[]={27,85,48,27,115,48};
 
 write(1,&init_string,sizeof(init_string));
 {
  unsigned char t[]={27,c[hquality-1],length,length>>8}; 
  write(1,&t,sizeof(t));
 }
}

void really_skip(void)
{
	unsigned char t[]={27,'J',0,13};
	int used_skip;

	while(to_skip)
	{
		used_skip=to_skip>=256?255:to_skip;
		t[2]=used_skip;
 		write(1,&t,sizeof(t));
		to_skip-=used_skip;
	}
}

void run_head(void)
{
	int i;
	unsigned char c;
	int length;

	really_skip();
	length=STRIP_WIDTH;
	while(length&&!(strip[length-1]&effective_head_mask))length--;
	start_graphics(length);
	for (i=0;i<length;i++)
	{
		c=strip[i];
		strip[i]&=~effective_head_mask;
		c>>=head_shift; /* When top needles are broken */
		write(1,&c,1);		
	}
	dots_to_be_printed&=~effective_head_mask;
}

/* Reduces by one microline */
void reduce(void)
{
	int a;

	if (dots_to_be_printed&0x80){
		run_head();
	}
	for (a=0;a<STRIP_WIDTH;a++) strip[a]=rotate_slice(strip[a]);
	dots_to_be_printed=rotate_slice(dots_to_be_printed);
	to_skip++;
	first_empty_microline--;
}

/* zero byte=white, nonzero byte=black */
void
add_printer_line(unsigned char *bytes, int length)
{
 int a;
 //int flag;
 unsigned strip_mask=strip_masks[first_empty_microline];
 
 if (length>STRIP_WIDTH) length=STRIP_WIDTH;
 //flag=0;
 for (a=0;a<length;a++)
 	if (bytes[a]){
		strip[a]|=strip_mask;
		dots_to_be_printed|=strip_mask;
		//		flag=1;
 	}
 first_empty_microline+=4-vquality;
 while (first_empty_microline>=22){
 reduce();
 }
}

void
write_header(void)
{
 unsigned char str[]=
 {
  27,64, /* Perform reset */
  27,'A',8, /* Line spacing to 8 dot rows */
  0x1b,0x55,0, /* Bidirectional */
  0xd /* Carriage return */
 };

 write(1,str,sizeof(str));
}

void compute_effective_head_mask(void)
{
	effective_head_mask=head_mask;
	head_shift=0;
	while (!(effective_head_mask&0x80)){
		effective_head_mask<<=1;
		head_shift++;
	}
}

/* Returns 1 error, 0 OK
   If <0, error, otherwise OK. */
int
open_printer(void)
{
	
	if (vquality<1) vquality=1;
	if (vquality>3) vquality=3;
	if (hquality<1) hquality=1;
	if (hquality>4) hquality=4;
	write_header();
	bzero(strip,sizeof(strip));
	to_skip=0;
	first_empty_microline=0;
	dots_to_be_printed=0;
	compute_effective_head_mask();
	return 0;
}

void pchar(unsigned char a)
{
	write(1,&a,1);
}

/* Call after you flush all the pixels into the printer */
void
close_printer(void)
{
 while(first_empty_microline) reduce();
 pchar(12); /* Form Feed */
}
