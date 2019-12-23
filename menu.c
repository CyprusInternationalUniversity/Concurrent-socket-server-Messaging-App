#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <getopt.h>
#include <stdbool.h>

char *serverip;
char *port;
char *username;
char *userid;
char *userpass;
char *recieverid;

int main(){
	
	mainmenu();
	
	return 0;
}


void mainmenu(void);





void mainmenu(void)
{

    char *title;

	 
	
    char choise=' ';
    do
    {   char flag=' ';
        while(flag==' ')
        {
        printf("\n\n 	Menu	\n\n");
        printf("   Please, Choose Your Option\n");
        printf("1-) Read/Delete Messages\n");
        printf("2-) Write Message to User\n");
        printf("3-) Change Config Parameters\n");
        printf("4-) QUIT\n");
        printf("Option >");

        
        scanf("%c", &choise);
        
        system("cls");

       

        switch (choise)
        
        if(choise == '1'){
        	title="     Read/Delete Messages \n";
			printf ("1- Read/ Delete Messages \n");  
		    scanf("%s",&recieverid);
                	
            break;         
          
        }
		 
		if(choise == '2'){
			
			title="     Write Message to User \n";
            
            printf(" selected 2 ");scanf("%s",&recieverid);                     
            break;		
}
						
		}if(choise == '3'){	
			title="     Change config parameters \n";								
            printf("3- Change Config Parameters \n"); 
			scanf("%s", &choise);           
            break;	
					
		}if(choise == '4'){
			printf("4- Quit \n ");
			break;			
		}else{
            break;
		}		
        scanf("%c", &flag);

        }

    
    while(choise!='4');
}


