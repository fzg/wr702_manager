#ifndef _B64_H_
#define _B64_H_
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);
void build_decoding_table();

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);
void build_decoding_table();
void base64_cleanup();
#endif
