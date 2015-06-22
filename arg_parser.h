#ifndef _ARG_P_
#define _ARG_P_

#include "defs.h"

//int a ajouter:
//   retries (n; infinite := -1)

//#ifndef NULL
//#define NULL 0
//#endif

typedef int (*cbk)(char **);

struct s_arg {
	const short unsigned	ops_n;	// number of operands
	const short unsigned	ops_m;	// mandatory ones
	const cbk const		handler;// fptr to handler
	const char		n_rbt;	// needs reboot?
        const char * const      aname;  // arg name
};
struct s_default_arg {
        const struct s_arg	*a;
        const unsigned short	i;
        char			*val;
};

typedef struct s_arg t_arg;
typedef struct s_default_arg t_default_arg;

extern t_arg args[];
extern t_default_arg defaults[];// = {

#define ARG(X) ((const t_arg*)(&args[(X)]))


// ARGUMENT HANDLING

int     	parse_args(int c, const char **v);
int             parse_arg(const char **v, int i, int *err);

const t_arg     *match_arg(const char *a);

// DEFAULTS MANAGEMENT

void 		override_default_arg(const t_arg *a, int i, char *val);
//char		isOp(char **a);
char 		*get_default(const t_arg *a, int i);


#endif

