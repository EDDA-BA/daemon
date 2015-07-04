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

#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include "include/pem_functions.h"
#include "include/base64.h"
#include "desfire/desfireAccess.h"
#include "pinpad/pinpad.h"

#define RCVBUFSIZE 8192
#define MAXPENDING 1
#define VERIFY "chg"
#define VERIFY_RANDOM "rnd="
#define PINPAD "pin"
#define UID "uid"

void DieWithError(const char *errorMessage);
void HandleTCPClient(int clntSocket);
char* parseStr(const char* ,const char* ,const char*);
char* parseCmd(const char* ,const char*);
char* verify(const char*);
char* pinPad();
unsigned char* createDesKey(char *password , char *keyType);

#endif
