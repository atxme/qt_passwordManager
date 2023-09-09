#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

// Path: oppenSSL.h
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>

// Path: dependiancies.h
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <signal.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// System dependiancies
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// Path: qt.h

//#include <QApplication>

// Path: path files
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


#endif 