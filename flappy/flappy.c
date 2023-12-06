#include "flappy.h"

#include "memory.h"
#include "pipe.h"
#include "functions.h"

#include <stdio.h>

t_flappy *
flappy_create(int nScore) {
	t_flappy	*newFlappy = NULL;
	int 		defaultHeight;

	newFlappy = xmalloc(sizeof(t_flappy));

	printf("Tout d'abord, veuillez entrer la hateur de depart: ");
	scanf("%d", &defaultHeight);
	puts("\n");

	purge(stdin);

	if(defaultHeight > MAX_HEIGHT || defaultHeight < 0) {
		defaultHeight = (int)MAX_HEIGHT >> 1;	// Si la hauteur par défaut est plus grande que la hauteur max, alors on l'a divise par 2 pour que le flappy soit centré
	}

	newFlappy->height = defaultHeight;
	newFlappy->nScore = nScore;
	newFlappy->isHit  = false;

	return newFlappy;
}

void
flappy_free(t_flappy *flappy) {
	if(flappy != NULL) {
		xfree(flappy);
	}
}

bool
flappy_collide_with_pipe(t_flappy *flappy, const t_pipe *pipe) {
	if(flappy->height != pipe->posDoor) {
		return true;
	}
	return false;
}

void
flappy_move(t_flappy *flappy, int newHeight) {
	if(newHeight > MAX_HEIGHT) { 	// PAN, UN NUAGE
		flappy->isHit = true;
		return;
	}
	if(newHeight < 0) {				// PAN, UNE PAQUERETE
		flappy->isHit = true;
		return;
	}

	flappy->height = newHeight;
}
