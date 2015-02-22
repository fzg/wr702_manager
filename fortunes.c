#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FBL (4096<<1)			// fortune line buffer length
#define MIN_LEN 5			// min length for match
#define DELIM " \n%.,'\"!\t?[](){};:"	// word delimiters

extern char gV;				// verbosity
static char *gFBuf;			// fortune line buffer

extern unsigned char data[] asm("_binary_f_off_start");
#ifdef __amd64__
#define data_loc (int)(data - 0x600000)
#else
#ifdef __mips__
#define data_loc 0x0001ac6
#else
#warning DATA_LOC NOT DEFINED
#endif
#endif
extern char *__progname;


static int wc(char *p) {
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

static char *nth(char *p, int i) {
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

static void rot13(char *p) {
  static char a, b;
  --p; while((a = *(++p))) *p=(((b=64^a&95)&&b<27)?a&224|(b+12)%26+1:a);
}

void cleanFortune() {
  if (gFBuf) free(gFBuf);
  gFBuf = NULL;
}

char *getFortune() {
  FILE *ifs = NULL;
  static char *buf = NULL, *p, *q;
  unsigned int i, j, sz;
  size_t si = FBL;
  if (!buf) {
   if (!(buf = (char*)malloc(FBL))) {
    perror("fortune malloc");
   }
   srand(time(NULL));
   gFBuf = buf;
   memset(buf, 0, sizeof(buf));
  }
//  if (!(ifs = fopen("./fortunes_off", "r"))) {
  if (!(ifs = fopen(__progname, "r"))) {
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
