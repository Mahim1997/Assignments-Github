#include "types.h"
#include "stat.h"
#include "user.h"

#define CHOICE 1
#define TEST 2  

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
