#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define NUMBER_OF_RESOURCES 4
#define NUMBER_OF_CUSTOMERS 5

//the available amount of each resource
int *resources;
// the amount currently allocated to each customer
int **allocated;
//the maximum demand of each customer
int **maximum;
//the remaining need of each customer
int **need;

void run(int customer_num);
int request(int customer_num, int request[]);
int release_resources(int customer_num, int request[]);
void print_states(int M[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], char *name);
bool getSafeSeq();

int main(int argc, char **argv){
        FILE *fp;
        char ch;
        char user_command[2];
        char input[100];
        fgets(input, 100, stdin);

        char *thread = strtok(input, " ");
        strcpy(user_command, thread);
        //
        printf("\nNumber of Customers: %d", NUMBER_OF_CUSTOMERS);
        //
        printf("\nCurrently Available Resources:");
        for (int i = 1; i < argc; i++){
                printf(" %s", argv[i]);
        }
        //
        resources = (int *)malloc(NUMBER_OF_RESOURCES * sizeof(*resources));
        printf("\nMaximum Resources From File:\n");

        fp = fopen("sample4_in.txt", "r");
        if (fp == NULL){
                printf("File not found. \n");
        }
        else{
                while ((ch = fgetc(fp)) != EOF){
                        printf("%c", ch);
                }
        }
        fclose(fp);
        printf("\nEnter Command:");
        if (strstr(user_command, "RUN")){
               //run(customer_num);
                printf("Entering Run Program Function");
        }
        else if (strstr(user_command, "*")){
                //print_state();
                printf("Entering Print State Function");
        }
        else if (strstr(user_command, "RQ")){
                //request(customer_num, request[]);
                printf("Entering Request Resource Function");
        }
        else if (strstr(user_command, "RL")){
                //release(customer_num, request[]);
                printf("Entering Release Resource Function");
        }       
        else{
                
        }
}

void run(int customer_num){

}

int request(int customer_num, int request[]){
    //initialize request vector for thread T

    //if request[j] == k
        //then, T wants k resourses
        //when this occurs, if request < need
            //then, if request < available
                //then, available - request & allocated_amt + request & need - request
                //else t wait and print resources not available
        //else, raise error that max exceeded

    //if state is safe, 
        //transaction completed & t has allocated resources
    //else t wait for request and old state is restored
}

int release(int customer_num, int request[]){

}

void print_states(){

}

bool safe_sequence(){

}
