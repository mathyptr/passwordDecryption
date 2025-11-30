#include "decryptSequential.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <chrono>
#include <cmath> 
#include "utilCrypt.h"

using namespace std;


std::string decryptDESwithDict(const std::string& encrypted_password,
    const std::string& salt, const vector<std::string>& password_list) {
    // Itera attraverso tutte le password nella lista
    for (const auto& word : password_list) {
//        std::string hash = cryptDES(word, salt);
        std::string hash = cryptDES(word);
        if (hash == encrypted_password) {
            return word; // Password trovata
        }
    }
    return "Password Not Found!";
}


void TestSeq(const string& password, const string& salt,vector<string> passwordList, int iter) {

    vector<double> execution_times;
    vector<int> positions;

//    string encrypted_password = cryptDES(password, salt);
    string encrypted_password = cryptDES(password);
    string pwdtmp;

    int block = passwordList.size() / iter;

    cout << "############################"<<endl; 
    cout << "##### Test Sequenziale #####"<<endl;

    for (int i = 0; i < iter; ++i) {
        pwdtmp=passwordList[block*i];
        passwordList[block*i]=password;

        string crypted_password =  cryptDES(password);
        auto start = chrono::high_resolution_clock::now();
        std::string pwddecrypted;
//        string pwddecrypted = decryptDESwithDict(encrypted_password, salt, passwordList);
        int pos=1;
        for (const auto& word : passwordList) {
            pwddecrypted = cryptDES(word);
            if (pwddecrypted == crypted_password) {
                break; // Password trovata
            }
            pos++;
        }
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;
        execution_times.push_back(elapsed.count());

        if (pwddecrypted == crypted_password) {
            cout << "Iter num: " << i + 1 << ", Password trovata. " << endl;
            cout << "Tempo di esecuzione: " << elapsed.count() << "s." << endl;
            cout << "Posizione: " << pos << endl;
        }
        else {
            cout << "Iter num: " << i + 1 << ", Password non trovata!!!" << endl;
            cout << "Tempo di esecuzione: " << elapsed.count() << "s." << endl;
        }
        passwordList[block*i]=pwdtmp;
        }
    }


