/**
* Project: Banker's algorithm
* author: Isha Raulji 
**/

#include <stdio.h>

//variables needed 
#define NUM_CUSTOMER 5
#define NUM_RESOURCE 4
int processes;
int resources;
int max_demand[][];
int allocated_amt[][];
int rem_need[][];
int available[];




//main will run the program
int main(int argc, char** argv){

}
// this is to determine whether a safe sequence is found
safe_state(){
    //initialize vectors work and finish 

    //if finish is false and need < or equal to work

        // then, work = work + allocated_amt and finish = true, then loop
       
    //or else, 
        //finish = true and system is safe state

}

//this is to determine whether request can be safely granted
resource_request(){
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


