#include "dependiancies.h"



void verifyFileExistance (){
    std::ifstream file (ABSOLUTE_PATH + "database.db");
    if (!file){
        
    }
    file.close();
}

