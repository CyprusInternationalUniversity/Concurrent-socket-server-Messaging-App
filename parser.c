#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"

int main(int argc, char * argv[])
{
    dictionary * ini ;
    char       * ini_name ;
    char *message, *port;
    char *user1;

    if (argc<2) {
      ini_name = "server.ini";
    } else {
      ini_name = argv[1] ;
    }

    ini = iniparser_load(ini_name);
    // iniparser_dump(ini, stdout);

    message = iniparser_getstring(ini, "server:servermsg", NULL);
    port = iniparser_getint(ini, "server:listenport", NULL);
    printf("Server PORT: %d \n", port);
    printf("Server Message: %s \n", message);

    user1 = iniparser_getstring(ini, "users:99990", NULL);
    printf("User One: %s \n", user1);

    iniparser_freedict(ini);

    return 0 ;
}