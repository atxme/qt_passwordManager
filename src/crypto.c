#include "include/config.hpp"

// Définition de TAG_SIZE si ce n'est pas déjà fait
#ifndef TAG_SIZE
#define TAG_SIZE 16
#endif

#define IV_SIZE 16
#define EVP_MAX_BLOCK_LENGTH 32

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

void generateAesKey(unsigned char *aesKey) {
    RAND_bytes(aesKey, 32);
}

void generateIv(unsigned char *iv) {
    RAND_bytes(iv, IV_SIZE);
}

void generateSalt(unsigned char *salt) {
    RAND_bytes(salt, 16);
}



void hex2bin(const char *hex, unsigned char *bin, int len) {
    for (int i = 0; i < len; i += 2) {
        sscanf(&hex[i], "%2hhx", &bin[i / 2]);
    }
}

void bin2hex(const unsigned char *bin, char *hex, int len) {
    for (int i = 0; i < len; i++) {
        sprintf(&hex[i * 2], "%02x", bin[i]);
    }
}


char *base64_encode(const unsigned char *input, int length) {
    BIO *bio, *b64;
    FILE *stream;
    int encodedSize = 4 * ((length + 2) / 3);
    char *buffer = (char*)malloc(encodedSize + 1); // Convertir le pointeur void* en char*
    if (buffer == NULL) return NULL;

    stream = fmemopen(buffer, encodedSize + 1, "w");
    if (stream == NULL) {
        free(buffer);
        return NULL;
    }
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stream, BIO_NOCLOSE);
    BIO_push(b64, bio);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); 
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_free_all(b64);
    fclose(stream);
    return buffer;
}

unsigned char *base64_decode(const char *input) {
    BIO *bio, *b64;
    int decodeLen = strlen(input);
    unsigned char *buffer = (unsigned char*)malloc(decodeLen); // Convertir le pointeur void* en unsigned char*
    if (buffer == NULL) return NULL;

    bio = BIO_new_mem_buf(input, -1);
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); 
    bio = BIO_push(b64, bio);
    BIO_read(bio, buffer, decodeLen);
    BIO_free_all(bio);
    return buffer;
}

long int getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb"); 
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1; 
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Erreur lors de la recherche de la fin du fichier");
        fclose(file);
        return -1;
    }
    long int file_size = ftell(file);

    // Fermer le fichier
    fclose(file);

    return file_size;
}
void aesEncrypt(const unsigned char *input, const unsigned char *aes_key, const unsigned char *iv, unsigned char *output) {
    EVP_CIPHER_CTX *ctx;
    int len, input_len = strlen((const char*)input);

    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
    EVP_EncryptInit_ex(ctx, NULL, NULL, aes_key, iv);
    EVP_EncryptUpdate(ctx, output + IV_SIZE, &len, input, input_len);

    EVP_EncryptFinal_ex(ctx, output + IV_SIZE + len, &len);

    // Récupération du tag
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, output + IV_SIZE + len + input_len);
    
    // Copie du IV au début de la sortie
    memcpy(output, iv, IV_SIZE);

    EVP_CIPHER_CTX_free(ctx);
}

int aesDecrypt(const unsigned char *ciphertext, const unsigned char *aes_key, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len, plaintext_len;
    unsigned char tag[TAG_SIZE];
    unsigned char iv[IV_SIZE];
    int ciphertext_len = strlen((const char*)ciphertext) - TAG_SIZE - IV_SIZE;

    // Extraire le IV du début du texte chiffré
    memcpy(iv, ciphertext, IV_SIZE);

    // Extraire le tag du texte chiffré
    memcpy(tag, ciphertext + IV_SIZE + ciphertext_len, TAG_SIZE);

    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
    EVP_DecryptInit_ex(ctx, NULL, NULL, aes_key, iv);
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext + IV_SIZE, ciphertext_len);
    plaintext_len = len;

    // Configurer le tag pour la vérification
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAG_SIZE, tag);

    // Si la fonction suivante échoue, cela signifie que les données ont été altérées
    if (!EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;  // Erreur
    }
    plaintext_len += len;
    plaintext[plaintext_len] = '\0';

    EVP_CIPHER_CTX_free(ctx);
    return 0;  
}


void pbkdf2(const char *password, const unsigned char *salt, unsigned char *key) {
    PKCS5_PBKDF2_HMAC(password, strlen(password), salt, 16 , 100000, EVP_sha3_256(), 32, key);
}

/*
int main() {
    unsigned char aesKey[32];
    unsigned char iv[12];
    unsigned char encryptedData[4096]; // Supposez une taille maximale pour la démo
    unsigned char decryptedData[4096];

    // Générer une clé et un IV
    generateAesKey(aesKey);
    generateIv(iv);

    // Texte à chiffrer
    const char *originalData = "Ceci est un test pour le chiffrement AES-GCM!";

    printf("Texte original: %s\n", originalData);

    // Chiffrement
    aesEncrypt((unsigned char *)originalData, aesKey, iv, encryptedData);

    // Pour la démo, affichons les 60 premiers caractères du texte chiffré
    printf("Texte chiffré (60 premiers chars): ");
    for (int i = 0; i < 60; i++) {
        printf("%02x", encryptedData[i]);
    }
    printf("\n");

    // Déchiffrement
    if (aesDecrypt(encryptedData, aesKey, iv, decryptedData) == -1) {
        printf("Erreur lors du déchiffrement! Le texte a peut-être été altéré.\n");
        return 1;
    }

    printf("Texte déchiffré: %s\n", decryptedData);

    return 0;
}
*/

