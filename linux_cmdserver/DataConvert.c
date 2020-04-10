#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "DataConvert.h"

int strToHex(char *ch, char *hex)
{
    int high,low;
    int tmp = 0;
    if(ch == NULL || hex == NULL){
        return -1;
    }

    if(strlen(ch) == 0){
        return -2;
    }

    while(*ch){
        tmp = (int)*ch;
        high = tmp >> 4;
        low = tmp & 15;
        *hex++ = valueToHexCh(high); //先写高字节
        *hex++ = valueToHexCh(low); //其次写低字节
        ch++;
    }
    *hex = '\0';
    return 0;
}

int hexToStr(char *hex, char *ch)
{
    int high,low;
    int tmp = 0;
    if(hex == NULL || ch == NULL){
        return -1;
    }

    if(strlen(hex) %2 == 1){
        return -2;
    }

    while(*hex){
        high = hexCharToValue(*hex);
        if(high < 0){
            *ch = '\0';
            return -3;
        }
        hex++; //指针移动到下一个字符上
        low = hexCharToValue(*hex);
        if(low < 0){
            *ch = '\0';
            return -3;
        }
        tmp = (high << 4) + low;
        *ch++ = (char)tmp;
        hex++;
    }
    *ch = '\0';
    return 0;
}

int hexCharToValue(const char ch){
    int result = 0;
    //获取16进制的高字节位数据
    if(ch >= '0' && ch <= '9'){
        result = (int)(ch - '0');
    }
    else if(ch >= 'a' && ch <= 'z'){
        result = (int)(ch - 'a') + 10;
    }
    else if(ch >= 'A' && ch <= 'Z'){
        result = (int)(ch - 'A') + 10;
    }
    else{
        result = -1;
    }
    return result;
}

char valueToHexCh(const int value)
{
    char result = '\0';
    if(value >= 0 && value <= 9){
        result = (char)(value + 48); //48为ascii编码的'0’字符编码值
    }
    else if(value >= 10 && value <= 15){
        result = (char)(value - 10 + 65); //减去10则找出其在16进制的偏移量，65为ascii的'A'的字符编码值
    }
    else{
        ;
    }

    return result;
}

int checkcharhexvalid(char *s)
{
    int i,n = 0;
    for(i = 0; s[i]; i += 2)
    {
        char c = s[i];
        char d = s[i+1];
        if(c < '0' || c > 'f')
        {
            perror("out of range");
            assert(0);
        }
        if(d < '0' || d > 'f')
        {
            perror("out of range");
            assert(0);
        }

    }
}


int hexChartoByte(char *s,char *byte)
{
    int i,n = 0;
    for(i = 0; s[i]; i += 2)
    {
        char fir = s[i];
        char sec = s[i + 1];
        if(fir >= 'A' && fir <= 'F')
            byte[n] = fir - 'A' + 10;
        else
            byte[n] = fir - '0';
        if(sec >= 'A' && sec <= 'F')
            byte[n] = (byte[n] << 4) | (sec - 'A' + 10);
        else
            byte[n] = (byte[n] << 4) | (sec - '0');
        ++n;
    }
    return n;
}

unsigned char ChartoAscii(const unsigned char cha)
{
    unsigned char ascii;
    if ((cha >= 0x0A) && (cha <= 0x0F))
    {
        ascii = cha + 'A' - 10;
    }
    else
    {
        ascii = cha + '0';
    }
    return ascii;
}


