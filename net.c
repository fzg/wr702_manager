#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#include "net.h"

//#define S(x) (sx))


extern char 	gP, gV, *gPass, *gUser, *gOb, *gProx;
extern struct addrinfo *gPaI;

char 		*gAddr = NULL, *gAuth = NULL;

struct in_addr *makeAddr(char *ip) {
	static struct in_addr x;

	if (gV > 1) printf("[makeAddr]\t%s\t", ip);
	inet_aton(ip, &x);
	gAddr = ip;
	return &x;
}

static char *encodeAuthString() {
	char *p = NULL, *q;
	size_t l;
	xmalloc(&p, S(gUser)+S(gPass)+2);
	strcpy(p, gUser);
	p[S(gUser)] = ':';
	strcpy(p+S(gUser)+1, gPass);
	q = base64_encode(p, S(p), &l);
	xfree(&p);
	return (q);
};

static int contact(char ok){//struct in_addr *x) {
        struct addrinfo hints, *res;
        int err, s;

        memset(&hints, 0, sizeof(hints));
	res = NULL;
        hints.ai_family = AF_INET;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_ADDRCONFIG;

	if (gP) {
		res = gPaI;
	} else {
        	if (err = getaddrinfo(gAddr, "80", &hints, &res)) perror("getaddrinfo");
	}
	if (!res) {
		puts("[contact]RES NULL!!!!");
		return s;
	}
	if ((err = s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
		perror("socket");
	else {
		if (!(gP && ok)) {	// FIXME: TODO: SAUF DANS LE CAS DE LA CONNEX AU PROXY
			if ((err = connect(s, res->ai_addr, res->ai_addrlen)) == -1) {
				puts(gai_strerror(err));
				perror("connect"); close(s); s = 0;
			}
		}
	}
	if (!gP) freeaddrinfo(res);
        if (gP) {
		char *ob = NULL;
		int r;
		// todo : envoyer la requète. Proxify quoi
		r = sendReq(gProx, strlen(gProx), &ob);
        }
	return s;
}

char *makeReq(const char *page, const char *req, size_t *sz) {
	const static char bp0[] = "GET http://", bp1[]="/userRpm/", bpst[] = "Rpm.htm";
        const static char ap0[] ="\r\nAuthorization: Basic ", el[] = "\r\n";
	const static char rp0[] = "User-Agent: Mozilla/5.0\r\nAccept: text/html\r\nAccept-Language: en;q=0.7\r\nReferer: http://";
	const static char ua[] = "\r\nConnection: keep-alive\r\n\r\n\r\n";//Cache-Control: max-age=0\r\n\r\n\r\n";
	const static char host[] = " HTTP/1.1\r\nHost: ";
	char *p = NULL;

	if (!gAuth) gAuth = encodeAuthString();
	if (gV > 1) printf("[makeReq]\tEnc\t\t%s\n", gAuth);
	*sz = S(bp0)+S(gAddr)+S(bp1)+S(page)+S(bpst)+S(req)+S(host)+S(gAddr)+S(el)+S(rp0)+S(gAddr)+S(bp1)+S(page)+S(bpst)+S(ap0)
		+S(gAuth)+S(ua)+ 2;
	xmalloc(&p, *sz);
	memset(p, 0, *sz);
	if (gV > 2) printf("[makeReq]\tAlloc:\t%i@%x\n",*sz,p);
	snprintf(p, *sz, "%s%s%s%s%s?%s%s%s%s%s%s%s%s%s%s%s%s", bp0, gAddr, bp1, page, bpst, req,
		host, gAddr, el, rp0, gAddr, bp1, page, bpst, ap0, gAuth, ua);
	if (gV) printf("[makeReq]\t-> %s\t%s\n", page, req);
	return p;
}

unsigned short bufToCode(const char buf[]) {
	static char b[4];
	static unsigned short r;

	b[3] = 0;
	memcpy(b, &buf[9], 3);
	r = atoi(b);
	if (gV > 1) printf("[bufToCode]\t<- HTTP #%i\n", r);
	return r;
}

unsigned short sendReq(const char *buf, size_t s, char **ob) {
		static short err = 200, socket;
		char *obuf = NULL, ret = 0;
		char oops = 0;
//		if (gP && buf[1] == 'O') puts(buf);
		if (gP && buf[1] == 'O') {// CONNECT
			oops = 1; gP = 0;
		}
        	if ((socket = contact(oops)) == -1 || socket == 0) {      // get socket
        	        err = EXIT_CONTACT;
			perror("contact");
			if (oops) gP = 1;
			return err;
        	}
		if (oops) gP = 1;
		xmalloc(&obuf, BL);
		*ob = obuf;
		if ((err = send(socket, buf, s, MSG_NOSIGNAL)) == -1) {
			perror("send"); exit(EXIT_FAILURE);
		}
		usleep(200); //FIXME: why?
		if (gV > 1) printf("s: %i\t", err);
		if ((err = recv(socket, obuf, BL, MSG_WAITALL)) < 0) {
			perror("recv"); exit(EXIT_FAILURE);
		}
		else {
			if (gV > 1) printf("r: %i\n", err);
			if (gV > 3) puts(obuf);
			err = bufToCode(obuf);
		}
		close(socket);
		if (err == 401) {
			if (++ret < 2) {
				puts("[sendReq]401: wrong username/password");
				tryReset(err);
			} else exit(EXIT_FAILURE);
		} else ret = 0;
		return err;
}

//RFC 2817
// host:port HTTP/1.1 to open tunnel


int		proxify(const char *ipprt) {
	const char *c_s_1 = "CONNECT ", *c_s_2 = " HTTP/1.1\r\nHost: ",
	 *c_s_3 = "\r\n";
	char *ibuf = NULL, *obuf = NULL;
	size_t	l, o;
	unsigned short tl = 0;
	int err = EXIT_SUCCESS, tt;

	if (!ipprt) return EXIT_FAILURE;
/*	if (xmalloc(&ibuf, (tt =S(c_s_1)+S(c_s_2)+S(c_s_3)+
		(S(ipprt)*2)+4))) {
			memset(ibuf, 0, tt);
			strcat(ibuf, c_s_1);
			strcat(ibuf + (tl += S(c_s_1)), ipprt);
        	        strcat(ibuf + (tl += S(ipprt)), c_s_2);
        	        strcat(ibuf + (tl += S(c_s_2)), ipprt);
        	        strcat(ibuf + (tl += S(ipprt)), c_s_3);
		sendReq(ibuf, strlen(ibuf), &obuf);
		printf("res: %s", obuf);
	}*/
	return err;
}

