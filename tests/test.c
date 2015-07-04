#include "test.h"

int test_parseStr()
{
  char *testBuffer= "hierSteht\n!vielo.0GESSSrieben";
  char *ret = parseStr(testBuffer,"!",".");
  return strcmp(ret,"vielo");
}


int test_desfire()
{
  RSA *rsa;
  rsa = readRSA();
  return 0;
}


int test_all()
{
  if (test_parseStr())
    return -1;
  if (test_desfire())
    return -1;
  return 0;
}


int main()
{
  if(test_all() != 0)
    fprintf(stderr,"Fehler aufgetreten\n");
  else
    printf("alles OK\n");
}


