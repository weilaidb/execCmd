#ifndef MSGTIPS_H
#define MSGTIPS_H

#include <QString>

class msgtips
{
public:
    msgtips();
};
void ShowTipsInfo(QString s) ;
void ShowTipsInfoWithShowTime(QString s, quint32 ultimeout);

#endif // MSGTIPS_H
