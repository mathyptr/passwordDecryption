#include "util.h"
#include <fstream>
#include <filesystem> 
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <cmath>
#include <numeric>

using namespace std;

void SplashScreen() {
    cout << "############################"<<endl;
    cout << "Questo programma implementa un attacco brute-force per"<< endl;
    cout << "decifrare password di otto caratteri crittografate con DES"<< endl;
    cout << "confrontando i costi computazionali di una versione sequentail"<< endl;
    cout << "e di una versione parallela di un attacco brute-force alle password,"<< endl;
    cout << "valutando la velocità ottenuta in diverse situazioni di test"<< endl;
    cout << "tramite parallelizzazione."<< endl;
    cout << "############################"<<endl;
}

void SplashResult(string& title,SequentialRes& seqr) {
    cout << "############################"<<endl;
    cout << title<< endl;
    cout << "Tempo medio: "<<seqr.mean_time<< endl;
    cout << "Deviazione Standard: "<<seqr.stddev_time<< endl;
    cout << "Tempo massimo: "<<seqr.max_time<< endl;
    cout << "Tempo minimo: "<<seqr.min_time<< endl;
    cout << "############################"<<endl;
}



void choosePwd(vector<string> passwordList,string& password) {

    string pwd;
    password="";
    cout << endl<<"============================="<< endl;
    cout << "   Scegli una password"<< endl;

    bool viewOtherPWD = true;
    while (viewOtherPWD) {
        int i=0;
        for (const auto& pwd : passwordList) {
            cout << "============================="<< endl;
            cout << "1. "<<pwd<< endl;
            cout << "2. Mostra altra Password"<< endl;

            int choice;
            cin >> choice;

            if (choice == 1) {
                password=pwd;
                cout << "Hai scelto la password:"<<password<<endl;
                passwordList.erase(passwordList.begin() + i);
                viewOtherPWD = false;
                break;
            } else if (choice != 2) {
                cout << "\nScelta non valida. Riprova di nuovo.\n";
            }
            i++;
        }
    }
    return ;
}


void buildFilePasswords(const string& filein, const string& fileout) {

    if(filesystem::exists(fileout)) {
        cout << "Trovato un file delle password. Non verrà ricreato!!!" << endl;
        return;
    }

    ifstream inputf(filein);
    if (!inputf) {
        cerr << "Errore: impossibile aprire il file delle password: " << filein << endl;
        return;
    }

    ofstream outputf(fileout);
    if (!outputf) {
        cout << "Errore: impossibile aprire il nuovo file delle password: " << fileout << endl;
        return;
    }

    string pwd;
    vector<string> pwds;

    std::string regExprStr(R"(([a-zA-Z0-9]{1,8}))");
    std::regex rgx(regExprStr);
    std::smatch smatch;

    while (getline(inputf, pwd)) {
        // Verifica della lunghezza esatta di 8 caratteri
        if (pwd.length() == 8)
            if (std::regex_search(pwd, smatch, rgx))
                pwds.push_back(pwd);

    }
    inputf.close();

    cout << "Numero di password conformi trovate: " << pwds.size() << endl;

    for (const auto& password : pwds) {
        outputf << password << endl;
    }
    cout << "Le password conformi sono state salvate in: " << fileout << endl;
    outputf.close();
}


std::vector<std::string> loadPasswords(const std::string& filepwd) {
    std::vector<std::string> passwords;
    std::ifstream filep(filepwd);
    std::string password;

    while (getline(filep, password)) {
        passwords.push_back(password);
    }

    return passwords;
}

double minTime(std::vector<double> execTimes) {

    return *std::min_element(execTimes.begin(), execTimes.end());
}
double maxTime(std::vector<double> execTimes) {

    return *std::max_element(execTimes.begin(), execTimes.end());
}
double meanTime(std::vector<double> execTimes) {

    return std::accumulate(execTimes.begin(), execTimes.end(), 0.0)/ execTimes.size();
}

double stdDev(std::vector<double> execTimes) {
    double variance = 0.0;
    for (const auto& time : execTimes)
           variance += std::pow(time - meanTime(execTimes), 2);
    return std::sqrt(variance / execTimes.size());
}


           



