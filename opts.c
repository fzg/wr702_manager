#include <string.h>
#include "ops.h"
#include "fortunes.h"

#define SZ(p) (sizeof(p)/sizeof(typeof(p)))

extern char *gAddr, *gUser, *gPass, *gAuth, gV;

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

