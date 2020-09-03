#ifndef MYDEBUG_H
#define MYDEBUG_H

#define DEBUGGING_MODE 0
//----------------------------------------------------------------------------------------------

#if DEBUGGING_MODE == 0
    #define MAX_PSYC_PAGES 4 //for now debugging with 4   
#else 
    #define MAX_PSYC_PAGES 15   //maximum number of pages per process active in 'physical memory'
#endif

#define MAX_TOTAL_PAGES 30  //assumption: max number of pages a process can have = 30

//----------------------------------------------------------------------------------------------
#define REPLACE_START_IDX 0    //2 //LATEST: Ignore first 8 pages .... ->NOT NEEDED-> //Will leave out pages [0, 1, 2] while replacing

#define DEBUG_MEMORY_INITIAL 1
#define DEBUG_MEMORY_USER_TESTS 1
#define FILEREAD_DEB 0
#define PROCESS_IGNORE_2    1
#define PROCESS_IGNORE      0
#define PROCESS_SH_PID      2
#define CHECKPOINT_FORK 1

#define DEBUG_PRINT 5
#define CHECK_DEB 1

#if DEBUG_PRINT >= 1
    #define SYSTEM_CALL_PRINT 1
#endif

#define WRITE_SWAPFILE_DEB 0
#define PRINT_PAGE_MASTER 1
#define PRINT_PAGE_MASTER_SWAPFILE_ARR 1

#define IGNORE_PAGES_CNT 8


#define PAGE_REPLACEMENT_MODE 2

#if PAGE_REPLACEMENT_MODE == 1
    #define FIFO 1
#elif PAGE_REPLACEMENT_MODE == 2
    #define SECOND_CHANCE 2
#endif

//#define PAGE_REPLACEMENT_MODE FIFO
//#define PAGE_REPLACEMENT_MODE SECOND_CHANCE



#endif	// MYDEBUG_H

