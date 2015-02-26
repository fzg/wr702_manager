#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dat.h"

#define FBL (4096<<1)			// fortune line buffer length
#define MIN_LEN 5			// min length for match
#define DELIM " \n%.,'\"!\t?[](){};:"	// word delimiters

extern char gV;				// verbosity
static char *gFBuf;			// fortune line buffer

int data_loc = -1; // DO NOT USE. Legacy
int gMinLen = MIN_LEN;

static int wc(char *p) {
  int c = -1;
  char *k, *tmp;
  while ((k = strpbrk(p, DELIM))) {
   if (k-p >= gMinLen) {
    ++c;
    if (gV > 2) {
     printf("%3i\t%s\n", c, (tmp = strndup(p, k-p)));
     free(tmp);
    }
   }
   p=++k;
  }
  return c;
}

static char *nth(char *p, int i) {
  int c = -1;
  char *k;
  static char *r = NULL;

//  printf("nth %d\n", i);
  while ((k = strpbrk(p, DELIM))) {
   if (k-p >= gMinLen) {
    ++c;
    if (c == i) {
     *(p+(k-p)) = 0;
     xstrdup(&r, p); // to comment
     *(p+(k-p)) = ' '; //tocomment
     return r;
    }
   }
   p=++k;
  }
}

static void rot13(char *p) {
  static char a, b;
  --p; while((a = *(++p))) *p=(((b=64^a&95)&&b<27)?a&224|(b+12)%26+1:a);
}

void cleanFortune() {
  if (gFBuf) free(gFBuf);
  gFBuf = NULL;
}

char *extractFortunes(int minl) {
  FILE *ifs = NULL;
  static char *buf = NULL, *p, *q;
  unsigned int i, j, sz, k;
  size_t si = FBL;
  gMinLen = minl;
  if (!buf) {
   xmalloc(&buf, FBL);
   srand(time(NULL));
   gFBuf = buf;
   memset(buf, 0, sizeof(buf));
  }
  if (0) {//!(ifs = fopen(__progname, "r"))) {
   perror("fortunes_off fopen");
  }
  fseek(ifs, data_loc, SEEK_SET);
  sz = getline(&buf, &si, ifs); // get nb of entries
  i = atoi(buf);
  if (gV) printf("Extracting words > %d chars\n", minl);
  for (j = 0; j <= i; ++j)  {
    sz = getdelim(&buf, &si, '%', ifs);
    rot13(buf); // offensive fortunes are rot13-scrambled
    i = wc(buf) + 1;
    for (k = 0; k < i; ++k) printf("%s\\n \\\n", nth(buf, k));
  }
  fclose(ifs);
  return NULL;
}


char *getFortune() {
  FILE *ifs = NULL;
  static char *buf = NULL, *p, *q;
  unsigned int i, j, sz;
  size_t si = FBL;
  if (!buf) {
   xmalloc(&buf, FBL);
   srand(time(NULL));
   gFBuf = buf;
   memset(buf, 0, sizeof(buf));
  }
  if (!(ifs = fopen("./fortunes_off", "r"))) {
//  if (!(ifs = fopen(__progname, "r"))) {
   perror("fortunes_off fopen");
  }
  fseek(ifs, data_loc, SEEK_SET);
  sz = getline(&buf, &si, ifs); // get nb of entries
  i = rand() % atoi(buf);
  for (j = 0; j <= i; ++j) sz = getdelim(&buf, &si, '%', ifs);
  // got a random fortune, now select a random word of min length x
  rot13(buf); // offensive fortunes are rot13-scrambled
  if (gV > 2) printf("Got:\n%s\n", buf);
  i = wc(buf) + 1;
  if (gV > 2) printf("There are %i bigwords\n", i);
  i = rand() % i;
  if (gV > 2) printf("We want the %ith\n", i);
  p = nth(buf, i);
  fclose(ifs);
  return p;
}

// we


char *minGetFortune() {
 static int nFort = 0, i;
 static char *ret = NULL;
 char *p, *k;
 if (!nFort) {
   p = fortunes;
   while (p && *p && (p = strchr(p, '\n'))) {++nFort;++p;}
   if (gV) printf("We got %d words\n", nFort);
   srand(time(NULL));
 }
 i = rand() % nFort;
//printf("%d\t", i);
 p = fortunes;
 while (((p = strchr(p, '\n')) && i)) {--i; ++p;}
 p += 2;
 k = strpbrk(p, DELIM);
//printf("k-p %d\n", k-p);
 *(p+(k-p)) = 0;
 xstrdup(&ret, p);
 *(p+(k-p)) = '\n';
 return ret;
}
