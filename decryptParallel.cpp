#include "decryptParallel.h"
#include "utilCrypt.h"
#include <omp.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <atomic>
#include <immintrin.h>

using namespace std;


std::string ompDecryption(const std::string& encrypted_password, const std::string& salt,
    const std::vector<std::string>& pwdList, int threadNum, int chunkSize) {
    std::string word = "";
    bool found = false;

    // Parallelizza il bruteforce della password 
#pragma omp parallel num_threads(threadNum) shared(found, word)
    {
        std::string local_result;
#pragma omp for schedule(dynamic, chunkSize)
        for (int i = 0; i < static_cast<int>(pwdList.size()); ++i) {
            if (!found) {  
                std::string hash = cryptDES(pwdList[i], salt);

                if (hash == encrypted_password) {
#pragma omp critical  // Sezione critica per aggiornare il risultato
                    {
                        if (!found) {
                            found = true;
                            word = pwdList[i];
                        }
                    }
                }
            }
        }
    }

    return word;
}


std::vector<testResult> testPar(const std::string& password, const std::string& salt,
    std::vector<std::string> passwordList, const std::vector<int>& thread_counts, int iter) {
    std::vector<testResult> all_results;

    std::vector<int> chunkSizes = { 500 };
    string pwdtmp;
    // Ottimizza la priorità del processo
//MATHY    HANDLE process = GetCurrentProcess();
//MATHY    SetPriorityClass(process, HIGH_PRIORITY_CLASS);

    // Warm-up per stabilizzare le prestazioni
#pragma omp parallel
    {
        volatile std::string hash = cryptDES("warmup", salt);
    }

    for (int threadNum : thread_counts) {
        testResult thread_result;


        for (int chunkSize : chunkSizes) {
            cout << "############################"<<endl; 
            cout << "###Test Parallelo con " << threadNum << " thread "<<"e chunk size: " << chunkSize << " ###"<<endl;

            // Configura l'affinità dei thread
//MATHY            DWORD_PTR mask = (1ULL << threadNum) - 1;
//MATHY            SetProcessAffinityMask(process, mask);

            std::vector<double> execTimes;
            const size_t list_size = passwordList.size();
            int block = passwordList.size() / iter;


            // Configura OpenMP
            omp_set_dynamic(0);
            omp_set_num_threads(threadNum);

            // Esegue i test multipli
            for (int i = 0; i < iter; ++i) {
                cout << "############################"<<endl;
                cout << "Iter num: " << i + 1<< endl;

                pwdtmp=passwordList[block*i];
                passwordList[block*i]=password;

                std::string encrypted_password = cryptDES(password, salt);

                // Pulisce la cache per test più accurati
                _mm_mfence();
                std::vector<char> cache_clear(1024 * 1024, 1);
                for (volatile char& c : cache_clear) {
                    c = 2;
                }

                // Misura il tempo di esecuzione
                auto start = std::chrono::high_resolution_clock::now();
                std::string result = ompDecryption(encrypted_password, salt, passwordList,
                    threadNum, chunkSize);
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> elapsed = end - start;
                execTimes.push_back(elapsed.count());

                passwordList[block*i]=pwdtmp;

                // Output dei risultati
                cout << " (chunk size " << chunkSize << "): "<< endl;
                cout << "Tempo: " << elapsed.count() << " secondi. "<< endl;
                cout << "Posizione: " << block*i << endl;
            }

            // Calcola le statistiche

            testResult seqp;
            seqp.threadNum=threadNum;
            seqp.chunkSize=chunkSize;
            seqp.max_time=maxTime(execTimes);
            seqp.min_time=minTime(execTimes);
            seqp.mean_time=meanTime(execTimes);
            seqp.stddev_time=stdDev(execTimes);
            seqp.num_password=passwordList.size();
            seqp.num_iter= iter;
            seqp.test_type=PARALLEL;

            // Output delle statistiche
            cout << "Statistiche Test Parallelo (" << threadNum << " thread, "
                << "chunk size " << chunkSize << "):" << endl;
            cout << "- Tempo medio: " << seqp.mean_time << "s" << endl;
            cout << "- Deviazione standard: " << seqp.stddev_time << "s" << endl;
            cout << "- Tempo minimo: " << seqp.min_time << "s" << endl;
            cout << "- Tempo massimo: " << seqp.max_time << "s" << endl;
            all_results.push_back(seqp);
        }

    }

    return all_results;
}
