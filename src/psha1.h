#ifndef SHA1_H
#define SHA1_H
/* ================ sha1.h ================ */
/*
SHA-1 in C
By Steve Reid <steve@edmweb.com>
100% Public Domain
*/

typedef struct {
    unsigned int state[5];
	unsigned int count[2];
    unsigned char buffer[64];
} SHA1_CTX;

void plg_SHA1Transform(unsigned int state[5], const unsigned char buffer[64]);
void plg_SHA1Init(SHA1_CTX* context);
void plg_SHA1Update(SHA1_CTX* context, const unsigned char* data, unsigned int len);
void plg_SHA1Final(unsigned char digest[20], SHA1_CTX* context);

#ifdef REDIS_TEST
int plg_sha1Test(int argc, char **argv);
#endif
#endif
