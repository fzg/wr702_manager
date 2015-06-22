#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "ops.h"
#include "fortunes.h"

#define SZ(p) (sizeof(p)/sizeof(typeof(p)))

extern char *gAddr, *gUser, *gPass, *gAuth, gV, gP;
struct addrinfo *gPaI = NULL;

int setVerbosity(char **p) {
  ++gV;
  return 0;
}

int setOverrideDefaults(char **p) {
  return 0;
}

int setIP(char **p) {
  xstrdup(&gAddr, p[0]);
  return 0;
//  size_t s = SZ(p);
//  return oIp(p[0], "255.255.255.0", MODE_STATIC);
}

int setUser(char **p) {
  xstrdup(&gUser, p[0]);
  xstrdup(&gPass, p[1]);
  return 0;
}

//TODO add support for named ones
int setMode(char **p) {
  return oMode(atoi(p[0]));
}

int setSSID(char **p) {
  return oSsid(p[0]);
}

//todo: use last specified user
int setNPwd(char **p) {
  puts("setNPwd: STUB");
  return -1;
}

//todo: use last specified pwd
int setNUser(char **p) {
  puts("setNUser: STUB");
  return -1;
}

int setPsk(char **p) {
  return oPsk(p[0]);
}

int setRandomSSID(char **x) {

  return oSsid(minGetFortune());
}

int setRandomPsk(char **x) {
  char *p = NULL;
  while (strlen((p = minGetFortune())) < 8 || strlen(p) > 63);
  return oPsk(p);

}

int askReboot(char **a) {
  int err = oReboot();
  waitForReboot();
  return err;
}

int askClientList(char **x) {
	char * s = NULL;
	 oSurvey(&s);
	 xfree(&s);
}

char *gProx = NULL;

int setProxy(char **a) {
	// Les varargs sont remplis en amont. On doit en avoir 2.
	// Sinon quoi? Exactement.
	char *ipprt = NULL;
	int err, l;
	struct addrinfo hints;

	if (!a || !a[0] || !a[1]) return EXIT_FAILURE;
	if (!xmalloc((void**)&ipprt, (l = strlen(a[0])+strlen(a[1])+2)))
		return EXIT_FAILURE;
	gP = 1;	// raise da flag.

  if (gV) printf("[Proxy] %s:%s\n",a[0], a[1]);

	// TODO: Test the proxy before silently using it
	memset(ipprt, 0, l);
	strcat(ipprt, a[0]);
	ipprt[strlen(a[0])] = ':';
	strcat(ipprt+(strlen(a[0])+1), a[1]);
	xfree(&gProx);
	gProx = ipprt;


        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_ADDRCONFIG;

	//hints = NULL;//FIXME TEST


	if (getaddrinfo(a[0], a[1], 0/*&hints*/, &gPaI)) perror("proxygetaddrinfo");

//	THIS WOULD HAVE BEEN FOR AN SSL PROXY
//	if (err = proxify(ipprt)) {perror("proxify"); exit(1);}
	return err;
}

