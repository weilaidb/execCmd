#ifndef DATACONVERT_H
#define DATACONVERT_H
int strToHex(char *ch, char *hex);
int hexToStr(char *hex, char *ch);
int hexCharToValue(const char ch);
char valueToHexCh(const int value);
int hexChartoByte(char *s,char *byte);
unsigned char ChartoAscii(const unsigned char cha);
int checkcharhexvalid(char *s);
#endif // DATACONVERT_H
