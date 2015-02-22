
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "net.h"
#include "ops.h"
#include "fortunes.h"
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

char gV = 0, *gUser = NULL, *gPass = NULL;
extern char *gAuth, *gAddr;

int work() {
	int err;
	char *it;

	if ((err = oSsid(getFortune())) == 401 || err == 65535) {
	  if ((err = tryReset(err)) == 65535)
            exit(printf("Network seems down. Aborting.\n"));
	}
	err = oPsk(getFortune());
//	err = oIp(s, "192.168.0.22", "255.255.255.0", MODE_STATIC);
	waitForReboot();
	err = oPwd(gUser, gPass, "toor", "root");
	xfree(&gAuth);
//	err = oMode(s, MODE_BRIDGE);
//	s = waitForReboot();
	err = oSurvey(&it);
	xfree(&it);
	return err;
}

int main(char c, const char **v) {
	int err = EXIT_FAILURE;
	static char addr[] = "192.168.0.22";
	xstrdup(&gUser, "toor");
	xstrdup(&gPass, "root");
	xstrdup(&gAddr, "192.168.0.22");
	if (c > 1) gV = (v[1][0] =='v')? 1: (v[1][0] =='V')? 2 : 0;
	err = work();				// work
	cleanup();
	return err;
}
