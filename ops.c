#include "ops.h"

char *gOb;
extern char *gAddr, *gUser, *gPass, *gAuth, gV;

void waitForReboot() {
  int s, d=11;
  if (gV) printf("Waiting %dsec for reboot ",d);
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
    free(p); free(t);
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
    free(p);
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
    if (gUser) free(gUser);
    if (gPass) free(gPass);
    gUser = strdup(nu); //TODO FIXME
    gPass = strdup(np); //TODO FIXME
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
       gAddr = strdup(ip);
    }
    free(p); free(q);
    return err;

}

int tryReset(int err) {
  char *dUser="admin", *dHost="192.168.0.254";
  char *oPass, *oUser, *oHost;

  if (gV) puts("This assumes the router was reset.\nReset and relaunch if you're impatient.");
  if (gAuth) {
    free(gAuth);
    gAuth = NULL;
  }					// for new http auth
  oPass = strdup(gPass); oUser = strdup(gUser);
  gPass = dUser; gUser = dUser;
  if (err != 401) {
   oIp(oHost, "255.255.255.0", MODE_STATIC);	// WR:  reset ip to whished ip
   gAddr=oHost;					// app: same
   waitForReboot();				// give it time to reset
   oHost = strdup(gAddr);
   gAddr = dHost;
  }
  oPwd(dUser, dUser, oUser, oPass);		// reset password ( on new ip)
  if (gAuth) free(gAuth);			// for new http auth
  gAuth = NULL;
  free(oPass); free(oUser);
  if (err == 401) free(oHost);
  return 0;
}
