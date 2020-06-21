#ifndef GETSTRDATA_H
#define GETSTRDATA_H

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class GetStrData
{
public:
    GetStrData();
};

#define GETSTRDATA(P, L) GetStrData(P, L).c_str()
#define GETSTRDATAINLEN(P) GetStrData(P, sizeof(P)).c_str()
#define GETSTRDATAINLENPTR(VAR) GetStrData(Var, sizeof(VAR)).c_str()

typedef unsigned char BYTE;
typedef char CHAR;
typedef unsigned short WORD16;
typedef unsigned int WORD32;


string GetStrData(BYTE *p, WORD32 dwLen);
void PrintStrData(CHAR *tips, BYTE *p, WORD32 dwLen);

#endif // GETSTRDATA_H
