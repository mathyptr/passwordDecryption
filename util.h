#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

enum testType { SEQUENTIAL, PARALLEL };

struct testResult {
    int threadNum=1;
    int chunkSize=0;
    double mean_time;
    double stddev_time;
    double max_time;
    double min_time;
    int num_password;
    int num_iter;
    testType  test_type;
};


void SplashScreen();
void SplashResult(string& title,std::vector<testResult>& seqr);
void saveResultToFile(const std::string& filename,std::vector<testResult>& result);
void choosePwd(vector<string>& passwordList,string& password);
void buildFilePasswords(const string& filein, const string& fileout);
std::vector<std::string> loadPasswords(const std::string& filepwd);
double minTime(std::vector<double> execTimes);
double maxTime(std::vector<double> execTimes);
double meanTime(std::vector<double> execTimes);
double stdDev(std::vector<double> execTimes);
#endif // UTIL_H
