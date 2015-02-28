
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "net.h"
#include "ops.h"
#include "fortunes.h"
#include "arg_parser.h"

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

int usage(const char *pn) {
  return printf("usage: %s [args]\n\nFor args, see the top of arg_parser.c\n", pn);
}

int work(char c, const char **v) {
	int err;
	char *it;

        xstrdup(&gUser, "toor");
        xstrdup(&gPass, "root");
        xstrdup(&gAddr, "192.168.0.22");

try:
      err = parse_args(c, v);
      if (gV > 1) printf("[main]\t\terr=%d\n", err);
      if (err == 401 || err == 65535) {
        if ((err = tryReset(err)) == 65535) {
            printf("[main]\t\tNetwork seems down. Aborting.\n");
	    goto end;
        }
	else goto try;
      }

//	if ((err = oSsid(getFortune())) == 401 || err == 65535) {
//	  if ((err = tryReset(err)) == 65535)
//            exit(printf("Network seems down. Aborting.\n"));
//	}
//	err = oPsk(getFortune());
//	err = oIp(s, "192.168.0.22", "255.255.255.0", MODE_STATIC);
//	waitForReboot();
//	err = oPwd(gUser, gPass, "toor", "root");
//	xfree(&gAuth);
//	err = oMode(s, MODE_BRIDGE);
//	s = waitForReboot();
//	err = oSurvey(&it);
//	xfree(&it);
end:
	return err;
}

int main(char c, const char **v) {
	int err = EXIT_FAILURE;

//	extractFortunes(5);
//	while (++ err < 42) puts(minGetFortune());
//	exit(0);

//	xstrdup(&gUser, "toor");
//	xstrdup(&gPass, "root");
//	xstrdup(&gAddr, "192.168.0.22");
//	if (c > 1) gV = (v[1][0] =='v')? 1: (v[1][0] =='V')? 2 : 0;
//	waitForReboot();

	err = (c>1)? work(c, v) : usage(v[0]);
	cleanup();
	return err;
}
