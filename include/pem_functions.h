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

#include <openssl/rsa.h>
#include <openssl/pem.h>

RSA *pubKeyToRsa(char *pem_key_pub);
RSA *privateKeyToRsa(char *pem_key_private);
int encryptWithPubKey(char *pem_key_pub, char *toBeCrypt, char *wasCrypt);
char *decryptWithPrivateKey(char *pem_key_private, char *toBeDecrypt, int lengthOfToBeDecrypt);


