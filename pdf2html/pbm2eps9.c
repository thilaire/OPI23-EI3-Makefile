/* (c) 2000 Karel Kulhavy, Clocksoft
 * clock@atrey.karlin.mff.cuni.cz
 * This program is a stdin filter that performs these operations:
 * 1) Reads a pbm
 * 2) Prints it on a printer. The pbm is thresholded.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <png.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "printer.h"

unsigned char *basename;
unsigned char *titlestring;
unsigned char *bottom;
unsigned char *l1; /* Input data, output data. lw<<1 bytes long */
int filenumber;
int x,y,levels;
png_structp png_ptr;
png_infop info_ptr;
unsigned long ppm;
unsigned long dpi;
unsigned char string[8192];
unsigned char *filename;

/* Returns 0 is OK, exit(1) on error, returns 1 on broken pipe */
int 
sure_read(unsigned char *dest, size_t len)
{
        ssize_t rd;
        if (!len) return 0;
        again:
        rd=read(0,dest,len);
        if (rd==len) return 0;
        if (!rd) return 1;
        if (rd<0&&(errno==EINTR||errno==EAGAIN||errno==EWOULDBLOCK)) goto again;
        if (rd>0&&rd<len){
                len-=rd;
                dest+=rd;
                fflush(stderr);
                goto again;
        }
        fprintf(stderr,"read error.\n");
        perror("");
        exit(1);
}

int
eat_up_whitespace_and_comments(void)
{
        again:
        sure_read(string,1);
        if (*string==' '||*string=='\t'){
                goto again;
        }
        if (*string=='\n'||*string=='\r'){
                goto again;
        }
        if (*string=='#'){
                /* Eat up comment */
                aa:
                sure_read(string,1);
                if (*string!='\n'&&*string!='\r') goto aa;
                goto again;
        }
        return *string;
}

int
read_header(void)
{
        if (sure_read(string,2)) return 1; /* P4 or what */
	if (string[0]!='P'||string[1]!='4'){
		fprintf(stderr,"Input format violated: %c%c instead of P4.\n",string[0],
			string[1]);
		exit(1);
	}
        x=eat_up_whitespace_and_comments()-'0';
        again:
        sure_read(string,1);
        if (*string!=' '&&*string!='\t'&&*string!='\n'&&*string!='\r'){
                x*=10;
                x+=*string-'0';
                goto again;
        }
        y=eat_up_whitespace_and_comments()-'0';
        bgain:
        sure_read(string,1);
        if (*string!=' '&&*string!='\t'&&*string!='\n'&&*string!='\r'){
                y*=10;
                y+=*string-'0';
                goto bgain;
        }
        return 0;
}

/*
 * Parameters: <dpi> <basename> <titlestring> <bottom_html_code>
 * <pageoffset> [first page number]
 */
int
main(int argc, char **argv)
{
        int a,z;
	unsigned char *l2;
 	
	head_mask=0xfe;
	if (argc<3){
		fprintf(stderr,"Usage: pbm2eps9 <hquality><vquality>\nReads the data from stdin.\n");
		return 0;
	}
	hquality=atol(argv[1]);
	vquality=atol(argv[2]);
        again:
        if (read_header()) return 0;
	l1=(unsigned char *)malloc(x);
	l2=(unsigned char *)malloc((x+7)>>3);
	open_printer();
	for (z=y;z;z--){
		sure_read(l2,(x+7)>>3);	
		for (a=0;a<x;a++) l1[a]=!!((l2[a>>3]>>((~a)&7))&1);
		add_printer_line(l1,x);
	}
	free(l1);
	free(l2);
	close_printer();
        goto again;
}
