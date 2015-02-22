#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FBL 4096
#define MIN_LEN 5

int wc(char *p) {
  int c = 0;
  char *k;
  while ((k = strchr(p, ' '))) {
   ++c; p=++k;
  }
  return c;
}

char *nth(char *b, int i) {
  int c = 0;
  char *p = b;
  while (i > c && (p = strchr(p, ' ') + 1)) { ++ c;
// puts(p);
}
  return p;

}

void rot13(char *p) {
  char a, b;
  --p; while((a = *(++p))) *p=(((b=64^a&95)&&b<27)?a&224|(b+12)%26+1:a);
}

char *getFortune() {
  FILE *ifs;
  static char *buf = NULL, *p, *q;
  unsigned int nbe, i, j, sz;
  size_t si;
  if (!buf) buf = malloc(FBL); //todo check
  srand(time(NULL));

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
//  printf("Got:\n%s\n", buf);
  i = wc(buf) + 1; // number of spaces.
//  printf("There are %i spaces\n", i);
  i = rand() % i;
//  printf("We want the %ith\n", i);
  p = nth(buf, i); // to do: put \0 at end of word
//  printf("We got:%s\n", p);
  q = strndup(p, (strpbrk(p, " \n%.,'\"") - p));
  fclose(ifs);

  if (strlen(q) < MIN_LEN)
    return getFortune();
  else
    return q;
}
