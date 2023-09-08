#ifndef DAEMON_H
#define DAEMON_H

void shutdownRequested();
void createDaemon(pid_t app_pid);

#endif