#include <string.h>
#include "ops.h"

char *gOb = NULL;
extern char *gAddr, *gUser, *gPass, *gAuth, gV;

void waitForReboot() {
  int s, d=11;
  if (gV) printf("[Reboot]\tWaiting %d' ",d);
     xsleep(d);
}

int oReset() {
    size_t sz;
    int err;
    char *ob, *p = makeReq("RestoreDefaultCfg", "RestoreFactory=Restore", &sz);
    err = sendReq(p, strlen(p), &ob);
    free(p);
    return err;
}

int oReboot() {
    size_t sz;
    int err = EXIT_FAILURE;
    char *ob, *p = makeReq("SysReboot", "Reboot=Reboot", &sz);
    err = sendReq(p, strlen(p), &ob);
    free(p);
    return err;
}

int oPsk(const char *psk) {
    const char it[] = "Save=Save&pskSecOpt=3&pskCipher=1&interval=86400&sectype=3&intervalWpa=86400&broadcast=2&pskSecret=";
    size_t sz, st; int err = EXIT_FAILURE;
    char *p, *t;
    if (strlen(psk) < 8)
	puts("WARNING: PSK shorter than 8 characters will be IGNORED");
    if (strlen(psk) > 63)
        puts("WARNING: PSK longer than 63 characters will be IGNORED");

    if (!(t = malloc((st = strlen(it) + strlen(psk) + 1)))) {
	perror("oPsk malloc"); return err;
    }
    snprintf(t, st, "%s%s", it, psk);
    p = makeReq("WlanSecurity", t, &sz);
    err = sendReq(p, strlen(p), &gOb);
    free(p); free(t);
    return err;
}

int oSsid(const char *ssid) {
    const char it[] = "Save=Save&broadcast=2&ap=1&ssid1=";
    size_t sz, st; int err = EXIT_FAILURE;
    char *ob, *t, *p;
    if (!(t = malloc((st = strlen(it) + strlen(ssid) + 1)))) {
        perror("oSsid malloc"); return err;
    }
    snprintf(t, st, "%s%s", it, ssid);
    p = makeReq("WlanNetwork", t, &sz);
    err = sendReq(p, strlen(p), &gOb);
    free(p);
    free(t);
    return err;
}

int oSurvey(char **res) {
    size_t sz;
    int err = EXIT_FAILURE;
    char *t, *u, *ob = NULL, *p = makeReq("WlanStation", "Page=1", &sz);
    err = sendReq(p, strlen(p), &ob);
    *res = ob;
    free(p);
    if (ob && (t= strstr(ob, "wlanHostPara"))) {
	u = index(t, '\n');
        printf("Currently connected users: %d\n", atoi(u));

        t = index((t = strstr(ob, "hostList")), '\n');      // debut de liste
        u = strstr(t, "</SCRIPT>") - 1;
        *u = 0;
        printf("HostList: %s\n", t, '\n');
    }
    return err;
}

int oMode(const short m) {
    size_t sz;
    int err = EXIT_FAILURE;
    char om[] = "Save=Save&opmode=c";
    *(index(om, 'c')) = m+'0';
    char *ob, *p = makeReq("WlanApMode", om, &sz);
    err = sendReq(p, strlen(p), &ob);
    free(p);    //xfree(&ob);
    return err;
}

int oPwd(const char *ou, const char *op, const char *nu, const char *np) {
    size_t sz;
    int err = EXIT_FAILURE;
    char fs[] = "Save=Save&oldname=%s&newname=%s&oldpassword=%s&newpassword=%s";
    char *ob, *q, *p;
    sz = strlen(fs)-(2*4)+1+strlen(ou)+strlen(op)+strlen(nu)+strlen(np);
    q = malloc(sz);
    snprintf(q, sz, fs, ou, nu, op, np);
    p = makeReq("ChangeLoginPwd", q, &sz);
    err = sendReq(p, strlen(p), &ob);
    xstrdup(&gUser, nu);
    xstrdup(&gPass, np);
    free(p); free(q);
    return err;
}

int oIp(const char *ip, const char *nm, const short m) {
    size_t sz;
    int err = EXIT_FAILURE;
    char fs[] = "Save=Save&lanip=%s&inputMask=%s&lanmask=1&porttype=%d";
    char *ob, *q, *p;

    sz = strlen(fs)-(2*3)+1+strlen(ip)+strlen(nm)+1;
    q = malloc(sz);
    snprintf(q, sz, fs, ip, nm, m);
    p = makeReq("NetworkCfg", q, &sz);
    err = sendReq(p, strlen(p), &ob);
    if (strncmp(ip, gAddr, max(strlen(ip), strlen(gAddr)))) {
       xstrdup(&gAddr, ip);
    }
    //xfree(&ob);
    free(p); free(q);
    return err;

}

int tryReset(int err) {
  char *dUser="admin", *dHost="192.168.0.254";
  char *oPass, *oUser, *oHost;
  int merr = 0;

  oPass = oUser = oHost = NULL;
  if (gV) puts("This assumes the router was reset.\nReset and relaunch if you're impatient.");
  xfree(&gAuth);
  xstrdup(&oPass, gPass); oUser = strdup(gUser);
  xstrdup(&oHost, dHost);
  xstrdup(&gPass, dUser); xstrdup(&gUser, dUser);
  if (err != 401) {
   merr = oIp(oHost, "255.255.255.0", MODE_STATIC);	// WR:  reset ip to whished ip
   if (merr == 65535) {
     goto cleanup;
   }
   gAddr = oHost;					// app: same
   askReboot();
   xstrdup(&oHost, gAddr);
   gAddr = dHost;
  }
  merr = oPwd(dUser, dUser, oUser, oPass);	// reset password ( on new ip)
cleanup:
  xstrdup(&gAddr, oHost);                                       // app: same
  xstrdup(&gPass, oPass); // pb
  xstrdup(&gUser, oUser); // pb
  xfree(&oHost);
  xfree(&gAuth);			// for new http auth
  xfree(&oPass); xfree(&oUser);
  if (err == 401) xfree(&oHost);
  return merr;
}
