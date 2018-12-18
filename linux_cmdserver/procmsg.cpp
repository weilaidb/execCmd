#include "procmsg.h"
#include <assert.h>
#include <ctype.h>
#include <string>
#include <iostream>


using namespace std;


/*
* 开关打印，不需要加换行
*/
#define switchprint(fmt,args...)\
do{\
	if(switchprintflag)\
	{\
		printf(fmt" F:%s, L:%u\n", ##args, __FUNCTION__, __LINE__);\
	}\
}while(0)


int switchprintflag = 1;

string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value)   
{   
    while(true)   {   
        string::size_type   pos(0);   
        if(   (pos=str.find(old_value))!=string::npos   )   
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}   


int delspace(char *psrcmsg, int len)
{
	char *phead = NULL;
	char *pbuf = (char *)malloc(len);
	if(NULL == pbuf)
	{
		assert(0);
		switchprint("memory faild");
		return -1;
	}
	phead = pbuf;//the pointer move to other position
	memset(pbuf, 0, len);
	
	switchprint("len:%u",len);
	while(*psrcmsg)
	{
		// switchprint("%02x", *psrcmsg);
		if(isalnum(*psrcmsg))
		{
			*phead = *psrcmsg;
			// switchprint(" is data, *phead:%c *psrcmsg:%c phead:%p", *phead, *psrcmsg, phead);			
			phead++;
		}
		else
		{
			// switchprint(" no isalnum!");
		}
		psrcmsg++;
	}
	
	switchprint("after delete space:%s, len:%u", (char *)pbuf, strlen(pbuf));
	if(pbuf)
	{
		free(pbuf);
	}
	
	return 0;
}


int procmsgfromgui(char *msg)
{
	// int len = strlen(msg);
	switchprint("msg content:%s", msg);
	// delspace(msg, len);
	
	const string oldvalue("\n");
	const string newvalue("");
	string resmsg = msg;
	string result;
	result = replace_all(resmsg,oldvalue, newvalue);
	result = replace_all(result,"0x", "");
	result = replace_all(result,"0X", "");
	result = replace_all(result," ", "");
	result = replace_all(result,"msg:", "");
	result = replace_all(result,"msgdone", "");
	cout << "last proc msg:" << result << endl;

	
	
}