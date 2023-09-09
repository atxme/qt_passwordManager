#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iostream>
#include <string>
#include <cstring> 
#include <cstdio>  
#include <openssl/err.h>

#define TAG_SIZE 16
#define IV_SIZE 16

void sha3_512(const unsigned char *input, char *output);
void generateAesKey(unsigned char *aesKey);
void generateIv(unsigned char *iv);
void hex2bin(const char *hex, unsigned char *bin, int len);
void bin2hex(const unsigned char *bin, char *hex, int len);
char *base64_encode(const unsigned char *input, int length);
unsigned char *base64_decode(const char *input);
long int getFileSize(const char *filename);
void aesEncrypt(const unsigned char *input, const unsigned char *aes_key, const unsigned char *iv, unsigned char *output);
int aesDecrypt(const unsigned char *ciphertext, const unsigned char *aes_key, const unsigned char *iv, unsigned char *plaintext);

#endif
