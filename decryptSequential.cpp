#include "decryptSequential.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <chrono>
#include <cmath> 
#include "utilCrypt.h"
#include "util.h"

using namespace std;


std::string decryptDESwithDict(const std::string& encrypted_password,
    const std::string& salt, const vector<std::string>& password_list) {
    // Itera attraverso tutte le password nella lista
    for (const auto& word : password_list) {
//        std::string hash = cryptDES(word, salt);
        std::string hash = cryptDES(word,"prova");
        if (hash == encrypted_password) {
            return word; // Password trovata
        }
    }
    return "Password Not Found!";
}


testResult TestSeq(const string& password, const string& salt,vector<string> passwordList, int iter) {

    vector<double> execTimes;
    vector<int> positions;

//    string encrypted_password = cryptDES(password, salt);
    string encrypted_password = cryptDES(password,"prova");
    string pwdtmp;

    int block = passwordList.size() / iter;

    cout << "############################"<<endl; 
    cout << "##### Test Sequenziale #####"<<endl;

    for (int i = 0; i < iter; ++i) {
        cout << "############################"<<endl;
        cout << "Iter num: " << i + 1 << endl;

        pwdtmp=passwordList[block*i];
        passwordList[block*i]=password;

        string crypted_password =  cryptDES(password,"prova");
        auto start = chrono::high_resolution_clock::now();
        std::string pwddecrypted;
//        string pwddecrypted = decryptDESwithDict(encrypted_password, salt, passwordList);
        int pos=1;
        for (const auto& word : passwordList) {
            pwddecrypted = cryptDES(word,"prova");
            if (pwddecrypted == crypted_password) {
                break; // Password trovata
            }
            pos++;
        }
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;
        execTimes.push_back(elapsed.count());

        if (pwddecrypted == crypted_password) {
            cout << "Password trovata. " << endl;
            cout << "Tempo di esecuzione: " << elapsed.count() << "s." << endl;
            cout << "Posizione: " << pos << endl;
        }
        else {
            cout << ", Password non trovata!!!" << endl;
            cout << "Tempo di esecuzione: " << elapsed.count() << "s." << endl;
        }
        passwordList[block*i]=pwdtmp;
        }
    testResult seqr;
    seqr.max_time=maxTime(execTimes);
    seqr.min_time=minTime(execTimes);
    seqr.mean_time=meanTime(execTimes);
    seqr.stddev_time=stdDev(execTimes);
    seqr.num_password=passwordList.size();
    seqr.num_iter= iter;
    seqr.test_type=SEQUENTIAL;
    return seqr;
    }


