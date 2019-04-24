#include "getstrdata.h"
#include <stdio.h>

GetStrData::GetStrData()
{
}



string GetStrData(BYTE *p, WORD32 dwLen)
{
    CHAR buf[4];
    string result("");
    WORD32 dwLp = 0;
    for(dwLp = 0; dwLp < dwLen; dwLp++)
    {
        snprintf(buf, sizeof(buf), "%02x ", p[dwLp]);
        result += buf;
    }

    return result;
}


void PrintStrData(CHAR *tips, BYTE *p, WORD32 dwLen)
{
    CHAR buf[4];
    string result("");
    WORD32 dwLp = 0;
    for(dwLp = 0; dwLp < dwLen; dwLp++)
    {
        snprintf(buf, sizeof(buf), "%02x ", p[dwLp]);
        result += buf;
    }
    fprintf(stdout, "len:%u, %s:%s\n", dwLen, tips, result.c_str());
}
