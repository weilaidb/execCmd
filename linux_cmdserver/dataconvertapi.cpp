#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <algorithm>

#ifdef __cplusplus
extern "C" {
#endif
#include "DataConvert.h"
#ifdef __cplusplus
}
#endif

using namespace std;

typedef unsigned char BYTE;

BYTE gArray[1234] = {0};

#define ARRAYSIZE(A) (unsigned int)(sizeof(A)/sizeof(A[0]))
/**
  * char数组 转换byte数组
  **/
int chararrayconvertbytearray(char *str, BYTE *parry)
{
    int i = 0;
    int len = strlen(str);
    if(len % 2 != 0)
    {
        perror("len is % 2 != 0");
        assert(0);
    }
    for(i = 0; i < len/2; i+=2)
    {
        if(str[i] && str[i+1])
        {
            printf("No:%-2u -- Val:%02X\n", i + 1, str[i]);
        }
    }
    //转换成大写
    string strA = str;
    transform(strA.begin(), strA.end(), strA.begin(), ::toupper);

    checkcharhexvalid((char *)strA.c_str());
    int ks = hexChartoByte((char *)strA.c_str(),(char *)parry);
    printf("ks:%u\n", ks);
    for(i = 0; i < ks; i++)
    {
        if(parry[i])
            printf("No:%-2u -- Val:%02X\n", i + 1, parry[i]);
    }
    return ks;
}

//int main(int argc, char *argv[])
//{
//    char str[] = "123456789abcdef123454578";
//    chararrayconvertbytearray(str,gArray);

//    cout << "hello world" << endl;
//    return 0;
//}


