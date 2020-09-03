#include "types.h"
#include "stat.h"
#include "user.h"

#define CHOICE 1
#define TEST 2

int main() {
    //Dealloc test
    uint _4KB = 4096;
    char *ptr[50];
    int counter = 0;

    char *myVal = (char *) 7777;

//    for (int i = 0; i < 2; i++) {
//        sbrk(_4KB);
//        ptr[counter++] = sbrk(_4KB); //Call SIX times to have pages in swap file
//        ptr[counter] = myVal;
//        myVal++;
//    }
    //    printf(1, "After 6 sbrks in user program... call negative sbrk twice\n");

    //    sbrk(-_4KB);
    //    sbrk(-_4KB);
    //    printf(1, "\n\nProcess calling exit() in user program\n\n");
    //    exit();


    char in[20];
    int cnt = 0, cnt2 = 0;
    int ptr_access = 0;
    printf(1, "USER PROGRAM\n\n");
    while (1) {
        printf(1, "Enter  for sbrk , d for dealloc, ex for exit, a for pointer[0]\n");
        gets(in, sizeof (in));
        in[strlen(in) - 1] = '\0'; //eliminate the last '\n'
        if (strcmp(in, "ex") == 0) {
            printf(1, "\nEXITING\n");
            exit();
        } else if (strcmp(in, "a") == 0) {
            printf(1, "Accessing *pointer[%d] ... = %d\n ", ptr_access ,*ptr[ptr_access]);
            ptr_access++;
        } else if (strcmp(in, "d") == 0) {
            cnt2++;
            printf(1, "Calling negative sbrk for the %d th time\n", cnt2);
//            sbrk(-_4KB * 5);
            sbrk(-_4KB);
        } else {
            cnt++;
            printf(1, "Calling sbrk _4kB ( X 4) for the %d th time\n\n", cnt);
//            ptr[counter++] = sbrk(_4KB * 4);
            ptr[counter++] = sbrk(_4KB);
            ptr[counter] = myVal;
            myVal++;
            //            printf(1, "\nReturned addr from sbrk = %d\n\n\n", returnedAddr);
        }
    }

}

/*
int main() {
    uint _4KB = 4096;
    char *ptr[50];
    int counter = 0;

    char *myVal = (char *) 7777;

    for (int i = 0; i < 4; i++) {
        ptr[counter++] = sbrk(_4KB); //Call four times
        ptr[counter] = myVal;
        myVal++;
    }

    printf(1, "Inside pgf.c user program(), called sbrk 4 times initially. \n");
    printf(1, "forking\n");
    int x = fork();
    if (x < 0) {
        printf(1, "fork error\n");
        //exit();
    } else if (x == 0) {
        //Child process
        printf(1, "Child proc pid = %d ... accessing *ptr[0] (No pg fault) = %x\n", getpid(), *ptr[0]);
#if TEST >= 2
        printf(1, "Child proc pid = %d .. calling sbrk once more\n", getpid());
        sbrk(_4KB);
        printf(1, "NOW Child proc pid = %d ... accessing *ptr[0] (SHOULD have pg fault) = %x\n", getpid(), *ptr[0]);
        printf(1, "Child exiting pid = %d\n", getpid());
#endif
//        exit();
    } else {
        //Main process
        wait();

        printf(1, "Main doin nothing\n");
        //        printf(1, "Main proc pid = %d ... accessing *ptr[0] (No pg fault) = %x\n", getpid(), *ptr[0]);

        //#if TEST >= 2
        //        printf(1, "Main proc pid = %d .. calling sbrk once more\n", getpid());
        //        sbrk(_4KB);
        //        printf(1, "MAIN proc pid = %d ... accessing *ptr[0] (SHOULD have pg fault) = %x\n", getpid(), *ptr[0]);
        //#endif


    }


    exit();

}
 */