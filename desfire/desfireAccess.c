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

#include "desfireAccess.h"


inline int authApplication(MifareTag tag,int keyNumber)
{
  MifareDESFireKey key = mifare_desfire_3des_key_new(createDesKey(makePinpad(), "des-ede"));
  return mifare_desfire_authenticate(tag,keyNumber, key);
}


nfc_device* getRfidDevice()
{
  size_t device_count = 0;
  nfc_connstring devices[8];
  nfc_context *context;

  nfc_init(&context);

  device_count = nfc_list_devices(context,devices,8);

  if (device_count > 0)
    return nfc_open (context,devices[0]);
  else
  {
    fprintf(stderr,"Connecting to nfc device failed\n");
    return NULL;
  }
}


int writeBignum(MifareTag tag, MifareDESFireAID aid,const BIGNUM const *bignum,int start)
{
  if (mifare_desfire_create_value_file (tag, start++, MDCM_ENCIPHERED, 0xeeee, 0, bignum->top+1, bignum->top, 0) < 0)
    return -1;
  if (mifare_desfire_create_value_file (tag, start++, MDCM_ENCIPHERED, 0xeeee, 0, bignum->dmax+1, bignum->dmax, 0) < 0)
    return -1;
  if (mifare_desfire_create_value_file (tag, start++, MDCM_ENCIPHERED, 0xeeee, 0, bignum->neg+1, bignum->neg, 0) < 0)
    return -1;
  if (mifare_desfire_create_value_file (tag, start++, MDCM_ENCIPHERED, 0xeeee, 0, bignum->flags+1, bignum->flags, 0) < 0)
    return -1;
  if (mifare_desfire_create_std_data_file(tag, start, MDCM_ENCIPHERED, 0x0000,sizeof(BN_ULONG)*(bignum->dmax)) < 0)
    return -1;
  if (mifare_desfire_write_data(tag,start,0,sizeof(BN_ULONG)*(bignum->dmax),bignum->d) < 0)
    return -1;

  return 0;
}


int readBignum(MifareTag tag, MifareDESFireAID aid, BIGNUM *bignum,int start)
{
  if (!bignum)
  {
    return -1;
  }
  if (mifare_desfire_get_value (tag, start++, &(bignum->top)) < 0)
    return -1;
  if (mifare_desfire_get_value (tag, start++, &(bignum->dmax)) < 0)
    return -1;
  if (mifare_desfire_get_value (tag, start++, &(bignum->neg)) < 0)
    return -1;
  if (mifare_desfire_get_value (tag, start++, &(bignum->flags)) < 0)
    return -1;
  bignum->d = calloc(1,sizeof(BN_ULONG)*(bignum->dmax)*2);
  if (!bignum->d)
    return -1;
  if (mifare_desfire_read_data(tag,start,0,0,bignum->d) < 0)
    return -1;

  return 0;
}


RSA* readRSA()
{
  nfc_device *device = NULL;
  MifareTag *tag = NULL;
  MifareDESFireAID aid;
  RSA *rsa = NULL;
  uint8_t key_data_null[8] = { 0,0,0,0,0,0,0,0};
  MifareDESFireKey defaultKey = mifare_desfire_des_key_new_with_version (key_data_null);

  device = getRfidDevice();

  if (!device)
    return NULL;

  tag = freefare_get_tags(device);

  mifare_desfire_connect (tag[0]);

  aid = mifare_desfire_aid_new(AID_NUMBER);

  mifare_desfire_select_application (tag[0], aid);

  if (authApplication(tag[0], defaultKeyNumber) < 0)
  {
    fprintf(stderr,"Falscher Key\n");
    nfc_close(device);
    return NULL;
  }

  if (!rsa)
    rsa = RSA_new();

  if (!rsa->n)
    rsa->n = BN_new();
  if (!rsa->d)
    rsa->d = BN_new();
  if (!rsa->e)
    rsa->e = BN_new();
  if (readBignum(tag[0],aid,rsa->n,0) < 0)
  {
    fprintf(stderr,"readBignum %d failed\n",0);
    nfc_close(device);
    return NULL;
  }

  if (readBignum(tag[0],aid,rsa->d,5) < 0)
  {
    fprintf(stderr,"readBignum %d failed\n",0);
    nfc_close(device);
    return NULL;
  }

  if (readBignum(tag[0],aid,rsa->e,10) < 0)
  {
    fprintf(stderr,"readBignum %d failed\n",0);
    nfc_close(device);
    return NULL;
  }

  nfc_close(device);

  return rsa;
}


int resetKey()
{
  nfc_device *device = NULL;
  MifareTag *tag = NULL;
  MifareDESFireAID aid;
  uint8_t key_data_null[8] = { 0,0,0,0,0,0,0,0};
  MifareDESFireKey defaultKey = mifare_desfire_des_key_new_with_version (key_data_null);

  device = getRfidDevice();

  if (!device)
    return NULL;

  tag = freefare_get_tags(device);

  mifare_desfire_connect (tag[0]);

  aid = mifare_desfire_aid_new(AID_NUMBER);

  if (mifare_desfire_select_application (tag[0], aid) < 0)
  {
    nfc_close(device);
    return -1;
  }

  MifareDESFireKey key = mifare_desfire_3des_key_new(createDesKey(makePinpad(),"des-ede"));

  if (mifare_desfire_authenticate(tag[0], defaultKeyNumber, key) < 0)
  {
    fprintf(stderr,"Authentication with key failed\n");
    nfc_close(device);
    return -1;
  }

  if (mifare_desfire_change_key(tag[0], 0, defaultKey, key) < 0)
  {
    fprintf(stderr,"Key change to defaultKey failed\n");
    nfc_close(device);
    return -1;
  }

  if (mifare_desfire_authenticate(tag[0], defaultKeyNumber, defaultKey) < 0 )
  {
    fprintf(stderr,"Authentication with defaultKey failed\n");
    nfc_close(device);
    return -1;
  }

  if (mifare_desfire_select_application (tag[0], NULL) < 0)
  {
    nfc_close(device);
    return -1;
  }

  if (mifare_desfire_authenticate(tag[0], defaultKeyNumber, defaultKey) < 0)
  {
    fprintf(stderr,"Authentication with key failed\n");
    if (mifare_desfire_change_key(tag[0], 0, defaultKey, key) < 0)
    {
      fprintf(stderr,"Key change to defaultKey in Master application failed\n");
      nfc_close(device);
      return -1;
    }
  }

  printf("Schlüssel Erfolgreich geändert\n");

  nfc_close(device);

  return 0;
}


int writeRSA(RSA *rsa)
{
  nfc_device *device = NULL;
  MifareTag *tag = NULL;
  MifareDESFireAID aid;
  uint8_t key_data_null[8] = { 0,0,0,0,0,0,0,0};
  MifareDESFireKey defaultKey = mifare_desfire_des_key_new_with_version (key_data_null);

  device = getRfidDevice();

  if (!device)
    return -1;

  tag = freefare_get_tags(device);

  mifare_desfire_connect (tag[0]);

  aid = mifare_desfire_aid_new(AID_NUMBER);

  mifare_desfire_create_application(tag[0],aid,0xED,1);

  if (mifare_desfire_select_application (tag[0], aid) < 0)
  {
    nfc_close(device);
    return -1;
  }

  MifareDESFireKey key = mifare_desfire_3des_key_new(createDesKey(makePinpad(),"des-ede"));

  if (mifare_desfire_authenticate(tag[0],0 , defaultKey) == 0 )
  {
    if (mifare_desfire_change_key(tag[0], 0, key, defaultKey) < 0)
    {
      fprintf(stderr,"Change defaultKey to key failed\n");
      nfc_close(device);
      return -1;
    }
  }

  if (mifare_desfire_authenticate(tag[0],0 , key) < 0 )
  {
    fprintf(stderr,"Falscher Key\n");
    nfc_close(device);
    return -1;
  }

  if (writeBignum(tag[0],aid,rsa->n,0) < 0)
  {
    fprintf(stderr,"writeBignum %d failed\n",0);
    nfc_close(device);
    return -1;
  }

  if (writeBignum(tag[0],aid,rsa->d,5) < 0)
  {
    fprintf(stderr,"writeBignum %d failed\n",5);
    nfc_close(device);
    return -1;
  }

  if (writeBignum(tag[0],aid,rsa->e,10) < 0)
  {
    fprintf(stderr,"writeBignum %d failed\n",10);
    nfc_close(device);
    return -1;
  }

  printf("Key Geschrieben\n");
  fflush(stdout);

  nfc_close(device);

  return 0;
}


char* getUid()
{
  nfc_device *device = NULL;
  MifareTag *tag = NULL;
  char* temp;

  device = getRfidDevice();

  if (!device)
    return NULL;

  tag = freefare_get_tags(device);

  if (!tag[0])
  {
    nfc_close(device);
    return NULL;
  }

  temp = freefare_get_tag_uid(tag[0]);

  nfc_close(device);

  return temp;
}


int deleteApplication()
{
  nfc_device *device = NULL;
  MifareTag *tag = NULL;
  MifareDESFireAID aid = mifare_desfire_aid_new(AID_NUMBER);
  uint8_t *files = NULL;
  size_t file_count = 0;
  uint8_t key_data_null[8] = { 0,0,0,0,0,0,0,0};
  MifareDESFireKey defaultKey = mifare_desfire_des_key_new_with_version (key_data_null);

  device = getRfidDevice();

  if (!device)
    return NULL;

  tag = freefare_get_tags(device);

  mifare_desfire_connect (tag[0]);

  if (mifare_desfire_select_application (tag[0], aid) < 0)
  {
    fprintf(stderr,"select AID failed\n");
    nfc_close(device);
    return -1;
  }

  if (mifare_desfire_authenticate(tag[0], defaultKeyNumber, defaultKey) < 0 )
  {
    fprintf(stderr,"auth default key on AID failed\n");
    nfc_close(device);
    return -1;
  }

  mifare_desfire_get_file_ids(tag[0], &files, &file_count);

  size_t i;
  for (i = 0; i < file_count; i++)
  {
    mifare_desfire_delete_file(tag[0],files[i]);
  }


  if (mifare_desfire_select_application (tag[0], NULL) < 0)
  {
    fprintf(stderr,"select Master Application failed\n");
    nfc_close(device);
    return -1;
  }

  if (mifare_desfire_authenticate(tag[0], defaultKeyNumber, defaultKey) < 0 )
  {
    fprintf(stderr,"auth Master Application failed\n");
    nfc_close(device);
    return -1;
  }

  if (mifare_desfire_delete_application(tag[0],aid) < 0)
  {
    fprintf(stderr,"delete application failed\n");
    nfc_close(device);
    return -1;
  }

  printf("Application deleted\n");

  nfc_close(device);

  return 0;
}

