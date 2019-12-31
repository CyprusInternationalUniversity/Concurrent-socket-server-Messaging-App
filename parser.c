#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"

int main()
{
    dictionary *ini;
    char       *ini_name;
    char *message, *port;

    ini_name = "server.ini";

    ini = iniparser_load(ini_name);
    // iniparser_dump(ini, stdout);

    message = iniparser_getstring(ini, "server:servermsg", NULL);
    port = iniparser_getint(ini, "server:listenport", NULL);
    printf("Server PORT: %d \n", port);
    printf("Server Message: %s \n", message);

    char *USER = "users:";
    char *USER_NO = "99990";
    char userToLookUp[80];
    strcpy(userToLookUp, USER);
    strcat(userToLookUp, USER_NO);
    // printf("User to look up => %s \n", userToLookUp);

    // char *userToLookUp = "users:99990";
    int CHECK = iniparser_find_entry(ini, userToLookUp);
    printf("CHECK: %d \n", CHECK);
    char *pass = iniparser_getstring(ini, userToLookUp, NULL);
    printf("User Pass: %s \n", pass);

    // char *user1;
    // user1 = iniparser_getstring(ini, "users:99990", NULL);
    // printf("User One: %s \n", user1);

    iniparser_freedict(ini);

    return 0;
}