#ifndef _OPS_H_
#define _OPS_H_

#include <stdlib.h>
#include <stdio.h>

#include "net.h"
#include "util.h"

// DEVICE MODES
#define MODE_AP         1
#define MODE_ROUTER     2
#define MODE_REPEATER   3
#define MODE_BRIDGE     4
#define MOD_CLIENT      5

// IP MODES
#define MODE_STATIC 1
#define MODE_SMART  2


extern char gV;
char *gOb;
extern char *gAddr, *gUser, *gPass, *gAuth;


void waitForReboot();



int oReset();
int oReboot();
int oPsk(const char *psk);
int oSsid(const char *ssid);
int oSurvey(char **res);
int oMode(const short m);
int oPwd(const char *ou, const char *op, const char *nu, const char *np);
int oIp(const char *ip, const char *nm, const short m);
int tryReset(int err);

#endif

