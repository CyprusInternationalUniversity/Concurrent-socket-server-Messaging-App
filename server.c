#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h> 

#include "iniparser.h"

/* Socket API headers */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Definitions */
#define DEFAULT_BUFLEN 512
#define PORT 9341

void do_job(int fd);
void createSocketConnection();
bool authentication(char *buffer,int size,char userId[]);
void displayWelcomeServer(bool isClient, int fd, int rcnt) {
    dictionary *ini;
    char       *iniName;
    char *message;
    int port;
    // int FOUND_USER_FLAG = 0;
    time_t timeVar;
    struct tm * timeDetail;
    timeDetail = localtime ( &timeVar );

    iniName = "server.ini";
    ini = iniparser_load(iniName);
    // iniparser_dump(ini, stdout);

    message = iniparser_getstring(ini, "server:servermsg", NULL);
    port = iniparser_getint(ini, "server:listenport", NULL);

    if(!isClient) {
        printf("\n\n\n############################################## \n");
        printf("Server PORT: %d \n", port);
        printf("Server Message: %s \n", message);
        printf("%s", asctime (timeDetail) );
        printf("############################################## \n\n\n");
    } else {
        rcnt = send(fd, "##############################################\n ",strlen("##############################################\n"),0);
        rcnt = send(fd, "Welcome to ",strlen("Welcome to "),0);
        rcnt = send(fd, message, strlen(message),0);
        rcnt = send(fd, "\n ",strlen("\n"),0);
        rcnt = send(fd, "##############################################\n ",strlen("##############################################\n"),0);
    }
}

int main() {
    displayWelcomeServer(false, 0, 0);

    // authentication();
    createSocketConnection();

    return 0;
}

bool isAuthenticated(char *buffer, int size, int fd) {
    int rcnt;

    dictionary *ini;
    char       *iniName;
    int FOUND_USER_FLAG = 0;

    iniName = "server.ini";
    ini = iniparser_load(iniName);

    // Remove next line at the end
    buffer[size-1]=0;
    // https://stackoverflow.com/questions/19803437/strtok-to-separate-all-whitespace
    char delim[] = " ";

    char *ptr = strtok(buffer, delim);
    char *USER_NO, *pass;
    int count = 0;
    while (ptr != NULL) {
        /*
            * Test results
            * printf("%d %s\n", count, ptr);
            0 USER
            1 9999
            2 pass
        */
        // printf("%d %s\n", count, ptr);
        if(count == 0 && strcasecmp(ptr,"user") != 0) {
            printf("Error wrong key used should be user or USER\n");
        } else if(count == 1) {
           USER_NO = ptr;
        } else if(count == 2) {
           pass = ptr;
        }
        ptr = strtok(NULL, delim);
        count++;
    }
    printf("USER ID %s\n", USER_NO );
    printf("USER PASS %s\n", pass );
	printf("\n");
    // char *USER_NO;
    char userToLookUp[80];

    // char str1[6], userID[10], password[10];
    // // scanf("%[^\n]", str1, str2, str3);
    // // int a, b;
	// scanf("%s%s%s", &str1, &userID, &password);

    // printf("\n\n############################################## \n");
    // printf("You entered following: %s %s %s \n", str1, userID, password);

    // strcpy(USER_NO, userID);
    strcpy(userToLookUp, "users:");
    strcat(userToLookUp, USER_NO);

    printf("############################################## \n\n\n");
    printf("############################################## \n");
    printf("Searching for authentication \n");
    printf("############################################## \n\n");
    int CHECK = iniparser_find_entry(ini, userToLookUp);
    printf("DB CHECK: %d \n", CHECK);

    if(!CHECK) {
        printf("-ERR UserID is wrong. \n\n");
        rcnt = send(fd, "-ERR UserID is wrong.\n ",strlen("-ERR UserID is wrong.\n"),0);
        return 0;
    }

    char *dbPass = iniparser_getstring(ini, userToLookUp, NULL);
    // printf("DB Pass: %s \n", dbPass);
    if( strcmp(pass, dbPass) == 0 ) {
        FOUND_USER_FLAG = 1;
    } else {
        FOUND_USER_FLAG = 0;
        printf("-ERR password is wrong.\n\n");
        rcnt = send(fd, "-ERR password is wrong.\n ",strlen("-ERR password is wrong.\n"),0);
        return 0;
    }

    if(FOUND_USER_FLAG) {
        printf("\n############################################## \n");
        printf("AUTHENTICATED! USER: %s is connected to the Server!\n", USER_NO);
        printf("############################################## \n\n");
        // createSocketConnection();
        // Notify client
        rcnt = send(fd, "-OK Connection Established Successfully!.\n ",strlen("-OK Connection Established Successfully!.\n"),0);
        return true;
    }
    rcnt = send(fd, "-ERR Unexpected connection error!.\n ",strlen("-ERR Unexpected connection error!\n"),0);
    return false;
}

void do_job(int fd)
{
    int length,rcnt;
    char recvbuf[DEFAULT_BUFLEN],bmsg[DEFAULT_BUFLEN];
    int  recvbuflen = DEFAULT_BUFLEN;
    char studentID[] = {}; // 99991
    /*
    *   Authenticate user
    *  */
    bool is_authenticated = false ;
    do {
        rcnt = send(fd, "Enter details in the following format <USER> <UserID> <Password>\n ",strlen("Enter details in the following format <USER> <UserID> <Password>\n"),0);
        rcnt = recv(fd, recvbuf, recvbuflen, 0);
        is_authenticated = isAuthenticated(recvbuf, rcnt, fd);
    } while(!is_authenticated);
    //  After authenticate display message server
    displayWelcomeServer(true, fd, rcnt);
    // show infinite time so that User can choose again and again
    char userSelection = 0;
    char userChoosed[1000];
    while (1) {
        rcnt = send(fd, "\nPlease choose your option NUMBER:\n", strlen("\nPlease choose your option NUMBER:\n"), 0);
        rcnt = send(fd, "1 - Read/Delete Messages\n", strlen("1 - Read/Delete Messages\n"), 0);
        rcnt = send(fd, "2 - Write Message to User\n", strlen("2 - Write Message to User\n"), 0);
        rcnt = send(fd, "3 - Change config parameters\n", strlen("3 - Change config parameters\n"), 0);
        rcnt = send(fd, "4 - Quit\n", strlen("4 - Quit\n"), 0);
        rcnt = send(fd, "Option->", strlen("Option->"), 0);
        rcnt = recv(fd, recvbuf, recvbuflen, 0);
    
        // printf("%s", recvbuf);
        strncpy(userChoosed, recvbuf, 4);

        if (strncasecmp(userChoosed,"1",4 ) == 0) 
        {
            userSelection = 1;
        }
        else if (strncasecmp(userChoosed,"2",4 ) == 0) 
        {
          userSelection = 2;
        }
        else if(strncasecmp(userChoosed,"3",4 ) == 0)
        {
            userSelection = 3;
        }
        else
        {
            userSelection = 4;
        }
        printf("userSelection = %s", userSelection);
        switch (userSelection)
        {
        case 1:
            printf("Send mesage to user");
            break;
        case 4:
            rcnt = send(fd, "Bye!\n", strlen("Bye!\n"), 0);
            return 0;
        default:
            break;
        }
    }

    // Receive until the peer shuts down the connection
    do {
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

        }
        else if (rcnt == 0)
            printf("Connection closing...\n");
        else  {
            printf("Receive failed:\n");
            close(fd);
            break;
        }
    } while (rcnt > 0);
}

void createSocketConnection() {

    int server, client;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    int length,fd,rcnt,optval;
    pid_t pid;

    /* Open socket descriptor */
    if ((server = socket( AF_INET, SOCK_STREAM, 0)) < 0 ) { 
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

    if ( bind( server, (struct sockaddr *)&local_addr, sizeof(local_addr) ) < 0 )
    {
        /* could not start server */
        perror("Bind error");
        return(1);
    }

    if ( listen( server, SOMAXCONN ) < 0 ) {
            perror("listen");
            exit(1);
    }

    printf("Concurrent socket server now starting on port %d\n",PORT);
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

        /* If fork create Child, take control over child and close on server side */
        if ((pid=fork()) == 0) {
            close(server);
            do_job(fd);
            printf("Child finished their job!\n");
            close(fd);
            exit(0);
        }

    }

    // Final Cleanup
    close(server);
}
