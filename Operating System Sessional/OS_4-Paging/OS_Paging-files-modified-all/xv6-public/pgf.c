#include "types.h"
#include "stat.h"
#include "user.h"

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

    int cnt = 0;
    char in[20];
    while (1) {
        printf(1, "Enter input: (ex -> Exit, a -> access 1st sbrk, f->fork() ,anything else -> sbrk): ");
        gets(in, sizeof (in));
        in[strlen(in) - 1] = '\0'; //eliminate the last '\n'
        if (strcmp(in, "ex") == 0) {
            printf(1, "\nEXITING\n");
            exit();
        } else if (strcmp(in, "a") == 0) {
            printf(1, "Accessing *pointer[0] ... = %d\n ", *ptr[0]);
        } else if(strcmp(in, "f") == 0){
            printf(1, "Forking pid now = %d\n", getpid());
            if(fork() == 0){
                printf(1, "Inside child process \n");
            }
            else{
                printf(1, "Inside Main process\n");
            }
        } 
        else {
            cnt++;
            printf(1, "Calling sbrk _4kB for the %d th time\n\n", cnt);
            sbrk(_4KB);
            //            printf(1, "\nReturned addr from sbrk = %d\n\n\n", returnedAddr);
        }
    }

    exit();
}