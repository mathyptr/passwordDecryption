#ifndef UTIL_CRYPT_H
#define UTIL_CRYPT_H

using namespace std;

string cryptDES(const std::string& password, const std::string& salt);
int decryptDES(void);
#endif // UTIL_CRYPT_H