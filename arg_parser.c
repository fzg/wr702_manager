#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "arg_parser.h"
#include "opts.h"

extern char gV;

t_arg args[] = {
{0, 0, &setVerbosity, 0, "-v"},
{1, 0, &setIP, 0,  "ip"},   // ip of device [NO CHANGE ON DEVICE]
{2, 0, &setUser, 0, "auth"},
{1, 0, &setMode, 1, "mode"},
{1, 0, &setSSID, 0, "ssid"},
{1, 0, &setNPwd, 0, "pass"},
{1, 0, &setNUser, 0, "user"},
{1, 0, &setPsk, 0, "psk"},
{0, 0, &setRandomSSID, 0, "rssid"},
{0, 0, &setRandomPsk, 0, "rpsk"}

};

t_default_arg defaults[] = {
{ARG(1), 0, "192.168.0.254"},
{ARG(2), 0, "admin"},
{ARG(2), 1, "admin"}};

void override_default_arg(const t_arg *a, int i, char *val) {
  t_default_arg *p = &defaults[0];
  while (p && a != p->a && p->i != i) ++p;
  if (p && i == p->i) {
   p->val = val;
  }
  else {
   puts("[OverrideDefaultArg]\t PROBLEM");
   exit(1);
  }
}

const t_arg *match_arg(const char *a) {
  int i;
  if (gV > 2) printf("[Match arg]\t\"%s\"\n\t\t->", a);
  for (i = 0; i < sizeof(args)/sizeof(t_arg); ++i) {
//   printf("%i %i strcmp %s %s\n", i, sizeof(args)/sizeof(t_arg), args[i].aname, a);
   if(!strcmp(args[i].aname, a)) {
     if (gV > 2) printf("Good arg\n");
     return &args[i];
   }
  }
  if (gV > 2) printf("Bad arg\n");
  return (t_arg*)NULL;
}

char isOp(const char **a) {
  char ret = (size_t)match_arg(*a);
  if (gV > 2) printf("[isOp]\t\t%s:\t%d\n", *a, ret);
  return (ret);
}

char *get_default(const t_arg *a, int i) {
  t_default_arg *p = &defaults[0];
  while (p && a != p->a && p->i != i);
  if (p && p->i == i) {
	return (p->val);
  }
  return (char *)NULL;
}

#define SZ_APTR (a->ops_n * sizeof(char*))

int parse_arg(const char **v, int i, int *err) {
 int   consumed = 1, n=1;
 const t_arg *a = (t_arg*)NULL;
 static char **aptr = NULL;
 if (!(a = match_arg(v[i]))) {
  if (1 || gV) printf("[ParseArg]\tInvalid arg %s\n", v[i]);
  *err = 1; return 1;
 } else {
// TODO: si il y a des operandes, check que les ops_f suivants
//       ne sont pas des operations. sinon erreur
//       si ca joue, mais que les facultatifs ne sont pas tous presents
//       chopper le default(overridable dans les appels precedents)[cmd][i]
//	 return le callback
  if (a->ops_n) {
   if (gV > 2) printf("[ParseArg]\tAllocated %dbytes\n", SZ_APTR);
   xmalloc(&aptr, SZ_APTR);
   memset(aptr, 0, SZ_APTR);
  }
  while (n-1 < a->ops_n) {
   ++ consumed;
   if ((!v[i+n] || isOp(&v[i+n]))&& n-1 < a->ops_m) {
    printf("[ParseArg]\tMissing mandatory argument for %s\n", a->aname);
    exit(1);
   }
   //xstrdup(&aptr[n-1], v[i+n]);
   if ( n -1 >= a->ops_m && isOp(&(v[i+n]))) {
    puts("[ParseArg]\tOOPS GOT OP INSTEAD OF OPERAND");
    *err = 2; return(1);
   }
   if (v[i+n]) xstrdup(&aptr[n-1], v[i+n]); else {
     xstrdup(&(aptr[n-1]), get_default(a, n-1));
   }
  ++n;
  }
 }
 if (gV > 2 && aptr) {
  for (i = 0; i < a->ops_n; ++i) {
  printf("args [%d]\t%s\n", i, aptr[i]);
  }
 }
 // WE DO THE CALLBACK HERE
  if (1 && !(a->handler)) puts("[ParseArg]\tFIXME: UNIMPLEMENTED HANDLER. PREPARE FOR CRASH");
  *err = a->handler(aptr);
  if (aptr) for (i = 0; i < a->ops_n; ++i) xfree(&(aptr[i]));
  xfree(&aptr);
  if (a->n_rbt)
    waitForReboot();
 // DONNNNNNNNNNNNNNNNNNNNE

 if (gV > 2) printf("[ParseArg]\tAte %dw\n", consumed);
 return consumed;
}

int	parse_args(int c, const char **v) {
 int	i, err, n;

 i = err = 0;
 while((!err || err == 200) && i < c - 1) {
  i += (n = parse_arg(v, i+1, &err));
  if (gV) printf("[ParseArgs]\t%s\t\terr=%d\targ=%d\n", v[i-n+1], err, i);
 }
 return err;
}
