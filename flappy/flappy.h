#ifndef FLAPPY_H_INCLUDED
#define FLAPPY_H_INCLUDED

#include <stdbool.h>

#include "pipe.h"

#define 	MAX_HEIGHT	(5)

struct s_flappy {
	unsigned char	height;
	unsigned int	nScore;
	bool			isHit;
};

typedef struct s_flappy t_flappy;

t_flappy *
flappy_create(int nScore);										// On créer le flappy avec un score défini

void
flappy_free(t_flappy *flappy);									// On libère le flappy :D

bool
flappy_collide_with_pipe(t_flappy *flappy, const t_pipe *pipe);	// On vérifie si le flappy entre en collision avec le tuyau

void
flappy_move(t_flappy *flappy, int newHeight);

#endif // FLAPPY_H_INCLUDED
