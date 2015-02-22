#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define NUMARGS(...)  (sizeof((void*[]){__VA_ARGS__})/sizeof(void**))
#define XFREE_N(...)  (xfree_n(NUMARGS(__VA_ARGS__), __VA_ARGS__))

void xfree(char **b) {
  if (!*b) return;
  free(*b); *b = NULL;
}

void xfree_n(int n, ...) {
 va_list x;
 int i;
 va_start(x, n);
 for (i = 0; i < n; ++i)
  xfree(va_arg(x, char **));
 va_end(x);
}

void xmalloc(void **b, size_t s) {
  if (*b) xfree((char**)b);
  if (!(*b = malloc(s))) {
   perror("xmalloc");
   exit(EXIT_FAILURE);
  }
}

void xstrdup(char **d, char *s) {
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
  putchar(x[n%strlen(x)]);
  fflush(stdout);
  usleep(1000000/3);
  putchar('\b');
  } while (--n > 0);
  puts("\b\n");
}

int max(int a, int b) {
        return (a > b)? a : b;
}

extern char     gV, *gPass, *gUser, *gOb, *gAddr, *gAuth;

void cleanup() {
  if (gV) puts("cleanup...");
  cleanFortune();
  base64_cleanup();
  XFREE_N(&gAddr, &gAuth, &gUser, &gPass);
  return;
}
