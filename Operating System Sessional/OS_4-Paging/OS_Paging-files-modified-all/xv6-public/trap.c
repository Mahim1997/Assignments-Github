#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[]; // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void
tvinit(void) {
    int i;

    for (i = 0; i < 256; i++)
        SETGATE(idt[i], 0, SEG_KCODE << 3, vectors[i], 0);
    SETGATE(idt[T_SYSCALL], 1, SEG_KCODE << 3, vectors[T_SYSCALL], DPL_USER);

    initlock(&tickslock, "time");
}

void
idtinit(void) {
    lidt(idt, sizeof (idt));
}

//-------------------------------------------My Function to handle page fault starts------------------------------

static pte_t *
my_walkpgdir(pde_t *pgdir, const void *va, int alloc) {
    pde_t *pde;
    pte_t *pgtab;

    pde = &pgdir[PDX(va)];
    if (*pde & PTE_P) { //PTE_P flag i.e. page table is NOT present at dir
        pgtab = (pte_t*) P2V(PTE_ADDR(*pde));
    } else {
        return 0; //return 0 otherwise
    }
    /*else {
        if (!alloc || (pgtab = (pte_t*) kalloc()) == 0)
            return 0;
        // Make sure all those PTE_P bits are zero.
        memset(pgtab, 0, PGSIZE);
        // The permissions here are overly generous, but they can
        // be further restricted by the permissions in the page table
        // entries, if necessary.
     *pde = V2P(pgtab) | PTE_P | PTE_W | PTE_U;
    }*/

    return &pgtab[PTX(va)];
}

void handlePageFault() {
    char *va_accessed = (char *) rcr2(); //the %CR2 register to determine the faulting address and identify the page
#if DEBUG_PRINT >= 3
    cprintf("Inside handlePageFault() .. trap of T_PGFLT ...va accessed = %x, pid = %d\n", va_accessed, myproc()->pid);
#endif  

    pte_t *page_table_entry = my_walkpgdir(myproc()->pgdir, va_accessed, 0);
    if (page_table_entry) {
        if ((uint) * page_table_entry & PTE_PG) { //Swapped bit is active
#ifdef FIFO
            if (swap_FIFO((char*) PTE_ADDR(va_accessed)) < 0) //In vm.c
            {
                cprintf("Error in swapFifO() , return;\n");
                return;
            }
#endif 

#ifdef SECOND_CHANCE
            if (swap_SecondChance((char*) PTE_ADDR(va_accessed)) < 0) //In vm.c
            {
                cprintf("Error in swapSecondChance() , return;\n");
                return;
            }
#endif
        }
    } else {
        cprintf("\n\nIn trap(), Invalid virtual address access %x so calling exit() pid = %d\n\n", va_accessed, myproc()->pid);
        exit();
    }
#if DEBUG_PRINT >= 3
    cprintf("Exiting function handlePageFault()\n\n");
#endif
    //1. Let's find the walkpgdir to get physical address and check if PTE_P is absent but PTE_PG is present

}

//-------------------------------------------My Function to handle page fault ends--------------------------------

//PAGEBREAK: 41

void
trap(struct trapframe *tf) {
    if (tf->trapno == T_SYSCALL) {
        if (myproc()->killed)
            exit();
        myproc()->tf = tf;
        syscall();
        if (myproc()->killed)
            exit();
        return;
    }

    switch (tf->trapno) {

        case T_IRQ0 + IRQ_TIMER:
            if (cpuid() == 0) {
                acquire(&tickslock);
                ticks++;
                wakeup(&ticks);
                release(&tickslock);
            }
            lapiceoi();
            break;
        case T_IRQ0 + IRQ_IDE:
            ideintr();
            lapiceoi();
            break;
        case T_IRQ0 + IRQ_IDE + 1:
            // Bochs generates spurious IDE1 interrupts.
            break;
        case T_IRQ0 + IRQ_KBD:
            kbdintr();
            lapiceoi();
            break;
        case T_IRQ0 + IRQ_COM1:
            uartintr();
            lapiceoi();
            break;
        case T_IRQ0 + 7:
        case T_IRQ0 + IRQ_SPURIOUS:
            cprintf("cpu%d: spurious interrupt at %x:%x\n",
                    cpuid(), tf->cs, tf->eip);
            lapiceoi();
            break;
            //For page fault TRAP = 14 traps.h
        case T_PGFLT:
            if (myproc()->pid <= 2) { //init or sh
                break;
            }
            handlePageFault();
            return;
            //PAGEBREAK: 13
        default:
            if (myproc() == 0 || (tf->cs & 3) == 0) {
                // In kernel, it must be our mistake.
                cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
                        tf->trapno, cpuid(), tf->eip, rcr2());
                panic("trap");
            }
            // In user space, assume process misbehaved.
            cprintf("pid %d %s: trap %d err %d on cpu %d "
                    "eip 0x%x addr 0x%x--kill proc\n",
                    myproc()->pid, myproc()->name, tf->trapno,
                    tf->err, cpuid(), tf->eip, rcr2());
            myproc()->killed = 1;
    }

    // Force process exit if it has been killed and is in user space.
    // (If it is still executing in the kernel, let it keep running
    // until it gets to the regular system call return.)
    if (myproc() && myproc()->killed && (tf->cs & 3) == DPL_USER)
        exit();

    // Force process to give up CPU on clock tick.
    // If interrupts were on while locks held, would need to check nlock.
    if (myproc() && myproc()->state == RUNNING &&
            tf->trapno == T_IRQ0 + IRQ_TIMER)
        yield();

    // Check if the process has been killed since we yielded
    if (myproc() && myproc()->killed && (tf->cs & 3) == DPL_USER)
        exit();
}
