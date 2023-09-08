
// Path: oppenSSL.h
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>

// Path: dependiancies.h
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <signal.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <stdio.h>

// System dependiancies
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Path: qt.h

//#include <QApplication>

// Path: path files
#include "include/dependiancies.hpp"
#include "include/daemon.h"
#include "include/crypto.h"
#include "include/userVerification.hpp"


//variables 

#define ABSOLUTE_PATH "/var/Pegasus/"
#define EVP_MAX_BLOCK_LENGTH 32


extern std::string userID;
extern int tempFileLenght ;
extern char tempKey [32];
extern char tempAesKey [32];




