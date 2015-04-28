#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>


static bool keepRunning = true;
int resource_types;
int *num_instances;
int *waiting_states;
int num_processes;
int **processes_detail;
int **current_allocation;
int **current_request;
int *avail_resources;

void intHandler();
void read_input();
void snapshot();
void resource_request(int process);
void resource_release(int process);
bool enough(int process);
bool request_check(int process);

/*
*	Signal used to complete simulation
*/
void intHandler() {
    keepRunning = false;
}

/*
*	Reads input for the beginning of the simulation and allocates memory based off those results.
*/
void read_input(){

	printf("Number of different resource types:");
	scanf("%d", &resource_types);

	num_instances = malloc(resource_types * sizeof(int));
	avail_resources = malloc(resource_types * sizeof(int));

	printf("Number of instances of each resource type:");
	for(int i=0; i < resource_types; i++){
		scanf("%d", &num_instances[i]);
	}

	memcpy(avail_resources, num_instances, resource_types * sizeof(int));

	printf("Number of processes:");
	scanf("%d", &num_processes);

	//sets all waiting states to zero.
	waiting_states = calloc(num_processes,num_processes * sizeof(int));

	processes_detail = (int **)malloc(num_processes * sizeof(int *));
	for (int i = 0; i < num_processes; ++i)
      processes_detail[i] = (int *)malloc(resource_types * sizeof(int));

  	//current allocation is initiated to zero
     current_allocation = (int **)calloc(num_processes,num_processes * sizeof(int *));
	for (int i = 0; i < num_processes; ++i)
      current_allocation[i] = (int *)calloc(num_processes,resource_types * sizeof(int));

  	//current request is initiated to zero
     current_request = (int **)calloc(num_processes,num_processes * sizeof(int *));
	for (int i = 0; i < num_processes; ++i)
      current_request[i] = (int *)calloc(num_processes,resource_types * sizeof(int));
  
	
	for(int i = 0; i < num_processes; i++){
		printf(" Details of P%d:",i+1);
		for(int j=0; j < resource_types; j++){
			scanf("%d", &processes_detail[i][j]);
		}
	}


}
/*
* Prints snapshot of formatted tables.
*/
void snapshot(){
	char a[30];
	char b[30];
	char c[30];
	char M[30];
	char avail[30];
	char temp[30] = "";
	char value[30] = "";

	//Header for Table
	printf("\n\t%s\t\t\t%s\t\t\t%s\t\t\t%s\t\t\t%s\t\t\t\n", "CA","CR","CAR","MPR","MAR");
	printf("----------------------------------------------------------------------------------------------------------------------\n");
 	
 	//Print first row (P1)
	strcpy(temp, "");
	for (int i = 0; i<resource_types; i++){
		sprintf(value, "%d ",current_allocation[0][i]);
		strcat(temp,value);
	}
	sprintf(a, "%s",temp);

	strcpy(temp, "");
	for (int i = 0; i<resource_types; i++){
		sprintf(value, "%d ",processes_detail[0][i]);
		strcat(temp,value);
	}
	sprintf(c, "%s",temp);

	strcpy(temp, "");
	for (int i = 0; i<resource_types; i++){
		sprintf(value, "%d ",current_request[0][i]);
		strcat(temp,value);
	}
	sprintf(b, "%s",temp);

	strcpy(temp, "");
	for (int i = 0; i<resource_types; i++){
		sprintf(value, "%d ",num_instances[i]);
		strcat(temp,value);
	}
	sprintf(M, "%s",temp);

	strcpy(temp, "");
	for (int i = 0; i<resource_types; i++){
		sprintf(value, "%d ",avail_resources[i]);
		strcat(temp,value);
	}
	sprintf(avail, "%s",temp);

	printf("P1	%s\t\t\t%s\t\t\t%s\t\t\t%s\t\t\t%s\t\t\t\n",a,b,avail,c,M);
	
	//print rest of processes
	for(int i = 1;i < num_processes;i++){

		strcpy(temp, "");
		for (int j = 0; j<resource_types; j++){
			sprintf(value, "%d ",current_allocation[i][j]);
			strcat(temp,value);
		}
		sprintf(a, "%s",temp);
		
		strcpy(temp, "");
		for (int j = 0; j<resource_types; j++){
			sprintf(value, "%d ",processes_detail[i][j]);
			strcat(temp,value);
		}
		sprintf(c, "%s",temp);

		strcpy(temp, "");
		for (int j = 0; j<resource_types; j++){
			sprintf(value, "%d ",current_request[i][j]);
			strcat(temp,value);
		}
		sprintf(b, "%s",temp);
		
		printf("P%d	%s\t\t\t%s\t\t\t\t\t\t%s\t\t\t\t\t\t\n",i+1,a,b,c);
	}
}

/*
* Called upon when running process wishes to make a request
*/
void resource_request(int process){
	char temp[30] = "";
	char value[30] = "";

	for (int j = 0; j<resource_types; j++){
			if(processes_detail[process][j] == 0){
				current_request[process][j] = 0;
			}
			else{
				current_request[process][j] = ((rand()) %  processes_detail[process][j]);
			}
			
		}

	for (int j = 0; j<resource_types; j++){
			sprintf(value, "%d,",current_request[process][j]);
			strcat(temp,value);
		}
	temp[strlen(temp)-1] = '\0';

	printf("Request (%s) came from P%d\n",temp,process+1);
	//Do we have enough resources?
	if(enough(process) == true){
		printf(" Request (%s) from P%d has been granted.\n",temp,process+1);
		
		for (int j = 0; j<resource_types; j++){
			avail_resources[j] -=  current_request[process][j];
			current_allocation[process][j] += current_request[process][j];
			current_request[process][j] = 0;
		}

		snapshot();
	}
	// Not enough resources. Process in wait state.
	else{
		printf("Request (%s) from P%d cannot be satisfied, P%d is in waiting state.\n",temp,process+1,process+1);
		waiting_states[process] = 1;
		snapshot();
	}
}

/*
* Called upon when running process wishes to make a release
*/
void resource_release(int process){
	char temp[30] = "";
	char value[30] = "";
	int release[resource_types];

	for (int j = 0; j<resource_types; j++){
			if (current_allocation[process][j] == 0)
			{
				release[j] = 0;
			}
			else{
				release[j] = ((rand()) %  current_allocation[process][j] ) ;
			}
			sprintf(value, "%d,",release[j]);
			strcat(temp,value);
		}

	temp[strlen(temp)-1] = '\0';

	//Release Resources to available resources
	printf("P%d has released (%s) resources\n",process+1,temp);

	for (int j = 0; j<resource_types; j++){
			avail_resources[j] +=  release[j];
			current_allocation[process][j] -= release[j];
			release[j] = 0;
		}

	//Checks if waiting process can proceed now
	for (int i = 0; i< num_processes; i++){
			if(waiting_states[i] == 1 && enough(i) == true){
				strcpy(temp, "");

				for (int j = 0; j<resource_types; j++){
					sprintf(value, "%d,",current_request[i][j]);
					strcat(temp,value);
				}

				printf("Previous request of (%s) from P%d has been satisfied\n",temp,i+1);

				waiting_states[i] = 0;
				
				for (int j = 0; j<resource_types; j++){
					avail_resources[j] -=  current_request[i][j];
					current_allocation[i][j] += current_request[i][j];
					current_request[i][j] = 0;
				}

			}
		}
		snapshot();
}
/*
* Helper function to check if resources are available to proceed with request
*/
bool enough(int process){
	bool enough = true;
	int temp;

	for (int j = 0; j<resource_types; j++){
			temp = avail_resources[j] - current_request[process][j];
			//cant be negative
			if(temp<0){
				enough = false;
			}
	}

	return enough;
}

// Checks if process already has a request 
bool request_check(int process){
	bool allowed = true;
	int temp;

	for (int j = 0; j<resource_types; j++){
			temp = current_request[process][j];
			if(temp>0){
				allowed = false;
			}
	}

	return allowed;
}

int main(void){
	

    time_t start = time(0);
    int seconds_since_start = 0;
    int old_time = 0;
    int *process_choices;
    
    read_input();
    signal(SIGINT, intHandler);


    process_choices = malloc(num_processes * sizeof(int));
    
    snapshot();

    while(keepRunning){
    	seconds_since_start = (int)difftime( time(0), start);
    	if(old_time!=seconds_since_start){
    		
    		old_time = seconds_since_start;

    		//every five seconds
    		if(seconds_since_start % 5 == 0){
    			
    			//randomly choose between 3 decisons
    			for(int i = 0;i < num_processes;i++){
    				process_choices[i] = ((rand()) % 3) ;
    			}

    			for(int i = 0;i < num_processes;i++){
    				switch(process_choices[i]){
    					case 2:
    						if(request_check(i)){
    							resource_request(i);
    						}
    						break;
    					case 1:
    						if(request_check(i)){
    							resource_release(i);
    						}
    						break;
    				}
    			}
    		}
    	}
   
    }

    printf("\n%s\n", "Simulation has been ended.");
	free(num_instances);
	free(avail_resources);


	for (int i = 0; i < num_processes; ++i)
      	free(processes_detail[i]);

    free(processes_detail); 

    for (int i = 0; i < num_processes; ++i)
      	free(current_allocation[i]);

    free(current_allocation); 

    for (int i = 0; i < num_processes; ++i)
      	free(current_request[i]);

    free(current_request);

	return 0;
}

