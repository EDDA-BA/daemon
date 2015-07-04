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

#include "daemon.h"


void DieWithError(const char *errorMessage) 
{ 
  perror(errorMessage);
  exit(1);
}


char* parseStr(const char* text,const char* search,const char *brk)
{
  char *start = NULL;
  char *end = NULL; 
  char *ret = NULL;

  start = strstr(text,search);

  if (start)
  {
    start += strlen(search);
    end = strpbrk(start,brk); 
  }
   
  if (!end)
  {
    printf("Kein Start oder End");
    fflush(stdout);
    return NULL;
  }
   
  int tmp = end - start;

  if (tmp<1)
    return NULL;

  ret = calloc(tmp+1,sizeof(char));

  if (!ret)
    return NULL;

  memcpy(ret,start,tmp);
  
  return ret;
}


void HandleTCPClient(int clntSocket)
{
  char buffer[RCVBUFSIZE];
  char *number;
  int recvMsgSize;
  char *cmd;
  char* data;
  RSA *rsa;

  recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0);
  
  if (recvMsgSize < 0)
    DieWithError("recv() failed");

  cmd = parseStr(buffer,"cmd=","\n!&");

  data = parseCmd(cmd,buffer);
  
  if (cmd)
    free(cmd);

  recvMsgSize = sprintf(buffer,
                        "HTTP/1.0 200 OK\n"\
                        "Date: Fri, 13 Jan 2006 15:12:48 GMT\n"\
                        "Last-Modified: Tue, 10 Jan 2006 11:18:20 GMT\n"\
                        "Access-Control-Allow-Origin: *\n"\
                        "Content-Language: de\n"\
                        "Content-Type: text/html; charset=utf-8\n\n"\
                        "%s\n\r",data);

  if (data)
    free(data);

  while (recvMsgSize > 0)
  {
    if (send(clntSocket, buffer, recvMsgSize, 0) != recvMsgSize)
      DieWithError("send() failed");
    recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, MSG_DONTWAIT);
  }

  close(clntSocket);
}


char* returnUid()
{
  getUid();
}


char* parseCmd(const char *cmd, const char *buffer)
{
  if (cmd == NULL)
    return NULL;
  else if (!strcmp(cmd,VERIFY))
    return verify(buffer);
  else if (!strcmp(cmd,PINPAD))
    return pinPad();
  else if (!strcmp(cmd,UID))
    return returnUid();
  
  return NULL;
}


char* pinPad()
{
  return makePinpad();
}


char* verify(const char *buffer)
{
  char *ret = NULL;
  char *number = NULL;
  int numberLen = 0;
  char *numberBase64 = NULL;
  size_t nB64Len = 0;
  char * decryptBuf = NULL;
  int decryptLen = 0;
  char sha[SHA_DIGEST_LENGTH] = {0};
  RSA *rsa = NULL;

  number = parseStr(buffer,VERIFY_RANDOM ,"\n&!\r");

  if (!number)
  {
    return NULL;
  }

  numberLen = strlen(number) - ((number[strlen(number)] == '=')?1:0);

  base64_decode_alloc(number, numberLen,&numberBase64,&nB64Len);

  if (!numberBase64)
    return NULL;

  rsa = readRSA();
 
  if (!rsa)
    return NULL;

  decryptBuf = calloc(1,RSA_size(rsa));

  if (!decryptBuf)
    return NULL;

  decryptLen = RSA_private_decrypt(RSA_size(rsa),numberBase64,decryptBuf,rsa,RSA_PKCS1_PADDING);

  if (decryptLen <= 0)
    return NULL;

  SHA1(decryptBuf, decryptLen, sha);

  base64_encode_alloc(sha,SHA_DIGEST_LENGTH,&ret);

  return ret;
}

