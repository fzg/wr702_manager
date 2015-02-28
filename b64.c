#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static const char enc_t[] = 	{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *dec_t = NULL;
static const int mod_t[] = {0, 2, 1};


char *base64_encode(const unsigned char *d, size_t il, size_t *ol) {

    *ol = 4 * ((il + 2) / 3);
    int i, j;
    char *ed;
    if (!(ed = malloc(*ol + 1))) return NULL;

    memset(ed, 0, *ol + 1);
    for (i = 0, j = 0; i < il;) {

        uint32_t octet_a = i < il ? (unsigned char)d[i++] : 0;
        uint32_t octet_b = i < il ? (unsigned char)d[i++] : 0;
        uint32_t octet_c = i < il ? (unsigned char)d[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        ed[j++] = enc_t[(triple >> 3 * 6) & 0x3F];
        ed[j++] = enc_t[(triple >> 2 * 6) & 0x3F];
        ed[j++] = enc_t[(triple >> 1 * 6) & 0x3F];
        ed[j++] = enc_t[(triple >> 0 * 6) & 0x3F];
    }

    for (i = 0; i < mod_t[il % 3]; i++)
        ed[*ol - 1 - i] = '=';
    return ed;
}

void build_decoding_table();

unsigned char *base64_decode(const char *d, size_t il, size_t *ol) {
    int i, j;
    unsigned char *dd;

    if (!dec_t) build_decoding_table();
    if (il % 4) return NULL;
    *ol = il / 4 * 3;
    if (d[il - 1] == '=') --(*ol);
    if (d[il - 2] == '=') --(*ol);
    if (!(dd = malloc(*ol))) return NULL;
    for (i = 0, j = 0; i < il;) {
        uint32_t sextet_a = d[i] == '=' ? 0 & i++ : dec_t[d[i++]];
        uint32_t sextet_b = d[i] == '=' ? 0 & i++ : dec_t[d[i++]];
        uint32_t sextet_c = d[i] == '=' ? 0 & i++ : dec_t[d[i++]];
        uint32_t sextet_d = d[i] == '=' ? 0 & i++ : dec_t[d[i++]];
        uint32_t t = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);
        if (j < *ol) dd[j++] = (t >> 2 * 8) & 0xFF;
        if (j < *ol) dd[j++] = (t >> 1 * 8) & 0xFF;
        if (j < *ol) dd[j++] = (t >> 0 * 8) & 0xFF;
    }
    return dd;
}


void build_decoding_table() {
    int i = -1;
    xmalloc((void**)&dec_t, 256);
    while (++i < 64) dec_t[(unsigned char) enc_t[i]] = i;
}


void base64_cleanup() {
    xfree(&dec_t);
}
