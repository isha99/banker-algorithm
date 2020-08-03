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

int *safe;
int finished_processes = 0;

pthread_mutex_t lockResources;
pthread_cond_t condition;

void *run(int customer_num);
int request(int customer_num, int request[]);
int release_resources(int customer_num, int request[]);
void print_states(int M[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], char *name);
bool safe_sequence();

int main(int argc, char *argv[])
{
        FILE *fp;
        char ch;
        char user_command[2];
        char input[100];

        printf("\nNumber of Customers: %d", NUMBER_OF_CUSTOMERS);

        printf("\nCurrently Available Resources:");
        for (int i = 1; i < argc; i++)
        {
                printf(" %s", argv[i]);
        }

        resources = (int *)malloc(NUMBER_OF_RESOURCES * sizeof(*resources));
        printf("\nMaximum Resources From File:\n");
        fp = fopen("sample4_in.txt", "r");
        if (fp == NULL)
        {
                printf("File not found. \n");
        }
        else
        {
                while ((ch = fgetc(fp)) != EOF)
                {
                        printf("%c", ch);
                }
        }
        fclose(fp);

        do{
                printf("\nEnter Command: ");
                fgets(input, 100, stdin);
                char *thread = strtok(input, " ");
                strcpy(user_command, thread);
                
                if (strstr(user_command, "RUN") != NULL)
                {       run(customer_num);
                        printf("Entering Run Program Function");
                }
                else if (strstr(user_command, "*")!= NULL)
                {
                        print_state();
                        printf("Entering Print State Function");
                }
                else if (strstr(user_command, "RQ") != NULL)
                {
                        request(customer_num, request[]);
                        printf("Entering Request Resource Function");
                }
                else if (strstr(user_command, "RL")!= NULL)
                {
                        release(customer_num, request[]);
                        printf("Entering Release Resource Function");
                }
                else{
                        printf("error: not command found");
                }
        } while (1);
        return 0;
}

void *run(int customer_num) {
        int p = *((int *)customer_num);
        pthread_mutex_lock(&lockResources);
        while (p != safe[finished_processes])
                pthread_cond_wait(&condition, &lockResources);

        // process
        printf("\n--> Custmomer/Thread %d", p + 1);
        printf("\n\tAllocated resources: ");
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                printf("%3d", allocated[p][i]);

        printf("\n\tNeeded : ");
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                printf("%3d", need[p][i]);

        printf("\n\tAvailable : ");
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                printf("%3d", resources[i]);

        printf("\n");
        sleep(1);

        printf("\tResource Allocated!");
        printf("\n");
        sleep(1);
        printf("\tProcess Code Running...");
        printf("\n");
        sleep(rand() % 3 + 2); // process code
        printf("\tProcess Code Completed...");
        printf("\n");
        sleep(1);
        printf("\tProcess Releasing Resource...");
        printf("\n");
        sleep(1);
        printf("\tResource Released!");

        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                resources[i] += allocated[p][i];

        printf("\n\tNow Available : ");
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                printf("%3d", resources[i]);
        printf("\n\n");

        sleep(1);

        // condition broadcast
        finished_processes++;
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&lockResources);
        pthread_exit(NULL);
        free(safe);
}

//int request(int customer_num, int request[]){
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
//}

int release(int customer_num, int request[])
{
        printf("P%d releases all the resources\n", customer_num + 1);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
                resources[j] = resources[j] + allocated[customer_num][j];
                allocated[customer_num][j] = 0;
        }
        return 0;
}

//void print_states(){

//}

bool safe_sequence()
{
        safe = (int *)malloc(NUMBER_OF_CUSTOMERS * sizeof(*safe));
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
                safe[i] = -1;

        // get safe sequence
        int tempRes[NUMBER_OF_RESOURCES];
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                tempRes[i] = resources[i];

        bool finished[NUMBER_OF_CUSTOMERS];
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
                finished[i] = false;
        int nfinished = 0;
        while (nfinished < NUMBER_OF_CUSTOMERS)
        {
                bool is_safe = false;
                for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
                {
                        if (!finished[i])
                        {
                                bool possible = true;
                                for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                                        if (need[i][j] > tempRes[j])
                                        {
                                                possible = false;
                                                break;
                                        }
                                if (possible)
                                {
                                        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                                                tempRes[j] += allocated[i][j];
                                        safe[nfinished] = i;
                                        finished[i] = true;
                                        ++nfinished;
                                        is_safe = true;
                                }
                        }
                }
                if (!is_safe)
                {
                        for (int k = 0; k < NUMBER_OF_CUSTOMERS; k++)
                                safe[k] = -1;
                        return false; // no safe sequence found
                }
        }
        return true; // safe sequence found
}
