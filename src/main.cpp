#include "include/config.hpp"


void parentSignalHandler(int signal) {
    printf("Le processus principal a reçu le signal SIGINT\n");
    fflush(stdout);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {   

    if (geteuid() != 0) {
        std::cerr << "L'application doit être exécutée avec des privilèges root (sudo)." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    verifyFileExistance();

    signal(SIGINT, parentSignalHandler);
    createDaemon(getpid());  

    
    
    return 0;   
}
