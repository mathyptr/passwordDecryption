#ifndef DECRYPT_SEQUENTIAL_H
#define DECRYPT_SEQUENTIAL_H

#include <string>
#include <vector>
#include "util.h"

using namespace std;

SequentialRes TestSeq(const std::string& password,const std::string& salt,std::vector<std::string> passwordList,int iter);

#endif // DECRYPT_SEQUENTIAL_H
