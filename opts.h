#ifndef _OPTS_H_
#define _OPTS_H_

#define OPT_ATTEMPT_RECOVER	1
#define OPT_RETRY_ON_ERROR	1 << 1
#define OPT_IGNORE_ERRORS	1 << 2


int setIP(char **);
int setUser(char **);
int setMode(char **);
int setSSID(char **);
int setPwd(char **);
int setPsk(char **);
int setNUser(char **);
int setNPwd(char **);

int setRandomSSID(char **);
int setRandomPsk(char **);


int setVerbosity(char **);
int setOverrideDefaults(char **);


#endif
