#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

void SplashScreen();
void choosePwd(vector<string> passwordList,string& password);
void buildFilePasswords(const string& filein, const string& fileout);
//void buildFilePasswords(const string& filein, const string& fileout, const string& password, int& pos);
std::vector<std::string> loadPasswords(const std::string& filepwd);
#endif // UTIL_H
