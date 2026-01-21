#ifdef _OPENMP
#include <omp.h> // for OpenMP library functions
#endif

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

/*********************************************************
**** Funzione per la descrizione a video del programma ***
*********************************************************/
void SplashScreen() {
    cout << "###############################################################"<<endl;
    cout << "Questo programma implementa un attacco brute-force per"<< endl;
    cout << "decifrare password di otto caratteri crittografate con DES"<< endl;
    cout << "confrontando i costi computazionali di una versione sequentail"<< endl;
    cout << "e di una versione parallela di un attacco brute-force alle password,"<< endl;
    cout << "valutando la velocità ottenuta in diverse situazioni di test"<< endl;
    cout << "tramite parallelizzazione."<< endl;
    cout << "################################################################"<<endl;
}

/**********************************************************
***** Funzione per la visualizzazione dei risultati:  *****
      -Numero Totale di password
      -Numero di Iterazioni
      -Tempo medio
      -Deviazione Standard
      -Tempo massimo
      -Tempo minimo
**********************************************************/
void SplashResult(string& title,std::vector<testResult>& result) {
    cout << "#####################################"<<endl;
    cout << title<< endl;

    for (const auto& res : result) {

    if( res.test_type==SEQUENTIAL)
        cout << "TEST SEQUENZIALE"<< endl;
    else if( res.test_type==PARALLEL)
        cout << "TEST PARALLELO"<< endl;
    else
        cout<<"TEST TYPE NON DEFINITO"<< endl;

    cout << "Numero Totale di password: "<<res.num_password<< endl;
    cout << "Numero di Iterazioni: "<<res.num_iter<< endl;
    cout << "Tempo medio: "<<res.mean_time<< endl;
    cout << "Deviazione Standard: "<<res.stddev_time<< endl;
    cout << "Tempo massimo: "<<res.max_time<< endl;
    cout << "Tempo minimo: "<<res.min_time<< endl;
    cout << "#####################################"<<endl;
    }
}

/***************************************************************************************************
******* Funzione per salvare i risultati su file csv.                                        *******
******* Intestazione di colonna:                                                             *******
******* testType,numThreads,chunkSize,meanTime,stdDev,maxTime,minTime,numIter,totalPasswords *******
************************************************************************************é**************/
void saveResultToFile(const std::string& filename,std::vector<testResult>& result) {
    std::ofstream fileCSV(filename);
    fileCSV << "testType,numThreads,chunkSize,meanTime,stdDev,maxTime,minTime,numIter,totalPasswords"<< endl;

    for (const auto& res : result) {
        if( res.test_type==SEQUENTIAL)
            fileCSV << "SEQUENZIALE,";
        else if( res.test_type==PARALLEL)
            fileCSV << "PARALLELO,";
        else
            fileCSV<<"NON_DEFINITO,";
        fileCSV << res.threadNum<<",";
        fileCSV << res.chunkSize<<",";
        fileCSV << res.mean_time<< ",";
        fileCSV << res.stddev_time<< ",";
        fileCSV << res.max_time<< ",";
        fileCSV << res.min_time<< ",";
        fileCSV << res.num_iter<< ",";
        fileCSV << res.num_password<<endl;
    }
}

/*************************************************************************
**** Funzione per la selezione della password su cui effettuare i test ***
*************************************************************************/
void choosePwd(vector<string>& passwordList,string& password) {

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
                cout << "Hai scelto la password: "<<password<<endl;
                passwordList.erase(passwordList.begin() + i);
                viewOtherPWD = false;
                break;
            } else if (choice != 2) {
                cout << "Scelta non valida. Riprova di nuovo."<<endl;
            }
            i++;
        }
    }
}

/****************************************************************************************
****** Funzione per la creazione dei file delle  password su cui effettuare i test  *****
****************************************************************************************/
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

/****************************************************************************************
**** Funzione per la lettura delle password da un dizionario di password su file     ****
****************************************************************************************/
std::vector<std::string> loadPasswords(const std::string& filepwd) {
    std::vector<std::string> passwords;
    std::ifstream filep(filepwd);
    std::string password;

    while (getline(filep, password)) {
        passwords.push_back(password);
    }

    return passwords;
}

/******************************************************
***** Funzione per il calcolo del tempo minimo    *****
*******************************************************/
double minTime(std::vector<double> execTimes) {

    return *std::min_element(execTimes.begin(), execTimes.end());
}
/******************************************************
***** Funzione per il calcolo del tempo massimo   *****
*******************************************************/
double maxTime(std::vector<double> execTimes) {

    return *std::max_element(execTimes.begin(), execTimes.end());
}

/*******************************************************
***** Funzione per il calcolo del tempo medio      *****
*******************************************************/
double meanTime(std::vector<double> execTimes) {

    return std::accumulate(execTimes.begin(), execTimes.end(), 0.0)/ execTimes.size();
}

/*********************************************************
**** Funzione per il calcolo della deviazione standard ***
*********************************************************/
double stdDev(std::vector<double> execTimes) {
    double variance = 0.0;
    for (const auto& time : execTimes)
           variance += std::pow(time - meanTime(execTimes), 2);
    return std::sqrt(variance / execTimes.size());
}


/*********************************************************
****************** Funzione di test OMP ******************
*********************************************************/
void testOMP(){
    int nthreads, tid;
    tid = -1;
    printf("Global copy of tid before threads execution = %d\n", tid);

/* Fork a team of threads giving them their own copies of variables */

#pragma omp parallel private(nthreads, tid) default(none)
    {
/* Obtain thread number */
        tid = omp_get_thread_num();
        printf("Hello World from thread = %d\n", tid);

/* Only master thread does this */
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf("Master thread - Number of threads = %d\n", nthreads);
        }

        tid += 100;
        printf("Local copy of tid = %d\n", tid);

    }  /* All threads join master thread and are destroyed */

    printf("Global copy of tid after threads execution = %d\n", tid);

// Bernstein's conditions and OpenMP
    {
        const long n = 100;
        int A[n];
        int histo[n];
#pragma omp parallel for default(none) shared(A, n)
        for (int i = 0; i < n; i++)
            A[i] = i;
// XXX the following code will produce bad results !
#pragma omp parallel for default(none) shared(A, n)
        for (int i = 1; i < n; i++)
            A[i] = A[i - 1];
#pragma omp parallel default(none) shared(histo, A, n)
        {
#pragma omp for
            for (int i = 0; i < n; i++)
                histo[i] = 0;
#pragma omp for
            for (int i = 0; i < n; i++)
                 histo[A[i]]++;
        }

        for (int i=0; i<n; i++) {
            if (histo[i]>1)
                printf("i: %d - count: %d\n", i, histo[i]);
        }
    }
}


