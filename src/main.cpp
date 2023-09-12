#include "include/config.hpp"

bool isRegistered = false;
bool isLogged = false;

void parentSignalHandler(int signal) {
    printf("Le processus principal a reçu le signal SIGINT\n");
    fflush(stdout);
    exit(EXIT_SUCCESS);
}

void writeOnFile (std::string filename,std::string content) {
    std::string path = ABSOLUTE_PATH + filename;
    std::ofstream file;
    file.open(path);
    file << content;
    file.close();
}

int main() {   

    if (geteuid() != 0) {
        std::cerr << "L'application doit être exécutée avec des privilèges root (sudo)." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    verifyFileExistance();

    while (isLogged == false){
        if (isRegistered == true){
            loginWindow login;
            login.connectUser();
            break;
        }
        else {
            createWindow create;
            create.createUser();
            if (isRegistered == true){
                loginWindow login;
                login.connectUser();
                break;
            }
        }
    }
    
    if (isLogged){
        std::cout << "welcome" << std::endl;
        signal(SIGINT, parentSignalHandler);
        
    }
    createDaemon(getpid()); 
    return 0;   
}
