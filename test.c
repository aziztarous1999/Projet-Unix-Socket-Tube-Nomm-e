#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>



int main() {
    pid_t child1, child2;
    printf("Start\n");
    if (!(child1 = fork())) {
        
        // first childi
		execlp("./client", "client", (char *)NULL);
       
        sleep(5);
        
        exit(0);
    } else if (!(child2 = fork())) {

        // second child
         execlp("./server", "server", (char *)NULL);
         
        sleep(5);
        exit(0);
    } else {
        // parent
        printf("Parent\n");
        wait(&child1);
            printf("got exit status from child 1\n");
            
            kill(child2,SIGUSR2);
        wait(&child2);
            printf("got exit status from child 2\n");
    }

    return 0;
}
