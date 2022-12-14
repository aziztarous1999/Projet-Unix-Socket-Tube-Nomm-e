#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include "serv_cli_fifo.h"
#include "Handlers_Serv.h"



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




void main(int argc, char *argv[])
{
	/* Déclarations */
	int i;
	int x=1;
	Question q;
	Reponse r;
	int mk;
	int mk2;

	/* Création des tubes nommés */
	mk = mkfifo(FIFO1, 0777);
	mk2 = mkfifo(FIFO2, 0777);

	/*initialisation du générateur de nombres aléatoires*/
	srand(getpid());
	

	
	/* initialisation des Handlers */
	void hand_reveil(int sig){
		sleep(3);
		printf("-> Le reponse a été recu d'aprés le client(%d).\n",q.pid);
		writeFile("(SERVER)-> Le reponse a été recu d'aprés le client.");
		printf("═════════════════════════════════════════════════════════\n\n");
	}
	
	void fin_serveur(int sig){
		/* liberation d'allocation */
		free(r.tab);
		unlink(FIFO1);
		unlink(FIFO2);
		x=0;
		/* affichage */
		printf("\033[0;31m\n╔═══════════════════════════════════════════════════════╗\n");
		printf("║\t       *** Fermeture Du Serveur ***\t        ║\n");
		printf("╚═══════════════════════════════════════════════════════╝\n\n\n\n\x1B[0m");
		writeFile("*** Fermeture Du Serveur ***");
		exit(1);
	}
	
	/* affichage */
	printf("\x1B[32m╔═══════════════════════════════════════════════════════╗\n");
	printf("║\t       *** Ouverture Du Server ***\t        ║\n");
	printf("╚═══════════════════════════════════════════════════════╝\n\n\x1B[0m");
	writeFile("*** Ouverture Du Server ***");
	

	
	while(x){
	
		/* Installation des Handlers */
		for(i=1;i<NSIG;i++){
		signal(i,fin_serveur);
		}
		signal(SIGUSR1,hand_reveil);
		
		/* lecture d’une question */
		/* Ouverture des tubes nommés */
		int fdread = open(FIFO1, O_RDONLY);
		read(fdread, &q, sizeof(Question));
		close(fdread);
		
		printf("-> Question du %d est %d.\n",q.pid,q.n);
		writeFile("(SERVER)-> Le question du client a ete recu.");
		/* construction de la réponse */
		r.pid = getpid();
		r.tab = malloc( sizeof(int) * q.n );
		for (i = 0; i < q.n; i++)
		{
			r.tab[i] = NMIN + rand() % (NMAX+1 - NMIN);
		};
		
		/* envoi du signal SIGUSR1 au client concerné */
		/* attender pour le registration du handler au signal 1 au niveau du client */
		sleep(1);
		kill(q.pid,SIGUSR1);
		
		/* envoi de la réponse */
		/* Ouverture des tubes nommés */
		writeFile("(SERVER)-> Le reponse a ete envoyer!");
		int fdwrite2 = open(FIFO2, O_WRONLY);
		write(fdwrite2, &r.pid, sizeof(int));
		write(fdwrite2, r.tab, (sizeof(int) * q.n) );
		close(fdwrite2);
		pause();
		
	}


}
