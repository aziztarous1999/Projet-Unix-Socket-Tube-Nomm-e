#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "table.h"
#include <signal.h>

#define NMIN 1
#define NMAX 10





void writeFile(char *line){
	FILE *out_file = fopen("resultat.txt", "a"); //appending
	// test for files not existing. 
	if (out_file == NULL) 
	{   
		printf("Error! Could not open file\n"); 
		exit(-1); // must include stdlib.h 
	}

	fputs(line,out_file); // write to file 
	fputs("\n",out_file); 
	fclose(out_file);

}




void fin_serveur(int sig){
		/* affichage */
		printf("\033[0;31m\n╔═══════════════════════════════════════════════════════╗\n");
		printf("║\t       *** Fermeture Du Serveur ***\t        ║\n");
		printf("╚═══════════════════════════════════════════════════════╝\n\n\x1B[0m");
		writeFile("*** Fermeture Du Serveur ***");
		exit(1);
	}


int main()
{


    /* Installation des Handlers */
		for(int i=1;i<NSIG;i++){
		signal(i,fin_serveur);
		}


    /*initialisation du générateur de nombres aléatoires*/
	srand(getpid());


    // reciperation du question
	int question;
	struct sockaddr_in servaddr;
	
	int fifo1 = socket(AF_INET, SOCK_DGRAM, 0);
	if(fifo1 == -1)
	{
		perror("failed to create socket");
		exit(EXIT_FAILURE);
	}
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(4000);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	int rc = bind(fifo1, (const struct sockaddr *)&servaddr, 
		sizeof(servaddr));
		
	if(rc == -1)
	{
		perror("failed to bind");
		close(fifo1);
		exit(EXIT_FAILURE);
	}
	socklen_t len = 0;
	


	/* affichage */
	printf("\x1B[32m╔═══════════════════════════════════════════════════════╗\n");
	printf("║\t       *** Ouverture Du Server ***\t        ║\n");
	printf("╚═══════════════════════════════════════════════════════╝\n\n\x1B[0m");
	writeFile("*** Ouverture Du Server ***");
	
	
    //attente des question
    while(1){

    	int n = recvfrom(fifo1, &question, sizeof(question), 0,
		0, &len);
    printf("-> Le question du client est %d.\n",question);
	writeFile("(SERVER)-> Le question du client a ete recu.");
    sleep(1);
    //envoi du reponse

    int reponse[question];


	struct sockaddr_in servaddr2;
	
	int fifo2 = socket(AF_INET, SOCK_DGRAM, 0);
	if(fifo2 == -1)
	{
		perror("failed to create socket");
		exit(EXIT_FAILURE);
	}
	
	servaddr2.sin_family = AF_INET;
	servaddr2.sin_port = htons(5000);
	servaddr2.sin_addr.s_addr =  inet_addr("127.0.0.1");;
	

    //creation du reponse
    for (int i = 0; i < question; i++)
		{
			reponse[i] = NMIN + rand() % (NMAX+1 - NMIN);
		};


	writeFile("(SERVER)-> Le reponse a ete envoyer!");
	int len2 = sendto(fifo2, reponse, sizeof(reponse),
		0, (const struct sockaddr *)&servaddr2, sizeof(servaddr2));
	if(len2 ==-1)
	{
		perror("failed to send");
	}
    close(fifo2);
    printf("-> Le reponse a été recu d'aprés le client.\n");
	writeFile("(SERVER)-> Le reponse a été recu d'aprés le client.");
    printf("═════════════════════════════════════════════════════════\n\n");
}




	close(fifo1);



printf("\033[0;31m\n╔═══════════════════════════════════════════════════════╗\n");
printf("║\t       *** Fermeture Du Serveur ***\t        ║\n");
printf("╚═══════════════════════════════════════════════════════╝\n\n");

    return 0;
}