#define TEXT_SZ 250
#define SIZE 100
struct shared_memory_use {
	int turnOfNow;     //1 for Inserter , 2 for Sorter , 3 For Getter	
	char some_text[TEXT_SZ];    //What text to write
        int arr[SIZE];  //BUFFER
        int noItems;    //number of items
        int firstDone;
        int numElements;
};
