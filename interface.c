#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <gtk/gtk.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

//gcc -o MyApp interface.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic



int argc;
char **argv;






// methodes d'affichage

// Variables
GtkWidget *window1; 
GtkWidget *windowsocket; 
GtkWidget *homemenu; 
GtkWidget *menuwindow;
GtkLabel *affichage; // Label
GtkLabel *affichagesocket; // Label


char text[10000]  = "texte a afficher"; 



void readResult(){
    text[0] = '\0';
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("resultat.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        strcat(text,line);
        gtk_label_set_text(affichage, text);
    }

    fclose(fp);
    if (line)
        free(line);
}



void stop()
{
	printf("stop the programme \n");
    gtk_widget_destroy(GTK_WIDGET(window1));
}



// Function when our button is pressed
void start()
{

    pid_t child1, child2;
    printf("Start\n");
    if (!(child1 = fork())) {
        
        // first childi
        sleep(1);
		execlp("./tube/client", "client", (char *)NULL);
       
        sleep(1);
        
        exit(0);
    } else if (!(child2 = fork())) {

        // second child
         execlp("./tube/server", "server", (char *)NULL);
         
        sleep(1);
        exit(0);
    } else {
        // parent
        printf("Parent\n");
        wait(&child1);
            printf("got exit status from child 1\n");
            
            kill(child2,SIGUSR2);
        wait(&child2);
            printf("got exit status from child 2\n");
            readResult();
    }
    

	
	gtk_label_set_text(affichage, text);
}
 
void clear()
{
	printf("clear\n");
	text[0] = '\0';
	gtk_label_set_text(affichage, text);
    remove("resultat.txt");
}


void menuTubeClick(GtkWidget *widget,gpointer data,int argc,char *argv[]){
	GtkBuilder *builder; 
    // Window
	gtk_init(&argc, &argv); 

	builder = gtk_builder_new(); 
	gtk_builder_add_from_file(builder, "test.glade", NULL); 

	// Assign the Variables
	window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	affichage = GTK_LABEL(gtk_builder_get_object(builder, "affichage")); 
	g_signal_connect(window1, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	// Essential for a GTK based program
	gtk_builder_connect_signals(builder, NULL); 
	g_object_unref(builder);
	
	gtk_widget_show_all(window1); 
	gtk_main(); 

}



/////////////////////////// interface graphique socket



// Function when our button is pressed
void startsocket()
{

    pid_t child1, child2;
    printf("Start\n");
    if (!(child1 = fork())) {
        
        // first childi
        sleep(1);
		execlp("./socket/client", "client", (char *)NULL);
       
        sleep(1);
        
        exit(0);
    } else if (!(child2 = fork())) {

        // second child
         execlp("./socket/server", "server", (char *)NULL);
         
        sleep(1);
        exit(0);
    } else {
        // parent
        printf("Parent\n");
        wait(&child1);
            printf("got exit status from child 1\n");
            
            kill(child2,SIGUSR2);
        wait(&child2);
            printf("got exit status from child 2\n");
            readResult();
    }
    

	
	gtk_label_set_text(affichagesocket, text);
}

void clearsocket()
{
	printf("clear socket \n");
	text[0] = '\0';
	gtk_label_set_text(affichagesocket, text);
    remove("resultat.txt");
}

void stopsocket()
{
	printf("stop the socket programme \n");
    gtk_widget_destroy(GTK_WIDGET(windowsocket));
}


void menuSocketClick(GtkWidget *widget,gpointer data,int argc,char *argv[]){
	GtkBuilder *builder; 
    // Window
	gtk_init(&argc, &argv); 

	builder = gtk_builder_new(); 
	gtk_builder_add_from_file(builder, "socket.glade", NULL); 

	// Assign the Variables
	windowsocket = GTK_WIDGET(gtk_builder_get_object(builder, "windowsocket"));
	affichagesocket = GTK_LABEL(gtk_builder_get_object(builder, "affichagesocket")); 
	g_signal_connect(windowsocket, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	// Essential for a GTK based program
	gtk_builder_connect_signals(builder, NULL); 
	g_object_unref(builder);
	
	gtk_widget_show_all(windowsocket); 
	gtk_main(); 

}


///////////////////////////////////////////////////////////////////////

GtkWidget *button,*fifo,*sjf,*prio,*priopreemp,*tour,*label;


char* resultat="\n\t\tResultat:\n";
char a[1000] = "";

void messageResultat(int argc,char *argv[]){
    
    strcat(a,resultat);
    strcat(a," abcd");
    gtk_label_set_text(GTK_LABEL(label), a);
}














void socketCallback(GtkWidget *widget,gpointer data,int argc,char *argv[])
{
    
    system("./tube/test");
}





void guiMenu(int argc,char *argv[]){

const gchar *const title ="Menu";

    GtkWidget *window, *grid, *label,*br1,*br2,*br4,*br5;
    gtk_init(&argc, &argv);


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);


    gtk_window_set_title(GTK_WINDOW(window), title);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label=gtk_label_new("\n\t\t>>>>\t Sélectionner le type du programme \t<<<<\t\t");
    br1=gtk_label_new("\t");
    br2=gtk_label_new("\n");
    br4=gtk_label_new("\n");
    br5=gtk_label_new("\n");


    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), br1, 0, 1, 1, 1);




  int k=2;




    fifo = gtk_button_new_with_label ("\t Tube Nommée\t");
    g_signal_connect(fifo,"clicked",G_CALLBACK(menuTubeClick),NULL);
    gtk_grid_attach(GTK_GRID(grid), fifo, 0, k, 1, 1);
    k++;

   gtk_grid_attach(GTK_GRID(grid), br2 , 0, k, 1, 1);
    k++;

    sjf = gtk_button_new_with_label ("\t Socket\t");
    g_signal_connect(sjf,"clicked",G_CALLBACK(socketCallback),NULL);
    gtk_grid_attach(GTK_GRID(grid), sjf, 0, k, 1, 1);
    k++;


    gtk_grid_attach(GTK_GRID(grid), br4, 0, k, 1, 1);
    k++;

    button = gtk_button_new_with_label ("Fermer");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    gtk_grid_attach(GTK_GRID(grid), button, 0, k, 1, 1);
    k++;

    gtk_grid_attach(GTK_GRID(grid), br5, 0, k, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();



}











void guiMenuInterface(int argc,char *argv[]){




	GtkBuilder *builder; // GTK Builder variable
	gtk_init(&argc, &argv); // Start GTK

	builder = gtk_builder_new(); // Create GTK UI Builder
	gtk_builder_add_from_file(builder, "menuinterface.glade", NULL); // Load our UI file

	// Assign the Variables
	menuwindow = GTK_WIDGET(gtk_builder_get_object(builder, "mymenuwindow")); // Load our window named MyWindow

	// Essential for a GTK based program
	gtk_builder_connect_signals(builder, NULL); 
	g_object_unref(builder);
	
	gtk_widget_show_all(menuwindow); // Show our window
	gtk_main(); // Run our program



}




void closehome()
{
	gtk_widget_destroy(GTK_WIDGET(homemenu));
    guiMenuInterface(argc,argv);
}
 


void menuQuitterClick()
{
	exit(0);
}



void gui(int argc,char *argv[]){




	GtkBuilder *builder; // GTK Builder variable
	gtk_init(&argc, &argv); // Start GTK

	builder = gtk_builder_new(); // Create GTK UI Builder
	gtk_builder_add_from_file(builder, "homeinterface.glade", NULL); // Load our UI file

	// Assign the Variables
	homemenu = GTK_WIDGET(gtk_builder_get_object(builder, "homewindow")); // Load our window named MyWindow

	// Essential for a GTK based program
	gtk_builder_connect_signals(builder, NULL); 
	g_object_unref(builder);
	
	gtk_widget_show_all(homemenu); // Show our window
	gtk_main(); // Run our program



}





int main(int argc,char *argv[])
{

argc = argc;
argv = argv;

gui(argc,argv);


return 0;
}








