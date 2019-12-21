#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "iniparser.h"

/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Definitions */
#define DEFAULT_BUFLEN 512
#define PORT 9341

int main()
{
    dictionary *ini;
    char       *iniName;
    char *message;
    int port;
    int FOUND_USER_FLAG = 0;

    iniName = "server.ini";


    ini = iniparser_load(iniName);
    // iniparser_dump(ini, stdout);

    message = iniparser_getstring(ini, "server:servermsg", NULL);
    port = iniparser_getint(ini, "server:listenport", NULL);
    printf("############################################## \n");
    printf("Server PORT: %d \n", port);
    printf("Server Message: %s \n", message);
    printf("############################################## \n\n\n");

    printf("Please write user details to authenticate in following format shown below \n");
    printf("USER UserID Password \n");

    char *USER_NO;
    char userToLookUp[80];

    char str1[6], userID[10], password[10];
    // scanf("%[^\n]", str1, str2, str3);
    // int a, b;
	scanf("%s%s%s", &str1, &userID, &password);


    printf("\n\n############################################## \n");
    printf("You entered following: %s %s %s \n", str1, userID, password);

    // strcpy(USER_NO, userID);
    strcpy(userToLookUp, "users:");
    strcat(userToLookUp, userID);

    printf("############################################## \n\n\n");
    printf("############################################## \n");
    printf("Searching for authentication \n");
    printf("############################################## \n\n");
    int CHECK = iniparser_find_entry(ini, userToLookUp);
    printf("DB CHECK: %d \n", CHECK);

    if(!CHECK) {
        printf("-ERR UserID is wrong. \n\n");
        return 0;
    }

    char *dbPass = iniparser_getstring(ini, userToLookUp, NULL);
    // printf("DB Pass: %s \n", dbPass);
    if( strcmp(password, dbPass) == 0 ) {
        FOUND_USER_FLAG = 1;
    } else {
        FOUND_USER_FLAG = 0;
        printf("-ERR password is wrong. \n\n");
        return 0;
    }

    if(FOUND_USER_FLAG) {

        int server, client;
        struct sockaddr_in local_addr;
        struct sockaddr_in remote_addr;
        int length,fd,rcnt,optval;
        char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
        int  recvbuflen = DEFAULT_BUFLEN;

        /* Open socket descriptor */
        if ( (server = socket( AF_INET, SOCK_STREAM, 0 )) < 0) {
            perror("Can't create socket!");
            return(1);
        }

        /* Fill local and remote address structure with zero */
        memset( &local_addr, 0, sizeof(local_addr) );
        memset( &remote_addr, 0, sizeof(remote_addr) );

        /* Set values to local_addr structure */
        local_addr.sin_family = AF_INET;
        local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        local_addr.sin_port = htons(PORT);

        // set SO_REUSEADDR on a socket to true (1):
        optval = 1;
        setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);


        if ( bind( server, (struct sockaddr *)&local_addr, sizeof(local_addr) ) < 0 ) {
            /* could not start server */
            perror("Bind problem!");
            return(1);
        }

        if ( listen( server, SOMAXCONN ) < 0 ) {
            perror("Listen");
            exit(1);
        }

        printf("First socket server now starting!\n");
        printf("Wait for connection\n");

        while(1) {  // main accept() loop
            length = sizeof remote_addr;
            if ((fd = accept(server, (struct sockaddr *)&remote_addr, \
                &length)) == -1) {
                perror("Accept Problem!");
                continue;
            }

            printf("Server: got connection from %s\n", \
                inet_ntoa(remote_addr.sin_addr));
        
        

            // Receive until the peer shuts down the connection
            do {
                // Clear Receive buffer
                memset( &recvbuf, '\0', sizeof(recvbuf) );
                rcnt = recv(fd, recvbuf, recvbuflen, 0);
                if (rcnt > 0) {
                    printf("Bytes received: %d\n", rcnt);

                // Echo the buffer back to the sender
                rcnt = send( fd, recvbuf, rcnt, 0 );
                    if (rcnt < 0) {
                        printf("Send failed:\n");
                        close(fd);
                        break;
                    }
                    printf("Bytes sent: %d\n", rcnt);
                } else if (rcnt == 0)
                    printf("Connection closing...\n");
                else  {
                    printf("Receive failed:\n");
                    close(fd);
                    break;
                }
            } while (rcnt > 0);

        }
        // Final Cleanup
        close(server);
    } else {
        return 0;
    }
}
