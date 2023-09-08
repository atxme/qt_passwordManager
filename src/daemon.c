#include "hearderFiles/dependiancies.hpp"



void shutdownRequested (){
    const char * filename = ABSOLUTE_PATH+"databse.db";
    long int size = getFileSize(filename);

    if (size != -1) {
        printf("La taille du fichier '%s' est : %ld octets\n", filename, size);

        unsigned char *buffer = (unsigned char *)malloc(size);
        if (!buffer) {
            perror("Erreur lors de l'allocation de mémoire pour le tampon");
            return 1;
        }

        FILE *file = fopen(filename, "rb");
        if (!file) {
            perror("Erreur lors de l'ouverture du fichier");
            free(buffer);
            return 1;
        }

        size_t bytes_read = fread(buffer, 1, size, file);
        if (bytes_read != size) {
            perror("Erreur lors de la lecture des données binaires");
            free(buffer); 
            fclose(file);
            return 1;
        }

        
        unsigned char iv [IV_SIZE];
        unsigned char aesKey [32];
        unsigned char decryptedData [size];

        generateIv(iv);



        free(buffer);
        fclose(file);
    } 
    
    else {
        printf("Impossible d'obtenir la taille du fichier '%s'\n", filename);
    }

    return 0;

}

void createDaemon (pid_t app_pid){
    pid_t pid, sid;
    pid = fork();

    pid < 0 ? exit(EXIT_FAILURE) : 0;
    
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    sid =setid();
    sid < 0 ? exit(EXIT_FAILURE) : 0;

    close (STDIN_FILENO);
    close (STDOUT_FILENO);
    close (STDERR_FILENO);

    int request_fd = open ("shutdown-request.txt", O_RDONLY);
    signal (SIGTERM, shutdownRequested);

    while(1){
        if (request_fd >= 0){
            close (request_fd);
            shutdownRequested();
        }

        if (kill (app_pid, 0) != 0){
            exit (EXIT_FAILURE);
        }
        sleep (5);
    }
}