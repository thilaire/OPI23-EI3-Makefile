#ifndef PIPE_H_INCLUDED
#define PIPE_H_INCLUDED

struct s_pipe {
	char	posDoor;
};

typedef struct s_pipe t_pipe;

void
new_pipe(t_pipe *pipe);

#endif // PIPE_H_INCLUDED
