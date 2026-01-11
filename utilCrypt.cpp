
#include <string>
using namespace std;

#include <stdio.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

//************************************************************************************************************
// from www.gnu.org
// Encrypting Passwords
// Function: char * crypt_r (const char *key, const char *salt, {struct crypt_data *} data)
//
// The crypt function takes a password, key, as a string, and a salt character array which is described below,
// and returns a printable ASCII string which starts with another salt.
// It is believed that, given the output of the function, the best way to find a key that will produce
// that output is to guess values of key until the original value of key is found.
//
// The salt parameter does two things. Firstly, it selects which algorithm is used, the MD5-based one or the DES-based one.
// Secondly, it makes life harder for someone trying to guess passwords against a file containing many passwords;
// without a salt, an intruder can make a guess, run crypt on it once, and compare the result with all the passwords.
// With a salt, the intruder must run crypt once for each different salt.
//
// For the MD5-based algorithm, the salt should consist of the string $1$, followed by up to 8 characters, terminated by either another $ or the end of the string.
// The result of crypt will be the salt, followed by a $ if the salt didn't end with one, followed by 22 characters from the alphabet ./0-9A-Za-z, up to 34 characters total.
// Every character in the key is significant.
//
// For the DES-based algorithm, the salt should consist of two characters from the alphabet ./0-9A-Za-z, and the result of crypt will be those two characters followed by 11 more from the same alphabet, 13 in total.
// Only the first 8 characters in the key are significant.
//
// Than the crypt function the Crypt_r takes an extra parameter which includes space for its result, so it can be reentrant. data->initialized must be cleared to zero before the first time crypt_r is called.
//************************************************************************************************************
string cryptDES(const std::string& password, const std::string& salt){
    char * p;
    int i;
    struct crypt_data data[1] = {0};


// Crypt the string password with the string salt
    p = crypt_r(password.c_str(), salt.c_str(),data);

    string pwd(p);

    return pwd;
}


//***********************************************************************************************
// decryptDES shows how to verify a password.
// It prompts the user for a password and prints "Test OK." if the user types GNU libc manual.
//***********************************************************************************************
int decryptDES(void){
    /* Hashed form of "GNU libc manual". */
    const char *const pass = "$1$/iSaq7rB$EoUw5jJPPvAPECNaaWzMK/";

    char *result;
    int ok;
    struct crypt_data data[1] = {0};
    /* Read in the user's password and encrypt it,
       passing the expected password in as the salt. */
    result = crypt_r(getpass("Password:"), pass,data);

    /* Test the result. */
    ok = strcmp (result, pass) == 0;

    puts(ok ? "Test OK." : "Test NO OK.");
    return ok ? 0 : 1;
}
