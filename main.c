#include <stdio.h>
#include <stdlib.h>
#include "net.h"

char gV = 0;	// verbose
char *gUser = "admin";
char *gPass = "admin";

int work(const int s) {
	char *r;
	r = makeReq("SysReboot", "Reboot=Reboot");

	free(r);
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
		err = work(s);				// work
	}
	cleanup(s);
	return err;
}
