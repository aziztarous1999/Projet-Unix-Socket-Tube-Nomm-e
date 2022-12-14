#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "table.h"


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



int main()
{
    

    /*initialisation du générateur de nombres aléatoires*/
	srand(getpid());
    int question= NMIN + rand() % (NMAX+1 - NMIN);
    int reponse[question];

    //envoi du message
	char * hello = "hello from client";
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
	char line[200];
	sprintf(line, "(Client)-> Envoi du question du client qui est :  %d .", question);
	writeFile(line);
	int len = sendto(fifo1, &question, sizeof(question),
		0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	if(len ==-1)
	{
		perror("failed to send");
	}




    // reciperation du reponse
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
	
	int rc = bind(fifo2, (const struct sockaddr *)&servaddr2, 
		sizeof(servaddr2));
		
	if(rc == -1)
	{
		perror("failed to bind");
		close(fifo2);
		exit(EXIT_FAILURE);
	}
	socklen_t len2 = 0;
	

    int n = recvfrom(fifo2, reponse, sizeof(reponse), 0,
    0, &len2);
	//printf("reponse : %d %d %d\n", reponse[0], reponse[1], reponse[2]);

	writeFile("(Client)-> Reponse:");


    //afficher la reponse
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


		for (int i = 0; i < question; i++)
		{
			char indice[10];
			char valeur[10];
			sprintf(indice, "%d", i);
			sprintf(valeur, "%d", reponse[i]);
			char line[20];
			sprintf(line, "- %d", reponse[i]);
			writeFile(line);
			ft_write_ln(table, indice,valeur);
		};

		/* Set center alignment for the 1st and second columns */
		ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
		ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);

		printf("%s\n", ft_to_string(table));
		ft_destroy_table(table);








	close(fifo2);




    close(fifo1);

	
    return 0;
}