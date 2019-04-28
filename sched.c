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
    FILE *jobList;
    struct jobs input;
    jobList = fopen ("jobs.dat", "r");
    if (jobList == NULL) {
        printf("\nUnable to either read jobs.dat file\n");
        exit (1);
    }

//    while(fread(&input, sizeof(struct jobs), 1, jobList)){
//
//    }
    fclose(jobList);
    return 0;
    
}