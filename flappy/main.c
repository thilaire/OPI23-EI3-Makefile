#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "flappy.h"
#include "pipe.h"
#include "cmd.h"
#include "functions.h"

void
slapsh_screen(void) {
	char *screen[10] = {
		"+-----------------------------------------------------------------------------+",
		"|                                                                             |",
		"|         _____ _         _ _____       _         _____                       |",
		"|        |  _  |_|_ _ ___| |     |___ _| |___    |_   _|___ ___ _____         |",
		"|        |   __| |_'_| -_| |   --| . | . | -_|     | | | -_| .'|     |        |",
		"|        |__|  |_|_,_|___|_|_____|___|___|___|     |_| |___|__,|_|_|_|        |",
		"|                                                                             |",
		"|                             Jeu fait par Le_BuG63                           |",
		"|                                                                             |",
		"+-----------------------------------------------------------------------------+",
	};

	for(int line = 0; line < 10; ++line) {
		puts(screen[line]);
		wait(125);
	}
	puts("\n\n");

	puts("Bienvenue dans FlappyTxt.\nTapez \"HELP\" pour avoir de l'aide !\n\n");
}

void
help(void) {
	puts("\nTapez les commandes suivantes pour faire une action:\n"
		 "\t- UP <n>: n est le nombre de case que va monter le Flappy\n"
		 "\t- DOWN <n>: pareil que UP, sauf pour descendre\n"
		 "\t- EXIT: quitte le jeu\n\n");
}

int main(void) {
	bool		running = true;
	t_flappy	*flappy = NULL;
	int			cmd;
	t_pipe		pipe;

	srand(time(NULL));

	slapsh_screen();

	flappy = flappy_create(0);

	while(running) {
		new_pipe(&pipe);

		printf("Vous etes a la hauteur %d. L'ouverture dans le tuyau se situe a %d de hauteur.\nEntrez une commande (tapez HELP pour l'aide):\n", flappy->height, pipe.posDoor);

		cmd = get_command();

		if(cmd == CMDEXIT) {
			running = false;
		}

		while(cmd == CMDHELP) {
			help();
			puts("Entez une commande:");
			cmd = get_command();
		}

		if(cmd >= MIN_MOUVEMENT && cmd <= MAX_MOUVEMENT) {
			int pos = cmd;
			int chance = 0;

			puts("\n*FLAP FLAP*...\n");

			wait(1500);

			if(pos != 1 && pos != 0 && pos != -1)
				chance = rand() % (int)(abs(pos) / 1.35);

			if(chance == (int)(pos >> 1)) {
				flappy_move(flappy, flappy->height + pos);
				printf("Tuyau passe ! Score [%d]...\n", flappy->nScore + 1);
			}
			else
				flappy->isHit = true;
		}


		if(flappy_collide_with_pipe(flappy, &pipe) || flappy->isHit) {
			int ans;

			printf("Tu as perdu... Tu as fait un score de: %d\n", flappy->nScore);
			puts("Veux-tu recommencer ? (1 = oui/ 0 = non)\n");

			scanf("%d", &ans);

			if(ans == 1) {
				flappy_free(flappy);
				flappy = flappy_create(0);
			} else {
				running = false;
			}
		} else {
			++flappy->nScore;
		}
	}

	flappy_free(flappy);

	puts("\n\nMerci d'avoir joue a FlappyTxt !");

    return 0;
}
