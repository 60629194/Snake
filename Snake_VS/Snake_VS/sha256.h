#pragma once
#ifndef SHA256_H
#define SHA256_H
#include <stddef.h>
#define SHA256_BLOCK_SIZE 32 
typedef unsigned char BYTE;
typedef unsigned int  WORDS;
typedef struct {
	BYTE data[64];
	WORDS datalen;
	unsigned long long bitlen;
	WORDS state[8];
} SHA256_CTX;
void sha256_init(SHA256_CTX* ctx);
void sha256_update(SHA256_CTX* ctx, const BYTE data[], size_t len);
void sha256_final(SHA256_CTX* ctx, BYTE hash[]);
#endif