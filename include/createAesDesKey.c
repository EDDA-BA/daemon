/*
This file is part of edda.

edda is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

edda is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with edda.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

unsigned char* createDesKey(char *password , char *keyType);

unsigned char* createDesKey(char *password , char *keyType)
{
    const EVP_CIPHER *cipher;
    const EVP_MD *dgst = NULL;
    unsigned char *key = calloc(1,EVP_MAX_KEY_LENGTH), iv[EVP_MAX_IV_LENGTH];
    const unsigned char *salt = NULL;
    int i;

    OpenSSL_add_all_algorithms();

    cipher = EVP_get_cipherbyname(keyType); // "for 3 key DES use des-ede3"
    if(!cipher) { fprintf(stderr, "no such cipher\n"); return "1"; }
  
    OpenSSL_add_all_digests();

    dgst=EVP_get_digestbyname("sha1");
    if(!dgst) { fprintf(stderr, "no such digest\n"); return "1"; }

    if(!EVP_BytesToKey(cipher, dgst, salt,
        (unsigned char *) password,
        strlen(password), 1, key, iv))
    {
        fprintf(stderr, "EVP_BytesToKey failed\n");
        return "1";
    }

    return key;
}
