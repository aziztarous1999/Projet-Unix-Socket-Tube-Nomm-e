#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include "serv_cli_fifo.h"
#include "Handlers_Cli.h"

#include "table.h"

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

	/**/
	/* Déclarations */
	Question q;
	Reponse r;
	int i;
	
	/*initialisation du générateur de nombres aléatoires*/
	srand(getpid());
	
	void afficher_reponse(){
		printf("\n\x1B[32m╔═══════════════════════════════════════╗\n");
		printf("║\t     *** CLIENT ***\t        ║\n");
		printf("╠═══════════════════════════════════════╣\n");
		printf("║\t        Reponse:\t        ║\n");
		printf("╚═══════════════════════════════════════╝\n\x1B[0m");
		/* un affichage amélioré( sous forme d'un tableau) */
		ft_table_t *table = ft_create_table();
		/* Change border style */
		ft_set_border_style(table, FT_DOUBLE2_STYLE);

		/* Set "header" type for the first row */
		ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
		ft_write_ln(table, "Indice du reponse", "Valeur du reponse");
		

		for (i = 0; i < q.n; i++)
		{
			char indice[10];
			char valeur[10];
			sprintf(indice, "%d", i);
			sprintf(valeur, "%d", r.tab[i]);
			char line[20];
			sprintf(line, "- %d", r.tab[i]);
			writeFile(line);
			ft_write_ln(table, indice,valeur);
		};

		/* Set center alignment for the 1st and second columns */
		ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
		ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);

		printf("%s\n", ft_to_string(table));
		ft_destroy_table(table);
	}
	
	
	
	/* handler */
	void hand_reveil(int sig){
	
	/* Lecture de la réponse */
	r.tab = malloc( sizeof(int) * q.n );
	
	/* Ouverture des tubes nommés */
	int fdread2 = open(FIFO2, O_RDONLY);
	read(fdread2, &r.pid, sizeof(int));
	read(fdread2, r.tab, sizeof(int) * q.n);
	close(fdread2);

	kill(r.pid,SIGUSR1);
	writeFile("(Client)-> Reponse:");
	/* Traitement local de la réponse */
	afficher_reponse();
	
	
	
	/* liberation d'allocation */
	free(r.tab);
	}
	
	/* Installation des Handlers */
	signal(SIGUSR1,hand_reveil);
	
	/* Construction et envoi d’une question */
	q.n = NMIN + rand() % (NMAX+1 - NMIN);
	q.pid = getpid();
	/* Ouverture des tubes nommés */
	int fdwrite = open(FIFO1, O_WRONLY);
	int qes = q.n;
	/* ecriture du question au fichier */
	char line[200];
	sprintf(line, "(Client)-> Envoi du question du client qui est :  %d .", qes);
	writeFile(line);
	
	write(fdwrite, &q, sizeof(Question));
	close(fdwrite);
	
	/* Attente de la réponse */
	pause();
	exit(1);
	
}
