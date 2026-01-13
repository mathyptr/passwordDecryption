#ifdef _OPENMP
#include <omp.h> // for OpenMP library functions
#endif
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <filesystem>

#include "util.h"
#include "utilCrypt.h"
#include "decryptSequential.h"
#include "decryptParallel.h"

int main() {

    string crypted_password = cryptDES("password","prova");
    std::cout << "pwd crypted " <<crypted_password<< std::endl;

    std::cout << "Directory corrente: " << filesystem::current_path() << '\n';

    std::string file_in = "./resource/rockyou.txt";
    std::string file_out = "./resource/password_file.txt";
    std::string file_inSeq = "./resource/resultSEQ.csv";
    std::string file_inPar = "./resource/resultPAR.csv";


    SplashScreen();

//*****************************************+*********************************************
//    Lettura file password e creazione del file con password conformi alle specifiche
//*****************************************+*********************************************
    std::cout << "Lettura file password e selezione delle password conformi..." << std::endl;
    buildFilePasswords(file_in, file_out);
    std::cout << "Password conformi salvate in: " << file_out << std::endl;

    std::string password;

//*****************************************+*********************************************
//    Lettura file password conformi alle specifiche
//*****************************************+*********************************************
    std::vector<std::string> passwordList = loadPasswords(file_out);

//*****************************************+*********************************************
//    Selezione password di test
//*****************************************+*********************************************
    choosePwd(passwordList, password);
    cout << "I test saranno effettuati su questa password: "<<password<<endl;

//*****************************************+*********************************************
//    Inserimento del numero di iterazioni
//*****************************************+*********************************************
    int iter;
    cout << "Inserisci il numero di iterazioni: "<<endl;
    cin >> iter;
//*****************************************+*********************************************
//    INIZIO TEST
//*****************************************+*********************************************
    std::vector<testResult> seqr;
    std::string title;
    std::string salt="ab";

//    Test Sequenziale
    seqr.push_back(TestSeq(password, salt,passwordList,iter));
    title="Risultati test sequenziale";
    SplashResult(title,seqr); //Visualizzazione Risultati Test Sequenziale
    saveResultToFile(file_inSeq,seqr); //Salvataggio su file risultati Test Sequenziale

//    Test Parallelo
#ifdef _OPENMP
    int maxThreads = omp_get_max_threads();

    std::cout << "_OPENMP defined" << std::endl;
    // OpenMP major + minor version
    std::cout << "OpenMP version: " << _OPENMP << std::endl;
    std::cout << "Numero massimo di thread disponibili: " << maxThreads << std::endl;
#endif

    std::vector<testResult> parr;
    std::vector<int> thread_counts;
    // Test con diversi numeri di thread
    for (int numThreads = 2; numThreads <= maxThreads; numThreads ++)
        thread_counts.push_back(numThreads);
    parr=testPar(password, salt,passwordList,thread_counts,iter);
    title="Risultati test parallelo";
    SplashResult(title,parr); //Visualizzazione Risultati Test Parallelo
    saveResultToFile(file_inPar,parr);  //Salvataggio su file risultati Test Parallelo

}
