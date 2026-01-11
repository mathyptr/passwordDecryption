#include "decryptSequential.h"

#include <iostream>
#include <vector>
#include <chrono>
#include "utilCrypt.h"
#include "util.h"

using namespace std;

//**************************************************************************************************
//Funzione che verifa la presenza della password scelta all'interno del dizionario delle password
//**************************************************************************************************
int checkPwd(const std::string& crypted_password,const std::string& salt, const vector<std::string>& passwordList){
  std::string pwddecrypted;
  int pos=1;
  for (const auto& word : passwordList) {
      pwddecrypted =  cryptDES(word,salt);
      if (pwddecrypted == crypted_password) {
        return pos; // La password è stata trovata e ritorno la poszione
      }
      pos++;
  }
  return -1; // La password non è stata trovata
}

testResult TestSeq(const string& password, const string& salt,vector<string> passwordList, int iter) {

    vector<double> execTimes;
    vector<int> positions;
    string pwdtmp;

    string encrypted_password = cryptDES(password, salt);


    int block = passwordList.size() / iter;
    int pospwd=block/2;

    cout << "############################"<<endl; 
    cout << "##### Test Sequenziale #####"<<endl;

    for (int i = 0; i < iter; ++i) {
        cout << "############################"<<endl;
        cout << "Iter num: " << i + 1 << endl;

        pwdtmp=passwordList[block*i+pospwd];
        passwordList[block*i+pospwd]=password;

        string crypted_password =  cryptDES(password,salt);

        auto start = chrono::high_resolution_clock::now();
        int pos= pos=checkPwd(crypted_password,salt,passwordList);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = end - start;
        execTimes.push_back(elapsed.count());

        if (pos != -1) {
            cout << "Password trovata. " << endl;
            cout << "Tempo di esecuzione: " << elapsed.count() << "s." << endl;
            cout << "Posizione: " << pos << endl;
        }
        else {
            cout << ", Password non trovata!!!" << endl;
            cout << "Tempo di esecuzione: " << elapsed.count() << "s." << endl;
        }
        passwordList[block*i+pospwd]=pwdtmp;
    }
//Salvo le statistiche in un vettore
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


