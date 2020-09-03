#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	unsigned int address1;
	address1 = 0xbfffead8;

	unsigned int address2;
	address2 = 0xbfffea6c;

	unsigned int difference;
	difference = address1 - address2;

	printf("Address1 = EBP = 0x%x, Address2 = Buffer[0] = 0x%x, Difference = %d\n", address1, address2, difference);

	if(argc > 1)
	{
		printf("ADDING ... argv[1] = %s", argv[1]);
		unsigned int additionAddr = *(unsigned int*)argv[1];
		unsigned int newAddr = address1 + additionAddr;

		printf("After adding --> new address = 0x%x\n", newAddr);
	}

}