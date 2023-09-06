#include "dependiancies.h"


void shutdownRequested (){
    //encrypt database 
    //clear system
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