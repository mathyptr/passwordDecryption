#ifndef UTIL_H
#define UTIL_H

#include <string>
using namespace std;

void SplashScreen();
void choosePwd(const string& filein,const string& fileout,string& password);
void checkPasswords(const string& filein, const string& fileout);
void buildFilePasswords(const string& filein, const string& fileout, const string& password, int& pos);

#endif // UTIL_H
