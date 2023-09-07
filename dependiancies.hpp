
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

// System dependiancies
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Path: qt.h

//#include <QApplication>



// Path: path files
#include "userVerification.hpp"
#include "daemon.h"

//variables 
extern std::string username ;

#define ABSOLUTE_PATH "/root/.myapp/"
