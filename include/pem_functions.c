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

#include "pem_functions.h"

RSA *pubKeyToRsa(char * pem_key_pub)
{
  BIO *bio_pub;
  RSA *rsa_pub;

  bio_pub = BIO_new(BIO_s_mem());
  BIO_write(bio_pub,pem_key_pub,strlen(pem_key_pub));
  rsa_pub = PEM_read_bio_RSAPublicKey(bio_pub,NULL,NULL,NULL);

  BIO_free_all(bio_pub);

  return(rsa_pub);
}


RSA *privateKeyToRsa(char * pem_key_private)
{
  BIO *bio_private;
  RSA *rsa_private;

  bio_private = BIO_new(BIO_s_mem());
  BIO_write(bio_private,pem_key_private,strlen(pem_key_private));
  rsa_private = PEM_read_bio_RSAPrivateKey(bio_private, NULL, NULL, NULL);

  BIO_free_all(bio_private);

  return(rsa_private);
}


int encryptWithPubKey(char *pem_key_pub, char *toBeCrypt, char *wasCrypt)
{
  int result;

  result = RSA_public_encrypt( strlen(toBeCrypt)+1, 
                              toBeCrypt, 
                              wasCrypt, 
                              pubKeyToRsa(pem_key_pub), 
                              RSA_PKCS1_PADDING);
  return(result);
}

char *decryptWithPrivateKey(char *pem_key_private, char *toBeDecrypt, int lengthOfToBeDecrypt)
{
  char *buf = malloc(RSA_size( privateKeyToRsa(pem_key_private)));

  if (lengthOfToBeDecrypt != RSA_size(privateKeyToRsa(pem_key_private)))
  {
      fprintf(stderr,"Error: ciphertext should match length of key\n");
      return(NULL);
  }

  RSA_private_decrypt( lengthOfToBeDecrypt, 
                      toBeDecrypt, 
                      buf, 
                      privateKeyToRsa(pem_key_private),
                      RSA_PKCS1_PADDING);

  return(buf);
}
