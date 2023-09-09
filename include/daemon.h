#ifndef DAEMON_H
#define DAEMON_H

void ignore_signal(int sig) ;
void shutdownRequested(int signal);
void createDaemon(pid_t app_pid);

#endif