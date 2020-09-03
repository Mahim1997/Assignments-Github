#include "types.h"
#include "stat.h"
#include "user.h"

//malloc() -> morecore() -> sbrk() 

int main()
{
    char in[50];
    int cnt = 0;
    uint _4kB = 4096; //2 kB
    sbrk(_4kB);
    sbrk(_4kB);
    sbrk(_4kB);
    while(1){
        printf(1, "\n\nEnter input(10 len, <ex> to exit, other to sbrk by 4096): ");
        gets(in, sizeof(in));
        in[strlen(in) - 1] = '\0';  //eliminate the last '\n'
//        printf(1, "You've entered input char = %c\n", in);
        if(strcmp(in, "ex") == 0){
            printf(1, "\nEXITING\n");
            exit();
        }else{
            cnt++;
            printf(1, "Calling sbrk _4kB for the %d th time\n\n", cnt);
            sbrk(_4kB); 
//            printf(1, "\nReturned addr from sbrk = %d\n\n\n", returnedAddr);
        }
    }

//    exit();
}