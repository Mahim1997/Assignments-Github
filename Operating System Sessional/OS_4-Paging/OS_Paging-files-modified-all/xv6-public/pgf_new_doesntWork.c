#include "types.h"
#include "stat.h"
#include "user.h"

#define INITIAL 4

int main() {
    uint _4KB = 4096;

    char *ptr[50];

    int counter = 0;

    char *myVal = (char *) 7777;
    //    char *values[7] = {(char *)7777, (char *)7778, (char *)7779, (char *)7780, (char *)7781, (char *)7782, (char *)7783};

    for (int i = 0; i < INITIAL; i++) {
        ptr[counter] = sbrk(_4KB); //Call four times
        //        ptr[counter] = &myVal;
        //        ptr[counter] = &values[counter];
        counter++;
    }


    printf(1, "Before running user prog while loop. printing the array\n");

    //    printf(1, "ptr[0] val = %x\n", *ptr[0]);
    for (int i = 0; i < INITIAL; i++) {
        printf(1, "idx = %d -> ptr[%d] = %x\n", i, i, ptr[i]);
    }

    printf(1, "After calling sbrk 4 times initially. \n\n\n");

    int cnt = 0;
    char in[20];
    while (1) {
        printf(1, "Enter input: (ex -> Exit, a -> access 1st sbrk call thing, s->swap, anything else -> sbrk): ");
        gets(in, sizeof (in));
        in[strlen(in) - 1] = '\0'; //eliminate the last '\n'
        if (strcmp(in, "ex") == 0) {
            printf(1, "\nEXITING\n");
            exit();
        } else if (strcmp(in, "a") == 0) {
            printf(1, "\n\nUSER PROG: Accessing ptr[0] ... = %x\n ", ptr[0]);
        } else if (strcmp(in, "s") == 0) {
            printf(1, "Calling ptr[0] = something\n");
            ptr[0] = myVal;
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
