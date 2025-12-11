#ifndef DECRYPT_PARALLEL_H
#define DECRYPT_PARALLEL_H

#include <string>
#include <vector>
#include "util.h"

using namespace std;
std::string ompDecryption(const std::string& encrypted_password, const std::string& salt,const std::vector<std::string>& pwdList, int threadNum, int chunkSize);
std::vector<testResult> testPar(const std::string& password, const std::string& salt,std::vector<std::string> pwdList, const std::vector<int>& thread_counts, int executions);
#endif // DECRYPT_PARALLEL_H
