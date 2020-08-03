//library imports
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

//defining constants & variables
#define RESOURCES 4
#define CUSTOMERS 5
//the available amount of each resource
int *resources;
// the amount currently allocated to each customer
int **allocated;
//the maximum demand of each customer
int **maximum;
//the remaining need of each customer
int **need;
//safe sequence
int *safe;
//number of process already ran
int finished_processes = 0;
int finish[CUSTOMERS]={0};


// available amount of each resource in banker algo
int banker_available[RESOURCES];
// maximum demand of each customer in banker algo
int banker_maximum[CUSTOMERS][RESOURCES];
//currently allocated amount to customer in banker algo
int banker_allocation[CUSTOMERS][RESOURCES];
//remaining need of each customer in banker algo
int banker_need[CUSTOMERS][RESOURCES];

//mutexes needed to operate pthreads
pthread_mutex_t mutex;
pthread_mutex_t lockResources;
pthread_cond_t condition;

//function declarations

//function the threads one by one 
void *run(int customer_num);
//function where customers can request resources
int request(int customer_num, int request[]);
//function where customers can release resources
int release(int customer_num, int request[]);
//function where it will print all the arrays
void print_states(int M[CUSTOMERS][RESOURCES], char *name);

//main function where program will start
int main(int argc, char *argv[]){
        //initializing variables for file input
        FILE *fp;
        char ch;
        //initializing variables for user input
        char user_command[2];
        char input[50];

        //printing the number of customer in current program
        printf("\nNumber of Customers: %d", CUSTOMERS);

        //printing available resources from the command line arguments given
        printf("\nCurrently Available Resources:");
        for (int i = 1; i < argc; i++) {
                printf(" %s", argv[i]);
        }

        //assigning the file input to the available array 
        resources = (int *)malloc(RESOURCES * sizeof(*resources));
       
        //printing the file input
        printf("\nMaximum Resources From File:\n");
        fp = fopen("sample4_in.txt", "r");
        //check if file is in directory
        if (fp == NULL){
                printf("File not found. \n");
        }else {
                while ((ch = fgetc(fp)) != EOF){
                        //print the characters 
                        printf("%c", ch);
                }
        }
        fclose(fp);

        //ask for user input
        printf("\nEnter Command: ");
        
        //prepare to read user input
        fgets(input, 100, stdin);
        char *thread = strtok(input, " ");
        strcpy(user_command, thread);

        // depending on user input, run functions respectively
        if (strstr(user_command, "RUN") != NULL) {
                //run(customer_num);
                printf("Entering Run Program Function");
        }
        else if (strstr(user_command, "*") != NULL) {
               // banker();
                printf("Entering Print State Function");
        }else if (strstr(user_command, "RQ") != NULL){
                //request(customer_num, request[]);
                printf("Entering Request Resource Function");
        }else if (strstr(user_command, "RL") != NULL){
               // release(customer_num, request[]);
                printf("Entering Release Resource Function");
        }else{
                printf("No Command was Found, Try Again.\n");
        }return 0;
}
//function that will run through the customer/threads
void *run(int customer_num) {

        int p = *((int *)customer_num);

        //lock the resources
        pthread_mutex_lock(&lockResources);

        //condition where if p is not safe to continue
        while (p != safe[finished_processes])
                //put wait condition
                pthread_cond_wait(&condition, &lockResources);

        //continue to go through threads
        printf("\n--> Custmomer/Thread %d", p + 1);

        //list allocated resources from allocated array
        printf("\n\tAllocated resources: ");
        for (int i = 0; i < RESOURCES; i++)
                printf("%3d", allocated[p][i]);

        // list the resources needed from need array
        printf("\n\tNeeded : ");
        for (int i = 0; i < RESOURCES; i++)
                printf("%3d", need[p][i]);

        // list the available resources from available array
        printf("\n\tAvailable : ");
        for (int i = 0; i < RESOURCES; i++)
                printf("%3d", resources[i]);

        printf("\n");
        sleep(1);

        //Resources will be allocated and begin to Run
        printf("\tResource Allocated!");
        printf("\n");
        sleep(1);
        printf("\tProcess Code Running...");
        printf("\n");
        //this is the process code
        sleep(rand() % 3 + 2);

        //once completed, the thread will release the resources
        printf("\tThread has finished");
        printf("\n");
        sleep(1);
        release(customer_num, request);
        printf("\tThread is releasing resources");
        printf("\n");
        sleep(1);
        for (int i = 0; i < RESOURCES; i++)
                resources[i] += allocated[p][i];

        //List the newly available resources
        printf("\n\tNew Available : ");
        for (int i = 0; i < RESOURCES; i++)
                printf("%3d", resources[i]);
        printf("\n\n");

        sleep(1);

        //the count for finished process increase, unlocks resources, thread will exit and free the safe seqeunce
        finished_processes++;
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&lockResources);
        pthread_exit(NULL);
        free(safe);
}

//this function is for when customers request resources
int request(int customer_num, int request[]){
    //initilize return value
    int val =-1;

    //lock resources
    pthread_mutex_lock(&lockResources);
    
    //this will identify which customer has requested the resources
    printf("\nP%d requests for ",customer_num+1);
    for(int i=0;i<RESOURCES;i++){
        printf("%d ",request[i]);
    }
    printf("\n");
        for(int i=0;i<RESOURCES;i++){  
            if(request[i]>resources[i]){
                //customer waits for the resources 
                printf("P%d is waiting for the reaources...\n",customer_num+1);
                //resources are unlocked
                pthread_mutex_unlock(&lockResources);
                return -1;
            } 
        }
        //return value is linked to the banker algorithm 
    val = banker(customer_num,request);
    
    //if the return value is 0
    if(val ==0){
        //initialize variables
        int no_need=1;
        //then safe sequence is found and print the sequence
        printf("Safe Sequence: ");
        for(int i=0;i<CUSTOMERS;i++){
            printf("P%d ",safe[i]+1 );
        }
        //Once successful, user is informed that request is satisfied
        printf("\nP%d's request has been satisfied.\n",customer_num+1);
       
        for(int j=0;j<RESOURCES;j++){
            allocated[customer_num][j]=allocated[customer_num][j]+request[j];
            resources[j]=resources[j]-request[j];
            need[customer_num][j]=need[customer_num][j]-request[j];
            if(need[customer_num][j]!=0){
                no_need=0;
            }
        }
        //if there is no more requests
        if(no_need){
                //add the customer to the finished array and release the resources
            finish[customer_num]=1;
            release(customer_num, request);
        }
        run(customer_num);
    }
    //if there is no safe sequence found
    else{
        printf("cannot find a safe sequence\n");
    }
    //unlock the resources and return value
    pthread_mutex_unlock(&lockResources);
    return val;
}

//this function is for the customer to release the resources 
int release(int customer_num, int request[]) {
        //let user know that customer is releasing resources
        printf("P%d releases all the resources\n", customer_num + 1);
        for (int j = 0; j < RESOURCES; j++) {
                resources[j] = resources[j] + allocated[customer_num][j];
                allocated[customer_num][j] = 0;
        }
        return 0;

}

//this function is to print the allocated, resources, need, and maximum array
void print_states(){

}
//this function is to get the safe sequence on the available resources
bool safe_sequence(){
        //initialize a safe array
        safe = (int *)malloc(CUSTOMERS * sizeof(*safe));
        for (int i = 0; i < CUSTOMERS; i++)
                safe[i] = -1;

        // get safe sequence
        int tempRes[RESOURCES];
        for (int i = 0; i < RESOURCES; i++)
                tempRes[i] = resources[i];

        bool finished[CUSTOMERS];
        for (int i = 0; i < CUSTOMERS; i++)
                finished[i] = false;
        int nfinished = 0;
        while (nfinished < CUSTOMERS){
                bool is_safe = false;
                for (int i = 0; i < CUSTOMERS; i++) {
                        if (!finished[i]){
                                bool possible = true;
                                for (int j = 0; j < RESOURCES; j++)
                                        if (need[i][j] > tempRes[j]){
                                                possible = false;
                                                break;
                                        }
                                if (possible){
                                        for (int j = 0; j < RESOURCES; j++)
                                                tempRes[j] += allocated[i][j];
                                        safe[nfinished] = i;
                                        finished[i] = true;
                                        ++nfinished;
                                        is_safe = true;
                                }
                        }
                }
                if (!is_safe){
                        for (int k = 0; k < CUSTOMERS; k++)
                                safe[k] = -1;
                        return false;
                }
        }
        return true;
}

//this is the banker algorithm where the safe sequence is fetched
int banker(int customer_num,int request[]){
    int finish[CUSTOMERS]={0};
    
    for(int i=0;i<RESOURCES;i++){ 
        banker_available[i]=resources[i];
        for(int j=0;j<CUSTOMERS;j++){
            banker_allocation[j][i]=allocated[j][i];
            
            banker_maximum[j][i]=maximum[j][i];
         
            banker_need[j][i]=need[j][i];
        }
    }
        for(int i=0;i<RESOURCES;i++){ 
            banker_available[i]=banker_available[i]-request[i];
            banker_allocation[customer_num][i]=banker_allocation[customer_num][i]+request[i];
            banker_need[customer_num][i]=banker_need[customer_num][i]-request[i];
        }
    int count=0;
    while(1){
        
        int I=-1;
        
        for(int i=0;i<CUSTOMERS;i++){
            int nLessThanA=1;
            for(int j=0; j<RESOURCES;j++){
                if(banker_need[i][j]>banker_available[j] || finish[i]==1){
                    nLessThanA=0;
                    break;
                }
            }
            if(nLessThanA){
                I=i;
                break;
            }
        }    
        if(I!=-1){
            safe[count]=I;
            count++;
            finish[I]=1;
            for(int k=0;k<RESOURCES;k++){
                banker_available[k]=banker_available[k]+banker_allocation[I][k];
            }
        } else{
            
            for(int i=0;i<RESOURCES;i++){
                if(finish[i]==0){
                    return -1;
                }
            }
            return 0;
        }
    }    
}
