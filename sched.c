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
#include <time.h>

struct jobs {
    int jid, arrival, duration;
};

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    char line[200];
    int i = 0, all = 1, current, forFIFO, forSJF, forBJF, forSTCF, forRR;
    FILE *jobList;
    struct jobs input[100];
    struct jobs FIFO[100];
    struct jobs SJF[100];
    struct jobs BJF[100];
    struct jobs STCF[100];
    struct jobs RR[100];
    time_t theTime;
    struct tm * timeinfo;
    time (&theTime);
    timeinfo = localtime(&theTime);
    long averager;
    time_t start = theTime;
    jobList = fopen ("jobs.dat", "r");
    if (jobList == NULL) {
        printf("\nUnable to either read jobs.dat file\n");
        exit (1);
    }
    while(fgets(line, sizeof(line), jobList) != NULL && i < all) {
        current = sscanf(line, "%i %i %i", &input[i].jid, &input[i].arrival, &input[i].duration);
        all++;
        i++;
    }
    all--;
    fclose(jobList);
    i = 0;
    while(i < all){
        FIFO[i] = input[i];
        SJF[i] = input[i];
        BJF[i] = input[i];
        STCF[i] = input[i];
        RR[i] = input[i];
        i++;
    }
    int m, n;
    for(m = 0; m < all-1; m++)
    {
        for(n = 0; n < all-1; n++)
        {
            if(FIFO[n].arrival > FIFO[n+1].arrival) {
                swap(&FIFO[n].arrival, &FIFO[n+1].arrival);
                swap(&FIFO[n].jid, &FIFO[n+1].jid);
                swap(&FIFO[n].duration, &FIFO[n+1].duration);
            }
        }
        
    }

    for(m = 0; m < all-1; m++)
    {
        for(n = 0; n < all-1; n++)
        {
            if(RR[n].arrival > RR[n+1].arrival) {
                swap(&RR[n].arrival, &RR[n+1].arrival);
                swap(&RR[n].jid, &RR[n+1].jid);
                swap(&RR[n].duration, &RR[n+1].duration);
            }
        }
        
    }

    for(m = 0; m < all-1; m++)
    {
        for(n = 0; n < all-1; n++)
        {
            if(SJF[n].duration > SJF[n+1].duration) {
                swap(&SJF[n].arrival, &SJF[n+1].arrival);
                swap(&SJF[n].jid, &SJF[n+1].jid);
                swap(&SJF[n].duration, &SJF[n+1].duration);
            }
        }
        
    }

    for(m = 0; m < all-1; m++)
    {
        for(n = 0; n < all-1; n++)
        {
            if(STCF[n].duration > STCF[n+1].duration) {
                swap(&STCF[n].arrival, &STCF[n+1].arrival);
                swap(&STCF[n].jid, &STCF[n+1].jid);
                swap(&STCF[n].duration, &STCF[n+1].duration);
            }
        }
    }
    for(m = 0; m < all-1; m++)
    {
        for(n = 0; n < all-1; n++)
        {
            if ((STCF[n].arrival + STCF[n].duration) > (STCF[n+1].arrival + STCF[n+1].duration)){
                swap(&STCF[n].arrival, &STCF[n+1].arrival);
                swap(&STCF[n].jid, &STCF[n+1].jid);
                swap(&STCF[n].duration, &STCF[n+1].duration);
            }
        }
    }
    
    i = 0;
    while(i <= all){
        BJF[i-1] = SJF[all-i];
        i++;
    }
    i = 0;
    start = theTime;
    printf("Using FIFO\n");
    while(i <= (all-1)){
        if ((theTime - FIFO[i].arrival) < start) {
            theTime = theTime + FIFO[i].arrival;
        }
        printf("Job #%i Start at: %s",FIFO[i].jid, asctime(timeinfo));
        theTime = theTime + FIFO[i].duration;
        timeinfo = localtime(&theTime);
        printf("Job #%i End at: %s", FIFO[i].jid, asctime(timeinfo));
        long roller = difftime((theTime - FIFO[i].duration), start + FIFO[i].arrival);
        long dif = difftime(theTime, start);
        printf("Response Time: %li seconds\nTotal elapsed time: %li seconds\n\n", roller, dif);
        i++;
    }
    averager = difftime(theTime, start)/all;
    printf("Average time per job: %li seconds\nEnd FIFO\n\n", averager);
    start = theTime;
    i = 0;
    printf("Using SJF\n");
    while(i <= (all-1)){
        if ((theTime - SJF[i].arrival) < start) {
            theTime = theTime + SJF[i].arrival;
        }
        printf("Job #%i Start at: %s",SJF[i].jid, asctime(timeinfo));
        theTime = theTime + SJF[i].duration;
        timeinfo = localtime(&theTime);
        printf("Job #%i End at: %s", SJF[i].jid, asctime(timeinfo));
        long roller = difftime((theTime - SJF[i].duration), start + SJF[i].arrival);
        long dif = difftime(theTime, start);
        printf("Response Time: %li seconds\nTotal elapsed time: %li seconds\n\n", roller, dif);
        i++;
    }
    averager = difftime(theTime, start)/all;
    printf("Average time per job: %li seconds\nEnd SJF\n\n", averager);
    start = theTime;
    i = 0;
    printf("Using BJF\n");
    while(i <= (all-1)){
        if ((theTime - BJF[i].arrival) < start) {
            theTime = theTime + BJF[i].arrival;
        }
        printf("Job #%i Start at: %s",BJF[i].jid, asctime(timeinfo));
        theTime = theTime + BJF[i].duration;
        timeinfo = localtime(&theTime);
        printf("Job #%i End at: %s", BJF[i].jid, asctime(timeinfo));
        long roller = difftime((theTime - BJF[i].duration), start + BJF[i].arrival);
        long dif = difftime(theTime, start);
        printf("Response Time: %li seconds\nTotal elapsed time: %li seconds\n\n", roller, dif);
        i++;
    }
    averager = difftime(theTime, start)/all;
    printf("Average time per job: %li seconds\nEnd BJF\n\n", averager);
        start = theTime;
    i = 0;
    printf("Using STCF\n");
    while(i <= (all-1)){
        if ((theTime - STCF[i].arrival) < start) {
            theTime = theTime + STCF[i].arrival;
        }
        printf("Job #%i Start at: %s",STCF[i].jid, asctime(timeinfo));
        theTime = theTime + STCF[i].duration;
        timeinfo = localtime(&theTime);
        printf("Job #%i End at: %s", STCF[i].jid, asctime(timeinfo));
        long roller = difftime((theTime - STCF[i].duration), start + STCF[i].arrival);
        long dif = difftime(theTime, start);
        printf("Response Time: %li seconds\nTotal elapsed time: %li seconds\n\n", roller, dif);
        i++;
    }
    averager = difftime(theTime, start)/all;
    printf("Average time per job: %li seconds\nEnd STCF\n\n", averager);

    long slice = 2, brok[all], k = 0;
    i = 0;
    start = theTime;
    printf("Using RR\n");
    while(i <= (all-1)){
        if ((theTime - RR[i].arrival) < start) {
            theTime = theTime + RR[i].arrival;
        }
        printf("Job #%i Start at: %s",RR[i].jid, asctime(timeinfo));
        brok[i] = ceil(RR[i].duration/slice);
        theTime = theTime + brok[i];
        k = k + brok[i];
        timeinfo = localtime(&theTime);
        printf("Job #%i End at: %s", RR[i].jid, asctime(timeinfo));
        long roller = difftime((theTime - RR[i].duration), start + RR[i].arrival);
        long dif = difftime(theTime, start);
        printf("Response Time: %li seconds\nTotal elapsed time: %li seconds\n\n", roller, dif);
        i++;
    }
    averager = difftime(theTime, start)/all;
    printf("Average time per job: %li seconds\n", averager);
    printf("Total Time Slices of Slice size %li: %li Slices\nEnd RR\n\n",slice, k);
    return 0;
}
