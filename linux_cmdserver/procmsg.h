#ifndef __PROCMSG_H__
#define __PROCMSG_H__


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/socket.h>




int procmsgfromgui(char *msg);












#endif /**__PROCMSG_H__ **/