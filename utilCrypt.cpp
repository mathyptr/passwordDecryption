
#include <string>
using namespace std;


#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>


//string cryptDES(const std::string& password,const std::string& salt){
string cryptDES(const std::string& password, const std::string& salt){
    unsigned long seed[2];

    const char *const seedchars =
            "./0123456789ABCDEFGHIJKLMNOPQRST"
            "UVWXYZabcdefghijklmnopqrstuvwxyz";
    char * p;
    int i;


    /* Generate a (not very) random seed.
       You should do it better than this... */
    seed[0] = time(NULL);
    seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);



    char salt1[12]="$1$12345678";
    /* Read in the user's password and encrypt it. */
    p = crypt(password.c_str(), salt1);

    string pwd(p);
//    free(p);
    return pwd;
}


int decryptDES(void){
    /* Hashed form of "GNU libc manual". */
    const char *const pass = "$1$/iSaq7rB$EoUw5jJPPvAPECNaaWzMK/";

    char *result;
    int ok;

    /* Read in the user's password and encrypt it,
       passing the expected password in as the salt. */
    result = crypt(getpass("Password:"), pass);

    /* Test the result. */
    ok = strcmp (result, pass) == 0;

    puts(ok ? "Access granted." : "Access denied.");
    return ok ? 0 : 1;
}
