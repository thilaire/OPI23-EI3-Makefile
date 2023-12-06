#include "pipe.h"

#include "functions.h"
#include "flappy.h"

#include <time.h>
#include <stdlib.h>

void
new_pipe(t_pipe *pipe) {
	int	door = rand() % MAX_HEIGHT + 1;

	if(door == 0) {
		door = 1;
	}

	pipe->posDoor = door;
}
