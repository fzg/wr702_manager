#ifndef _UTIL_H_
#define _UTIL_H_

#ifndef WARN___D
#define WARN___D
#endif

void xfree(char **);
void *xmalloc(void **, size_t);
void xstrdup(char **, const char *);
void xsleep(int n);
int max(int a, int b);

#endif

