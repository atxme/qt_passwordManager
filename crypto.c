#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>


void sha3_512(const unsigned char *input, char *output) {
    unsigned char digest[64];
    EVP_MD_CTX *mdctx;

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha3_512(), NULL);
    EVP_DigestUpdate(mdctx, input, strlen((char *)input));
    EVP_DigestFinal_ex(mdctx, digest, NULL);
    EVP_MD_CTX_free(mdctx);

    for (int i = 0; i < 64; i++) {
        sprintf(output + (i * 2), "%02x", digest[i]);
    }
}

void generateAesKey(char *aesKey) {
    unsigned char key[32];
    RAND_bytes(key, sizeof(key));
    for (int i = 0; i < 32; i++) {
        sprintf(aesKey + (i * 2), "%02x", key[i]);
    }
}

void generateIv(char *iv) {
    unsigned char iv2[16];
    RAND_bytes(iv2, sizeof(iv2));
    for (int i = 0; i < 16; i++) {
        sprintf(iv + (i * 2), "%02x", iv2[i]);
    }
}

void aesEncrypt(const char *data, const char *aesKey, const char *iv, char *output) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    unsigned char ciphertext[128];
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_xts(), NULL, (const unsigned char *)aesKey, (const unsigned char *)iv);
    EVP_EncryptUpdate(ctx, ciphertext, &len, data, strlen((char *)data));
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;
    for (int i = 0; i < ciphertext_len; i++) {
        sprintf(output + (i * 2), "%02x", ciphertext[i]);
    }
    EVP_CIPHER_CTX_free(ctx);
}

char *base64_encode(const unsigned char *input, int length) {
    BIO *bmem, *b64;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    char *base64_output = (char *)malloc(bptr->length + 1);
    if (base64_output) {
        memcpy(base64_output, bptr->data, bptr->length);
        base64_output[bptr->length] = '\0';
    }

    BIO_free_all(b64);

    return base64_output;
}

long getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erreur à l'ouverture du fichier");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

int main() {
    char hash_output[129];
    char aes_key[65];
    char iv[33];
    unsigned char encrypted_data[2048];  // Utilisez unsigned char pour les données chiffrées

    const char *message = "bobooooooooooooooooooooooooooooo";

    sha3_512((unsigned char *)message, hash_output);
    printf("SHA3-512 hash: %s\n", hash_output);

    generateAesKey(aes_key);
    generateIv(iv);

    aesEncrypt((unsigned char *)message, aes_key, iv, encrypted_data);

    char *base64_encrypted_data = base64_encode(encrypted_data, 128);
    printf("AES-XTS encrypted data (base64): %s\n", base64_encrypted_data);
    free(base64_encrypted_data);

    return 0;
}
