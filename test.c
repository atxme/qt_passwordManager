#include "crypto.h"
#include "dependiancies.hpp"

int main(){
    char * data = "hello world";
    char * dataEncrypted;
    sha_512(data, dataEncrypted);
    std::cout << dataEncrypted << std::endl;
    return 0;
}