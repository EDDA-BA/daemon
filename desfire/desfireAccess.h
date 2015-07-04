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

#ifndef __DESFIRE_ACCESS_H__
#define __DESFIRE_ACCESS_H__
#include <stdio.h>
#include <stdint.h>  
#include <nfc/nfc.h>
#include <freefare.h>
#include <openssl/rsa.h>
#include "../daemon.h"

RSA* readRSA();
int resetKey();
int writeRSA(RSA *rsa);
char* getUid();

#define defaultKeyNumber 0
#define AID_NUMBER 0x00010203

#endif
