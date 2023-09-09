#include "include/config.hpp"

void ignore_signal(int sig) {
    NULL;
}

void shutdownRequested (int signal) {
    
    const char *filename = ABSOLUTE_PATH "database.db";
    long int size = getFileSize(filename);

    if (size != -1) {

        unsigned char *buffer = (unsigned char *)malloc(size);
        if (!buffer) {
            perror("Erreur lors de l'allocation de mémoire pour le tampon");
            return ;
        }

        FILE *file = fopen(filename, "rb");
        if (!file) {
            perror("Erreur lors de l'ouverture du fichier");
            free(buffer);
            return ;
        }

        size_t bytes_read = fread(buffer, 1, size, file);
        if (bytes_read != size) {
            perror("Erreur lors de la lecture des données binaires");
            free(buffer);
            fclose(file);
            return ;
        }

        unsigned char iv[IV_SIZE];
        unsigned char aesKey[32];
        unsigned char decryptedData[size];

        generateIv(iv);

        printf("IV : %s\n", base64_encode(iv, IV_SIZE));
        fflush(stdout);

        free(buffer);
        fclose(file);
        exit(EXIT_SUCCESS);
    } 
    
    else {
        printf("Impossible d'obtenir la taille du fichier '%s'\n", filename);
        exit(EXIT_FAILURE);
    }

    
}

void createDaemon(pid_t app_pid) {
    printf("Création du démon\n");
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {  
        pid_t sid = setsid();
        if (sid < 0) {
            exit(EXIT_FAILURE);
        }

        //close(STDIN_FILENO);
        //close(STDOUT_FILENO);
        //close(STDERR_FILENO);

        signal(SIGINT, shutdownRequested);   // Capture CTRL+C
        signal(SIGTERM, shutdownRequested);  // Capture l'arrêt du système

        while (1) {
            if (kill(app_pid, 0) != 0) {
                shutdownRequested(0);  // Si le processus parent est terminé, exécutez la fonction de chiffrement
            }
            sleep(2);
        }
    }
}