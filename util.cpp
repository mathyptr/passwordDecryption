#include "util.h"
#include <fstream>
#include <filesystem> 
#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

void checkPasswords(const string& filein, const string& fileout) {
    ifstream inputf(filein);
    if (!inputf) {
        cerr << "Errore: impossibile aprire il file delle password: " << filein << endl;
        return;
    }

    ofstream outputf(fileout);
    if (!outputf) {
        cerr << "Errore: impossibile aprire il nuovo file delle password: " << fileout << endl;
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
