#include "functions.h"

#include <time.h>

static long seed;

long
ran(void) {


	seed *= 125;
	seed -= (seed/2796203) * 2796203;

	return seed;
}

int
rnd(int range) {
	return range < 1 ? 0 : ((ran() + ran())&0x7fffffffl) % range; // Si vous trouvez la référence <3
}

void
wait(unsigned int delay) {
    time_t 	start,
			end;

    start=clock();

    while(((end=clock())-start) <= delay);
}

void
purge (FILE * fp) {
   int 	c;
   while((c = fgetc (fp)) != '\n' && c != EOF)
	;
}
