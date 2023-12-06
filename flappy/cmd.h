#ifndef CMD_H_INCLUDED
#define CMD_H_INCLUDED

enum {
	MIN_MOUVEMENT = -5,
	MAX_MOUVEMENT = 5,
	CMDHELP,
	CMDEXIT
};

int
get_command(void);	// Reçevoir une commande

#endif // CMD_H_INCLUDED
