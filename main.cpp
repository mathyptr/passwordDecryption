#ifdef _OPENMP
#include <omp.h> // for OpenMP library functions
#endif
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "util.h"
#include "utilCrypt.h"
#include "decryptSequential.h"
#include "decryptParallel.h"

int main() {
    int nthreads, tid;
    tid = -1;


    string crypted_password = cryptDES("password","prova");
    std::cout << "pwd crypted " <<crypted_password<< std::endl;

    std::string file_in = "/mnt/datadisk1/c++/clion/passwordDecryption/resource/rockyou.txt";
    std::string file_out = "/mnt/datadisk1/c++/clion/passwordDecryption/resource/password_file.txt";  ///< File delle password
    std::string file_tmp = "/mnt/datadisk1/c++/clion/passwordDecryption/resource/password_file.tmp";  ///< File delle password per i test


    SplashScreen();

    std::cout << "Lettura file password e selezione delle password conformi..." << std::endl;
    buildFilePasswords(file_in, file_out);
    std::cout << "Password conformi salvate in: " << file_out << std::endl;

    std::string password;

    std::vector<std::string> passwordList = loadPasswords(file_out);
    choosePwd(passwordList, password);
    cout << "I test saranno effettuati su questa password:"<<password<<endl;

    testResult seqr;
    seqr=TestSeq(password, "abc",passwordList,10);
    std::string title="Risultati test sequenziale";
    SplashResult(title,seqr);

    std::vector<testResult> parr;
    std::vector<int> thread_counts = { 2, 3, 4, 5,6,7,8,9,10 };
    parr=testPar(password, "abc",passwordList,thread_counts,10);

#ifdef _OPENMP
    std::cout << "_OPENMP defined" << std::endl;
    // OpenMP major + minor version
    std::cout << "OpenMP version: " << _OPENMP << std::endl;
    std::cout << "Num processors (Phys+HT): " << omp_get_num_procs() << std::endl;
#endif

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
