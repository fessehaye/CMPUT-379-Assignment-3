#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

struct process {
    int* resources;
    int arrivalTime;
    int executionTime;
};

int* getNumArray(int length);

int main(int argc, char* args[]) {
    char inputResourceCount[BUFSIZ];
    char inputResourceNames[BUFSIZ];
    char inputResourceInstances[BUFSIZ];
    char inputProcessCount[BUFSIZ];
    
    int resourceCount;
    int* resourceInstances;
    int processCount;

    int resourceInstancesCount = 0;

    struct process* processes;

    printf("Number of different resource types: ");
    scanf("%[^\n]s", inputResourceCount);
    resourceCount = atoi(inputResourceCount);
    getchar();

    printf("Names of each resource type: ");
    scanf("%[^\n]s", inputResourceNames);
    getchar();

    printf("Number of instances of each resource type: ");
    scanf("%[^\n]s", inputResourceInstances);
    resourceInstances = getNumArray(resourceCount);

    getchar();
    printf("Number of processes: ");
    scanf("%[^\n]s", inputProcessCount);
    processCount = atoi(inputProcessCount);

    printf("\nrc: %s\nrn: %s\nri: %s\npc: %s\n", inputResourceCount, inputResourceNames, inputResourceInstances, inputProcessCount);




    free(resourceInstances);
}

int* getNumArray(int length) {
    int* array = malloc(length * sizeof(int));
    for(int i = 0; i < length; ++i) {
        scanf("%d", array+i);
    }
    return array;
}