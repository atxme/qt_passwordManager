#ifndef USER_VERIFICATION_HPP
#define USER_VERIFICATION_HPP

#include "config.hpp"

void createTables(sqlite3 *db);
void verifyFileExistance();

#endif