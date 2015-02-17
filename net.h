#ifndef _NET_C_
#define _NET_C_

#include <sys/types.h>

struct in_addr 	*makeAddr(char *ip);
int 		contact(struct in_addr *x);
char 		*makeReq(const char *page, const char *req, size_t *sz);
unsigned short	sendReq(short socket, const char *buf, size_t s, char **);
void 		cleanup(int);
#endif


