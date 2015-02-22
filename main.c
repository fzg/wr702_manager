#include <stdio.h>
#include <stdlib.h>
#include "net.h"
#include "ops.h"
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
char *gUser = "toor";
char *gPass = "root";
extern char *gAuth;

char *getFortune();
void cleanFortune();
int work() {
	int err;
	const char *it;

	err = oSsid(getFortune());
	err = oPsk(getFortune()); // broken
//	s = waitForReboot();
//	err = oIp(s, "192.168.0.22", "255.255.255.0", MODE_STATIC);
	waitForReboot();
	err = oPwd(gUser, gPass, "toor", "root");
	free(gAuth); gAuth = NULL;
//	err = oMode(s, MODE_BRIDGE);
//	s = waitForReboot();

	err = oSurvey(&it);
//	free((char*)it);
	return err;
}

int main(char c, const char **v) {
	int err = EXIT_FAILURE;
	static char addr[] = "192.168.0.22";

//        puts(getFortune());
//	cleanFortune();
//        exit(0);

	if (c > 1) gV = (v[1][0] =='v')? 1: (v[1][0] =='V')? 2 : 0;
        gAddr = addr;
	err = work();				// work
	cleanup();
	cleanFortune();
	return err;
}
