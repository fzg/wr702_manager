#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FBL (4096<<1)
#define MIN_LEN 5

#define DELIM " \n%.,'\"!\t?[](){};:"

extern char gV;

int wc(char *p) {
  int c = -1;
  char *k, *tmp;
  while ((k = strpbrk(p, DELIM))) {
   if (k-p >= MIN_LEN) {
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

char *nth(char *p, int i) {
  int c = -1;
  char *k;
  while ((k = strpbrk(p, DELIM))) {
   if (k-p >= MIN_LEN) {
    ++c;
    if (c == i) {
     *(p+(k-p)) = 0;
     return p;
    }
   }
   p=++k;
  }
}

void rot13(char *p) {
  char a, b;
  --p; while((a = *(++p))) *p=(((b=64^a&95)&&b<27)?a&224|(b+12)%26+1:a);
}

char *gFBuf;

void cleanFortune() {
  if (gFBuf) free(gFBuf);
  gFBuf = NULL;
}

char *getFortune() {
  FILE *ifs = NULL;
  static char *buf = NULL, *p, *q;
  unsigned int nbe, i, j, sz;
  size_t si = FBL;
  if (!buf) {
   if (!(buf = malloc(FBL))) {
    perror("fortune malloc");
   }
   srand(time(NULL));
   gFBuf = buf;
   memset(buf, 0, sizeof(buf));
  }
  ifs = fopen("./fortunes_off", "r");
  sz = getline(&buf, &si, ifs); // get nb of entries
  nbe = atoi(buf);
//  printf("There are %i entries\n", nbe);
  i = rand() % nbe ;
//  printf("We get the %i-th\n", i);
  for (j = 0; j <= i; ++j)
    sz = getdelim(&buf, &si, '%', ifs);
  // got a random fortune, now select a random word of min length x
  rot13(buf);
  if (gV > 2) printf("Got:\n%s\n", buf);
  i = wc(buf) + 1; // number of spaces.
  if (gV > 2) printf("There are %i bigwords\n", i);
  i = rand() % i;
  if (gV > 2) printf("We want the %ith\n", i);
  p = nth(buf, i); // to do: put \0 at end of word
//  printf("We got:%s\n", p);
  //q = strndup(p, (strpbrk(p, " \n%.,'\"") - p));
  fclose(ifs);

//  if (strlen(q) < MIN_LEN)
//    return getFortune();
//  else
    return p;
}
