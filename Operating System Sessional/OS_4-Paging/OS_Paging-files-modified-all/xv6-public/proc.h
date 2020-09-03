#include "param.h"

// Per-CPU state

struct cpu {
    uchar apicid; // Local APIC ID
    struct context *scheduler; // swtch() here to enter scheduler
    struct taskstate ts; // Used by x86 to find stack for interrupt
    struct segdesc gdt[NSEGS]; // x86 global descriptor table
    volatile uint started; // Has the CPU started?
    int ncli; // Depth of pushcli nesting.
    int intena; // Were interrupts enabled before pushcli?
    struct proc *proc; // The process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.

struct context {
    uint edi;
    uint esi;
    uint ebx;
    uint ebp;
    uint eip;
};
//----------------------------------Paging MetaData-------------------------------

struct myPage {
    char *virtual_address; //To keep compatible with the structure below
    //    uint physical_address;    //Actually can be obtained by using V2P(virtual_address) macro
};
//struct page_in_file{
//    char *virtual_addr; //Better keeping char * for writeToSwapFile ...
//};

struct pageMaster {
    int num_pages_in_physical_mem; //Number of pages in physical memory
    int index_array_physical_mem; //To add to the array .... //TAIL
    int index_to_replace; //For FIFO    //HEAD

    int num_pages_in_swap_file; //Number of pages in the swap file
    int index_swapped_file; //Index for swapped_file_page


    int was_page_opened ;
    int ignore_pages_cnt; //8
    struct myPage physical_mem_pages[MAX_PSYC_PAGES]; //Array of pages in phyiscal memory already stored MAX_PSYC [hence array used]
    struct myPage swapped_file_pages[MAX_TOTAL_PAGES]; //Assume at max 30 ta thakbe ?? 15 tao enough hobe
};


//---------------------------------Paging MetaData-----------------------------------

enum procstate {
    UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE
};

// Per-process state

struct proc {
    uint sz; // Size of process memory (bytes)
    pde_t* pgdir; // Page table
    char *kstack; // Bottom of kernel stack for this process
    enum procstate state; // Process state
    int pid; // Process ID
    struct proc *parent; // Parent process
    struct trapframe *tf; // Trap frame for current syscall
    struct context *context; // swtch() here to run process
    void *chan; // If non-zero, sleeping on chan
    int killed; // If non-zero, have been killed
    struct file *ofile[NOFILE]; // Open files
    struct inode *cwd; // Current directory
    char name[16]; // Process name (debugging)

    //Swap file. must initiate with create swap file
    struct file *swapFile; //page file


    struct pageMaster pagingController;
};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap

