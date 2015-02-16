#include <stdio.h>
#include <stdlib.h>
#include "net.h"

/*\ TODO:
|*|	command dispatcher
|*|	commands
|*|	testing
\*/


/*
	Do we take parameters? if so how? pass along argv[] and chain funcs and their args?
	  Ex: #./prg ip 8.8.8.8 lg Admin pw hackme sm bridge sip 192.168.1.23 rb
				conf file + batch actions file?
	  Ex: #./prg actions
		actions: un Admin pw Admin
			 sup root toto
			 sssid FreeWifi
			 spsk 4free
			 smode Client
*/

char gV = 0;	// verbose
char *gUser = "admin";
char *gPass = "admin";

int work(const int s) {
	int err;
	size_t sz;
	char *r;
	r = makeReq("SysReboot", "Reboot=Reboot", &sz);
	err = sendReq(s, r, sz);
	free(r);
	return err;
}

int main(char c, const char **v) {
	int err, s;
	static char addr[] = "192.168.0.254";

	gV = (c > 1 && v[1][0] == 'v');
	if (gV) {
		printf("wr_tool: %s\n", addr);
	}
	if ((s = contact(makeAddr(addr))) == -1) {	// get socket
		strerror("contact");
	} else {
		if (gV) printf("Socket fd: %d\n", s);
		err = work(s);				// work
	}
	cleanup(s);
	return err;
}
