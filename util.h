#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

struct SequentialRes {
    double mean_time;
    double stddev_time;
    double max_time;
    double min_time;
};


void SplashScreen();
void SplashResult(string& title,SequentialRes& seqr);
void choosePwd(vector<string> passwordList,string& password);
void buildFilePasswords(const string& filein, const string& fileout);
//void buildFilePasswords(const string& filein, const string& fileout, const string& password, int& pos);
std::vector<std::string> loadPasswords(const std::string& filepwd);
double minTime(std::vector<double> execTimes);
double maxTime(std::vector<double> execTimes);
double meanTime(std::vector<double> execTimes);
double stdDev(std::vector<double> execTimes);
#endif // UTIL_H
