#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/shm.h>
#include "semutil_2.h"
#include "shm_com.h"
#include <bits/stdc++.h>


#define MAX_SIZE 100

void process_inserter();
void process_sorter();
void process_getter();

int shared_arr[MAX_SIZE];

static int sem_id;

static int keySempahore;
static int keyMemory;

int main() {
    pid_t parent = getpid();

    pid_t id = fork();

    //Create Semaphore ....
    int keySempahore = 1234;
    sem_id = semget((key_t) keySempahore, 1, 0666 | IPC_CREAT);

    //Set Semaphore initial value
    if (!set_semvalue(sem_id)) {
        printf("Error in setting semaphore value in process id = "
                "%d\n", (int) getpid());
        exit(-1);
    }



    if (id > 0) {
        //This is parent process ...... 
        pid_t id2 = fork();

        if (id2 != 0) {
            //Parent process now 
            pid_t id3 = fork();
            if (id3 == 0) {
                process_getter();
            }
        } else if (id2 == 0) {
            //Child process
            process_sorter();
        } else if (id2 < 0) {
            printf("FORK FAILED 2nd fork");
            exit(-1);

        }
    } else if (id == 0) {
        //This is child 1 process...
        process_inserter();
    } else if (id < 0) {
        printf("FORK FAILED first fork");
        exit(-1);
    }

    while (1);
    printf("RETURNING MAIN...\n");
    return 0;
}

void process_inserter() {

    printf("\nInside inserter... pid = %d\n", (int) getpid());


    void *shared_memory = (void*) 0;

    int shmid = shmget((key_t) 5555, sizeof (shared_memory), 0666 | IPC_CREAT);
    if (shmid < 0) {
        printf("ERROR in shmget of child pid = %d\n", (int) getpid());
        exit(-1);
    }

    shared_memory = shmat(shmid, NULL, 0);
    printf("Initialize Inserter pointer ... %p\n", shared_memory);
    if (shared_memory == (void*) - 1) {
        printf("ERROR in shm of child pid = %d\n", (int) getpid());
        exit(-1);
    }
    shared_memory_use *pointer = (shared_memory_use *) shared_memory;

    printf("INSERTER ... pointer is .. %p\n", pointer);

    //INITIALIZATION IS DONE .....................

    //GET INFO FROM USER ... CRITICAL REGION ...
    //Wait Semaphore
    if (!semaphore_p(sem_id)) {
        printf("Problem in wait semaphore in Inserter \n");
        exit(-1);
    }

    printf("\n==============================ACQUIRING SEMAPHORE Inserter==================================\n");



    //fgets(buffer, BUFSIZ, stdin);
    //get the input from console ...

    printf("ENTER numbers in Inserter .... n followed by arr \n");
    int n;
    scanf(" %d", &n);
    int i;
    pointer->numElements = n;
    for (i = 0; i < n; i++) {
        scanf(" %d", &pointer->arr[i]);
    }

    printf("AFTER SCANNING IN INSERTER .... printing buffer int ..\n");
    for (i = 0; i < n; i++) {
        printf("%d , ", pointer->arr[i]);
    }
    printf("\n");

    char msg[250] = "Sort";
    strncpy(pointer->some_text, msg, 250);

    printf("\n==============================REALEASIING SEMAPHORE Inserter==================================\n");

    pointer->firstDone = 1;
    pointer->turnOfNow = 2;
    if (!semaphore_v(sem_id)) {
        printf("Problem in release semaphore in Inserter\n");
        exit(-1);
    }


    int run = 1;
    while (run) {
        while (pointer->turnOfNow != 1); //wait

        if (!semaphore_p(sem_id)) {
            printf("Problem in wait semaphore in Inserter \n");
            exit(-1);
        }

        printf("\n==============================ACQUIRING SEMAPHORE Inserter==================================\n");

        if (strncmp(pointer->some_text, "Sorted", 5) == 0) {
            //take input again ...
            printf("ENTER input again n followed by array...\n");
            scanf(" %d", &n);
            for (i = 0; i < n; i++) {
                scanf(" %d", &pointer->arr[pointer->numElements + i]);
            }
            pointer->numElements += n;
            printf("\nArray input done.. printing array..\n ");

            for (i = 0; i < n; i++) {
                printf("%d , ", pointer->arr[i]);
            }
            printf("\n");
        } else if (strncmp(pointer->some_text, "Terminate", 9) == 0) {
            //terminate..
            run = 0;

        }

        printf("\n==============================Releasing SEMAPHORE Inserter==================================\n");


        if (!semaphore_v(sem_id)) {
            printf("Problem in release semaphore in Inserter\n");
            exit(-1);
        }


    }

}

void process_sorter() {
    printf("\nInside sorter... pid = %d\n", (int) getpid());
    void *shared_memory = (void*) 0;

    int shmid = shmget((key_t) 5555, sizeof (shared_memory), 0666 | IPC_CREAT);
    if (shmid < 0) {
        printf("ERROR in shmget of child pid = %d\n", (int) getpid());
        exit(-1);
    }

    shared_memory = shmat(shmid, NULL, 0);
    printf(">> Initialize Sorter .. pointer ... %p\n", shared_memory);
    if (shared_memory == (void*) - 1) {
        printf("ERROR in shm of child pid = %d\n", (int) getpid());
        exit(-1);
    }

    shared_memory_use *pointer2 = (shared_memory_use *) shared_memory;

    printf("SORTER ... pointer2 is .. %p\n", pointer2);


    if (!semaphore_p(sem_id)) {
        printf("Problem in wait semaphore in Inserter \n");
        exit(-1);
    }

    printf("\n==============================ACQUIRING SEMAPHORE Sorter==================================\n");


    if (pointer2->firstDone != 1) {
        pointer2->turnOfNow = 1;
    }

    printf("\n==============================REALEASIING SEMAPHORE Sorrter==================================\n");

    if (!semaphore_v(sem_id)) {
        printf("Problem in release semaphore in Sorter\n");
        exit(-1);
    }

    //INITIALIZATION IS DONE .....................


    int running = 1;
    while (running == 1) {
        while (pointer2->firstDone != 1) {
            //wait ... 
        }
        while (pointer2->turnOfNow != 2);

        //Acquire lock ...
        if (!semaphore_p(sem_id)) {
            printf("Problem in wait semaphore in Inserter \n");
            exit(-1);
        }

        printf("\n==============================ACQUIRING SEMAPHORE Sorter==================================\n");

        printf("Shared message is %s\n", pointer2->some_text);

        int i, j;

        printf("Array is \n");
        for (i = 0; i < pointer2->numElements; i++) {
            printf("%d, ", pointer2->arr[i]);
        }

        printf("\n Sorting in Sorter ...\n");
        int tot = pointer2->numElements;
        int sorted = 0;
        for (i = 1; i < tot; i++) {
            for (j = 0; j < i; j++) {
                if (pointer2->arr[i] < pointer2->arr[j]) {
                    sorted = 1;
                    int temp = pointer2->arr[i];
                    pointer2->arr[i] = pointer2->arr[j];
                    pointer2->arr[j] = temp;
                }
            }
        }
        printf("After sort, Array is \n");
        for (i = 0; i < pointer2->numElements; i++) {
            printf("%d, ", pointer2->arr[i]);
        }
        printf("\n");
        if (sorted == 0) {
            //already sorterd .
            char msg1[250] = "Terminate";
            strncpy(pointer2->some_text, msg1, 250);
            running = 0; //Terminates itself
        } else {
            char msg2[250] = "Sorted";
            strncpy(pointer2->some_text, msg2, 250);

        }

        pointer2->turnOfNow = 1;

        printf("\n==============================REALEASIING SEMAPHORE Sorrter==================================\n");

        if (!semaphore_v(sem_id)) {
            printf("Problem in release semaphore in Sorter\n");
            exit(-1);
        }
    }



}

void process_getter() {
    printf("\nInside getter... pid = %d\n", (int) getpid());
    void *shared_memory = (void*) 0;

    int shmid = shmget((key_t) 5555, sizeof (shared_memory), 0666 | IPC_CREAT);
    if (shmid < 0) {
        printf("ERROR in shmget of child pid = %d\n", (int) getpid());
        exit(-1);
    }

    shared_memory = shmat(shmid, NULL, 0);
    printf(">> Initialize Getter .. pointer ... %p\n", shared_memory);
    if (shared_memory == (void*) - 1) {
        printf("ERROR in shm of child pid = %d\n", (int) getpid());
        exit(-1);
    }

    shared_memory_use *pointer3 = (shared_memory_use *) shared_memory;

    int run = 1;
    while (run) {
        if (!semaphore_p(sem_id)) {
            printf("Problem in wait semaphore in Inserter \n");
            exit(-1);
        }
        printf("In Getter, first item is %d\n", pointer3->arr[0]);

        if (!semaphore_v(sem_id)) {
            printf("Problem in release semaphore in Sorter\n");
            exit(-1);
        }
        sleep(2);
    }
}