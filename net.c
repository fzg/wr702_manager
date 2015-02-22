#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

#include "net.h"

#define S(x) (strlen(x))

extern char 	gV, *gPass, *gUser, *gOb;
char 		*gAddr = NULL, *gAuth = NULL;

struct in_addr *makeAddr(char *ip) {
	static struct in_addr x;

	if (gV) printf("[%s]\t-> ", ip);
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
	free(p);
	return (q);
};

static int contact(struct in_addr *x) {
        struct addrinfo hints, *res;
        int err, s;

        memset(&hints, 0, sizeof(hints));
	res = NULL;
        hints.ai_family = AF_INET;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_ADDRCONFIG;
        if (err = getaddrinfo(gAddr, "80", &hints, &res)) perror("getaddrinfo");
	if ((err = s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
		perror("socket");
	else {
//		printf("will connect to %s\n", gAddr);
		if ((err = connect(s, res->ai_addr, res->ai_addrlen)) == -1) {
			perror("connect"); close(s); s = 0;
		}
	}
	freeaddrinfo(res);
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
	if (gV > 1) printf("Encoded to: %s\n", gAuth);
	*sz = S(bp0)+S(gAddr)+S(bp1)+S(page)+S(bpst)+S(req)+S(host)+S(gAddr)+S(el)+S(rp0)+S(gAddr)+S(bp1)+S(page)+S(bpst)+S(ap0)
		+S(gAuth)+S(ua)+ 2;
	xmalloc(&p, *sz);
	memset(p, 0, *sz);
	if (gV > 1) printf("Allocated %i@%x\n",*sz,p);
	snprintf(p, *sz, "%s%s%s%s%s?%s%s%s%s%s%s%s%s%s%s%s%s", bp0, gAddr, bp1, page, bpst, req,
		host, gAddr, el, rp0, gAddr, bp1, page, bpst, ap0, gAuth, ua);
	if (gV) printf("%s:%s\n", page, req);
	return p;
}

unsigned short bufToCode(const char buf[]) {
	static char b[4];
	static unsigned short r;

	b[3] = 0;
	memcpy(b, &buf[9], 3);
	r = atoi(b);
	if (gV) printf("Got back code %i\n", r);
	return r;
}

unsigned short sendReq(const char *buf, size_t s, char **ob) {
		static short err = 200, socket;
		static char *obuf = NULL, ret = 0;
        	if ((socket = contact(makeAddr(gAddr))) == -1 || socket == 0) {      // get socket
        	        err = EXIT_CONTACT;
			perror("contact");
			return err;
        	}
		xmalloc(&obuf, BL);
		*ob = obuf;
		if ((err = send(socket, buf, s, MSG_NOSIGNAL)) == -1) {
			perror("send"); exit(EXIT_FAILURE);
		}
		usleep(200); //FIXME: why?
		if (gV) printf("sent:%i ", err);
		if ((err = recv(socket, obuf, BL, MSG_WAITALL)) < 0) {
			perror("recv"); exit(EXIT_FAILURE);
		}
		else {
			if (gV) printf("recd:%i \n", err);
			if (gV > 1) puts(obuf);
			err = bufToCode(obuf);
		}
		close(socket);
		if (err == 401) {
			if (++ret < 2) {
				puts("401: wrong username/password");
				tryReset(err);
			} else exit(EXIT_FAILURE);
		} else ret = 0;
		return err;
}
