#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>


#include "b64.h"
#include "fortunes.h"

#define NUMARGS(...)  (sizeof((void*[]){__VA_ARGS__})/sizeof(void**))
#define XFREE_N(...)  (xfree_n(NUMARGS(__VA_ARGS__), __VA_ARGS__))

extern char gV;

void xfree(char **b) {
  if (!*b) return;
  free(*b); *b = NULL;
}

void xfree_n(int n, ...) {
 va_list x;
 int i;
 if (gV > 1) printf("xfree_n: %d", n);
 va_start(x, n);
 for (i = 0; i < n; ++i)
  xfree(va_arg(x, char **));
 va_end(x);
}

void *xmalloc(void **b, size_t s) {
  if (*b) xfree((char**)b);
  if (!(*b = malloc(s))) {
   perror("xmalloc");
   exit(EXIT_FAILURE);
  } else return *b;
}

void xstrdup(char **d, const char *s) {
  if (*d) xfree((char**)d);
  if (!(*d = strdup(s))) {
   perror("xstrdup");
   exit(EXIT_FAILURE);
  }
}

void xsleep(int n) {
  const char x[] = "/|\\-";
  n *= 3;
  do {
   if (gV) {
    putchar(x[n%strlen(x)]);
    fflush(stdout);
   }
   usleep(1000000/3);
   if (gV) putchar('\b');
  } while (--n > 0);
  if (gV) {
   putchar('\b');
   fflush(stdout);
   puts(" \b");
  }
}

int max(int a, int b) {
        return (a > b)? a : b;
}

extern char     gV, *gPass, *gProx, *gUser, *gOb, *gAddr, *gAuth, **gFBuf;

void cleanup() {
  if (gV) { printf("[cleanup]\t..."); fflush(stdout); }
  base64_cleanup();
  XFREE_N(&gAuth, &gAddr, &gUser, &gPass, &gOb, &gProx);
  if (gFBuf) xfree(gFBuf);
  if (gV) puts(" done.");
  return;
}
