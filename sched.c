#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

struct jobs {
    int jid;
    int arrival;
    int duration;
};

int main() {
    char *p;
    char *delim1 = " ";
    char line[200];
    int i = 0;
    int current;
    FILE *jobList;
    struct jobs input[20000];
    jobList = fopen ("jobs.dat", "r");
    if (jobList == NULL) {
        printf("\nUnable to either read jobs.dat file\n");
        exit (1);
    }
    while(fgets(line, sizeof(line), jobList) != NULL && i < 1000) {
        current = sscanf(line, "%i %i %i", &input[i].jid, &input[i].arrival, &input[i].duration);
        i++;
    }

    //Test Prints to check if jobs.dat has been correctly read;
    i = 0;
    while(i < 10){
        printf("\n%i, %i, %i", input[i].jid, input[i].arrival, input[i].duration);
        i++;
    }
    printf("\n");
    fclose(jobList);
    return 0;
    
}