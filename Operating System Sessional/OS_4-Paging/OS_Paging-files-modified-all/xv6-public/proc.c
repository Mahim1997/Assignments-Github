#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

struct {
    struct spinlock lock;
    struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

void
pinit(void) {
    initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled

int
cpuid() {
    return mycpu() - cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.

struct cpu*
mycpu(void) {
    int apicid, i;

    if (readeflags() & FL_IF)
        panic("mycpu called with interrupts enabled\n");

    apicid = lapicid();
    // APIC IDs are not guaranteed to be contiguous. Maybe we should have
    // a reverse map, or reserve a register to store &cpus[i].
    for (i = 0; i < ncpu; ++i) {
        if (cpus[i].apicid == apicid)
            return &cpus[i];
    }
    panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure

struct proc*
myproc(void) {
    struct cpu *c;
    struct proc *p;
    pushcli();
    c = mycpu();
    p = c->proc;
    popcli();
    return p;
}

//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.

static struct proc*
allocproc(void) {
    struct proc *p;
    char *sp;

    acquire(&ptable.lock);

    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
        if (p->state == UNUSED)
            goto found;

    release(&ptable.lock);
    return 0;

found:
    p->state = EMBRYO;
    p->pid = nextpid++;

    release(&ptable.lock);

    // Allocate kernel stack.
    if ((p->kstack = kalloc()) == 0) {
        p->state = UNUSED;
        return 0;
    }
    sp = p->kstack + KSTACKSIZE;

    // Leave room for trap frame.
    sp -= sizeof *p->tf;
    p->tf = (struct trapframe*) sp;

    // Set up new context to start executing at forkret,
    // which returns to trapret.
    sp -= 4;
    *(uint*) sp = (uint) trapret;

    sp -= sizeof *p->context;
    p->context = (struct context*) sp;
    memset(p->context, 0, sizeof *p->context);
    p->context->eip = (uint) forkret;

    ///----------------------------------------- For paging things -------------------------------------------

    ///----------------------------------------- For paging things -------------------------------------------

    if (p->pid > PROCESS_IGNORE_2) {

#if DEBUG_PRINT >= 3
        cprintf("Inside proc.c alloc() for process pid = %d, proc deb name = %s ,opening swapfile and initialising\n",
                p->pid, p->name);
#endif
        //initialise parameters
        p->pagingController.num_pages_in_physical_mem = 0;
        p->pagingController.num_pages_in_swap_file = 0;
        p->pagingController.index_array_physical_mem = 0;
        p->pagingController.index_to_replace = REPLACE_START_IDX; //i.e. 0
        p->pagingController.ignore_pages_cnt = IGNORE_PAGES_CNT; //i.e. 8
        p->pagingController.index_swapped_file = 0;

        int _iter;
        for (_iter = 0; _iter < MAX_PSYC_PAGES; _iter++) {
            p->pagingController.physical_mem_pages[_iter].virtual_address = (char *) 0; //(char *) 0xffff; //(uint) 0;//(char*)(0x00);
        }
        //initialising parameters done 
        int retVal_deb = createSwapFile(p); //create swap file
        if (retVal_deb != 0) {
            cprintf("\n\n-->>>PROBLEM in createSwapFile in allocproc() for proc.c pid = %d\n", p->pid);
            panic("In allocproc() , createSwapFile error\n");
        }

#if DEBUG_PRINT >= 3
        cprintf("Inside alloc() for pid=%d, success in opening file\n\n", p->pid);
#endif

    }


    return p;
}

//PAGEBREAK: 32
// Set up first user process.

void
userinit(void) {
    struct proc *p;
    extern char _binary_initcode_start[], _binary_initcode_size[];

    p = allocproc();

    initproc = p;
    if ((p->pgdir = setupkvm()) == 0)
        panic("userinit: out of memory?");
    inituvm(p->pgdir, _binary_initcode_start, (int) _binary_initcode_size);
    p->sz = PGSIZE;
    memset(p->tf, 0, sizeof (*p->tf));
    p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
    p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
    p->tf->es = p->tf->ds;
    p->tf->ss = p->tf->ds;
    p->tf->eflags = FL_IF;
    p->tf->esp = PGSIZE;
    p->tf->eip = 0; // beginning of initcode.S

    safestrcpy(p->name, "initcode", sizeof (p->name));
    p->cwd = namei("/");

    // this assignment to p->state lets other cores
    // run this process. the acquire forces the above
    // writes to be visible, and the lock is also needed
    // because the assignment might not be atomic.
    acquire(&ptable.lock);

    p->state = RUNNABLE;

    release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.

int
growproc(int n) {
    uint sz;
    struct proc *curproc = myproc();

    sz = curproc->sz;
    if (n > 0) {
#if DEBUG_PRINT >= 4
        cprintf("\n=>> In growproc(n) about to call my_allocuvm( ) , pid = %d, n = %d, curproc->sz = %d\n", curproc->pid, n, sz);
#endif
        if ((sz = my_allocuvm(curproc->pgdir, sz, sz + n)) == 0) {
            return -1;
        }
    } else if (n < 0) {

#if DEBUG_PRINT >= 4
        cprintf("\n=>> In growproc(n) abt to call my_deallocuvm( ) , pid = %d, n = %d, curproc->sz = %d\n", curproc->pid, n, sz);
#endif
        if ((sz = my_deallocuvm(curproc->pgdir, sz, sz + n)) == 0) {
            return -1;
        }
    }
    curproc->sz = sz;
    switchuvm(curproc);
    return 0;
}
//------------------------------------------------For Paging------------------------------------------------------------------
//----------------------------------

int
strcmp(const char *str1, const char *str2) {
    while (*str1 && *str1 == *str2)
        str1++, str2++;
    return (uchar)*str1 - (uchar) * str2;
}
//----------------------------------

void
copyMemoryThings(struct proc* oldProcess, struct proc* newProcess) {
    int will_copy_swap_file = 1;
    if (strcmp(oldProcess->name, "init") == 0) {
        will_copy_swap_file = 0;
        cprintf("\n\n-->>>> oldProcess pid = %d is init process , new pid = %d\n", oldProcess->pid, newProcess->pid);
    } else if (strcmp(oldProcess->name, "sh") == 0) {
        will_copy_swap_file = 0;
        cprintf("\n\n-->>>>> oldProcess pid = %d is sh process, new pid = %d\n", oldProcess->pid, newProcess->pid);
    }
    if (oldProcess->pid == 1) {
        //oldPid < 1 ... ignore ...
        return;
    }

#if DEBUG_PRINT >= 3
    cprintf("In copyMemory() , oldPid = %d, newPid = %d, willCopySwapFile = %d\n", oldProcess->pid, newProcess->pid, will_copy_swap_file);
#endif

    if (oldProcess->pid > PROCESS_SH_PID) {
        //Normal fork() er copy
#if DEBUG_PRINT >= 3
        cprintf("Normal proc memory is being copied .. printing page controller of oldProc pid = %d", oldProcess->pid);
        printPageController(oldProcess);
#endif
        //1 Copying variables
        newProcess->pagingController.ignore_pages_cnt = oldProcess->pagingController.ignore_pages_cnt;
        newProcess->pagingController.index_array_physical_mem = oldProcess->pagingController.index_array_physical_mem;
        newProcess->pagingController.index_swapped_file = oldProcess->pagingController.index_swapped_file;
        newProcess->pagingController.index_to_replace = oldProcess->pagingController.index_to_replace;
        newProcess->pagingController.num_pages_in_physical_mem = oldProcess->pagingController.num_pages_in_physical_mem;
        newProcess->pagingController.num_pages_in_swap_file = oldProcess->pagingController.num_pages_in_swap_file;

#if CHECKPOINT_FORK == 1
        cprintf("Variables copying done in fork(), newPID = %d, oldPID = %d\n", newProcess->pid, oldProcess->pid);
#endif
        //2 Copying arrays
        for (int i = 0; i < oldProcess->pagingController.num_pages_in_physical_mem; i++) {
            newProcess->pagingController.physical_mem_pages[i].virtual_address = oldProcess->pagingController.physical_mem_pages[i].virtual_address;
        }
        for (int i = 0; i < oldProcess->pagingController.num_pages_in_swap_file; i++) {
            newProcess->pagingController.swapped_file_pages[i].virtual_address = oldProcess->pagingController.swapped_file_pages[i].virtual_address;
        }
#if CHECKPOINT_FORK == 1
        cprintf("Array copying done in fork(), newPID = %d, oldPID = %d\n", newProcess->pid, oldProcess->pid);
#endif
        //3 Copy Swap File Contents
        if (oldProcess->pagingController.num_pages_in_swap_file > 0) { //If swap file HAS content
            int placeOnFile = 0;
            int BUFF_SIZE = PGSIZE / 2;
            char buff[BUFF_SIZE]; //panic hoile less than PGSIZE nite hobe ...
            while (1) {
                memset(buff, 0, sizeof (buff));
                //Read from old process's swap file
                int read_ret = readFromSwapFile(oldProcess, buff, placeOnFile, BUFF_SIZE);
                if (read_ret == 0) { //Done reading EOF 
                    break;
                }
                if (read_ret < 0) {
                    cprintf("In fork(), copyMemory(), reading from swap file error, calling return;\n");
                    return;
                }
                //Write back to new process's swap file
                int write_ret = writeToSwapFile(newProcess, buff, placeOnFile, BUFF_SIZE);
                if (write_ret < 0) {
                    cprintf("In fork(), write to swap file error for newProc pid = %d, calling panic\n", newProcess->pid);
                    panic("Write to swap file error for new process in fork");
                }
                placeOnFile += read_ret;
            }
        }
#if CHECKPOINT_FORK == 1
        cprintf("Swap file copying done in fork(), newPID = %d, oldPID = %d\n", newProcess->pid, oldProcess->pid);
#endif
        //1.4 Placing in physical mem ?? Already done by copyuvm() !

    }

}
//------------------------------------------------For Paging------------------------------------------------------------------



// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.

int
fork(void) {
    int i, pid;
    struct proc *np;
    struct proc *curproc = myproc();

    // Allocate process.
    if ((np = allocproc()) == 0) {
        return -1;
    }

    // Copy process state from proc.
    if ((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0) {
        kfree(np->kstack);
        np->kstack = 0;
        np->state = UNUSED;
        return -1;
    }
    np->sz = curproc->sz;
    np->parent = curproc;
    *np->tf = *curproc->tf;

    // Clear %eax so that fork returns 0 in the child.
    np->tf->eax = 0;

    for (i = 0; i < NOFILE; i++)
        if (curproc->ofile[i])
            np->ofile[i] = filedup(curproc->ofile[i]);
    np->cwd = idup(curproc->cwd);

    safestrcpy(np->name, curproc->name, sizeof (curproc->name));

    pid = np->pid;

    //---------------------For Paging----------------
#if DEBUG_PRINT >= 3
    cprintf("Inside fork(), abt to call copyMemoryThings(curProc pid = %d, newProc pid = %d)\n", curproc->pid, np->pid);
#endif

    copyMemoryThings(curproc, np); //Before making it runnnable ... call memory things copier
    //---------------------For Paging----------------

    acquire(&ptable.lock);

    np->state = RUNNABLE;

    release(&ptable.lock);

#if DEBUG_PRINT >= 3
    cprintf("After copyMemoryThings(), Exiting fork( ) , returning pid = %d\n", pid);
#endif

    return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.

void
exit(void) {
    struct proc *curproc = myproc();
    struct proc *p;
    int fd;

    if (curproc == initproc)
        panic("init exiting");

    if (myproc()->pid > 2) {
#if DEBUG_PRINT >= 4
        cprintf("Inside exit() for pid = %d, delete the file required\n", myproc()->pid);
#endif
        int retVal_deb = removeSwapFile(myproc());
        if (retVal_deb != 0) {
            cprintf("Error in removing swap file for proc id = %d\n", myproc()->pid);
#if DEBUG_PRINT >= 4
            if (retVal_deb == 0) {
                cprintf("Removed swap file for proc id = %d\n", myproc()->pid);
            }
#endif
        }
    }

    // Close all open files.
    for (fd = 0; fd < NOFILE; fd++) {
        if (curproc->ofile[fd]) {
            fileclose(curproc->ofile[fd]);
            curproc->ofile[fd] = 0;
        }
    }

    begin_op();
    iput(curproc->cwd);
    end_op();
    curproc->cwd = 0;

    acquire(&ptable.lock);

    // Parent might be sleeping in wait().
    wakeup1(curproc->parent);

    // Pass abandoned children to init.
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if (p->parent == curproc) {
            p->parent = initproc;
            if (p->state == ZOMBIE)
                wakeup1(initproc);
        }
    }

    // Jump into the scheduler, never to return.
    curproc->state = ZOMBIE;
    sched();
    panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.

int
wait(void) {
    struct proc *p;
    int havekids, pid;
    struct proc *curproc = myproc();

    acquire(&ptable.lock);
    for (;;) {
        // Scan through table looking for exited children.
        havekids = 0;
        for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
            if (p->parent != curproc)
                continue;
            havekids = 1;
            if (p->state == ZOMBIE) {
                // Found one.
                pid = p->pid;
                kfree(p->kstack);
                p->kstack = 0;
                freevm(p->pgdir);
                p->pid = 0;
                p->parent = 0;
                p->name[0] = 0;
                p->killed = 0;
                p->state = UNUSED;
                release(&ptable.lock);
                return pid;
            }
        }

        // No point waiting if we don't have any children.
        if (!havekids || curproc->killed) {
            release(&ptable.lock);
            return -1;
        }

        // Wait for children to exit.  (See wakeup1 call in proc_exit.)
        sleep(curproc, &ptable.lock); //DOC: wait-sleep
    }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.

void
scheduler(void) {
    struct proc *p;
    struct cpu *c = mycpu();
    c->proc = 0;

    for (;;) {
        // Enable interrupts on this processor.
        sti();

        // Loop over process table looking for process to run.
        acquire(&ptable.lock);
        for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
            if (p->state != RUNNABLE)
                continue;

            // Switch to chosen process.  It is the process's job
            // to release ptable.lock and then reacquire it
            // before jumping back to us.
            c->proc = p;
            switchuvm(p);
            p->state = RUNNING;

            swtch(&(c->scheduler), p->context);
            switchkvm();

            // Process is done running for now.
            // It should have changed its p->state before coming back.
            c->proc = 0;
        }
        release(&ptable.lock);

    }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.

void
sched(void) {
    int intena;
    struct proc *p = myproc();

    if (!holding(&ptable.lock))
        panic("sched ptable.lock");
    if (mycpu()->ncli != 1)
        panic("sched locks");
    if (p->state == RUNNING)
        panic("sched running");
    if (readeflags() & FL_IF)
        panic("sched interruptible");
    intena = mycpu()->intena;
    swtch(&p->context, mycpu()->scheduler);
    mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.

void
yield(void) {
    acquire(&ptable.lock); //DOC: yieldlock
    myproc()->state = RUNNABLE;
    sched();
    release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.

void
forkret(void) {
    static int first = 1;
    // Still holding ptable.lock from scheduler.
    release(&ptable.lock);

    if (first) {
        // Some initialization functions must be run in the context
        // of a regular process (e.g., they call sleep), and thus cannot
        // be run from main().
        first = 0;
        iinit(ROOTDEV);
        initlog(ROOTDEV);
    }

    // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.

void
sleep(void *chan, struct spinlock *lk) {
    struct proc *p = myproc();

    if (p == 0)
        panic("sleep");

    if (lk == 0)
        panic("sleep without lk");

    // Must acquire ptable.lock in order to
    // change p->state and then call sched.
    // Once we hold ptable.lock, we can be
    // guaranteed that we won't miss any wakeup
    // (wakeup runs with ptable.lock locked),
    // so it's okay to release lk.
    if (lk != &ptable.lock) { //DOC: sleeplock0
        acquire(&ptable.lock); //DOC: sleeplock1
        release(lk);
    }
    // Go to sleep.
    p->chan = chan;
    p->state = SLEEPING;

    sched();

    // Tidy up.
    p->chan = 0;

    // Reacquire original lock.
    if (lk != &ptable.lock) { //DOC: sleeplock2
        release(&ptable.lock);
        acquire(lk);
    }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.

static void
wakeup1(void *chan) {
    struct proc *p;

    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
        if (p->state == SLEEPING && p->chan == chan)
            p->state = RUNNABLE;
}

// Wake up all processes sleeping on chan.

void
wakeup(void *chan) {
    acquire(&ptable.lock);
    wakeup1(chan);
    release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).

int
kill(int pid) {
    struct proc *p;

    acquire(&ptable.lock);
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if (p->pid == pid) {
            p->killed = 1;
            // Wake process from sleep if necessary.
            if (p->state == SLEEPING)
                p->state = RUNNABLE;
            release(&ptable.lock);
            return 0;
        }
    }
    release(&ptable.lock);
    return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.

void
procdump(void) {
    static char *states[] = {
        [UNUSED] "unused",
        [EMBRYO] "embryo",
        [SLEEPING] "sleep ",
        [RUNNABLE] "runble",
        [RUNNING] "run   ",
        [ZOMBIE] "zombie"
    };
    int i;
    struct proc *p;
    char *state;
    uint pc[10];

    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if (p->state == UNUSED)
            continue;
        if (p->state >= 0 && p->state < NELEM(states) && states[p->state])
            state = states[p->state];
        else
            state = "???";
        cprintf("%d %s %s", p->pid, state, p->name);
        if (p->state == SLEEPING) {
            getcallerpcs((uint*) p->context->ebp + 2, pc);
            for (i = 0; i < 10 && pc[i] != 0; i++)
                cprintf(" %p", pc[i]);
        }
        cprintf("\n");
    }
}
