#ifndef _NET_C_
#define _NET_C_

#include "defs.h"

#include <sys/types.h>
#include "b64.h"

#define EXIT_CONTACT -1
#define BL      4096*16	// Input buffer size for html pages


struct in_addr 	*makeAddr(char *ip);
//int 		contact(struct in_addr *x);
char 		*makeReq(const char *page, const char *req, size_t *sz);
unsigned short	sendReq(const char *buf, size_t s, char **);
void 		cleanup(void);
int		proxify(const char *ipprt);
#endif


