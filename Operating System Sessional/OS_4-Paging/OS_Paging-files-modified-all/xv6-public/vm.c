#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "elf.h"

extern char data[]; // defined by kernel.ld
pde_t *kpgdir; // for use in scheduler()

// Set up CPU's kernel segment descriptors.
// Run once on entry on each CPU.

void
seginit(void) {
    struct cpu *c;

    // Map "logical" addresses to virtual addresses using identity map.
    // Cannot share a CODE descriptor for both kernel and user
    // because it would have to have DPL_USR, but the CPU forbids
    // an interrupt from CPL=0 to DPL=3.
    c = &cpus[cpuid()];
    c->gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0, 0xffffffff, 0);
    c->gdt[SEG_KDATA] = SEG(STA_W, 0, 0xffffffff, 0);
    c->gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0, 0xffffffff, DPL_USER);
    c->gdt[SEG_UDATA] = SEG(STA_W, 0, 0xffffffff, DPL_USER);
    lgdt(c->gdt, sizeof (c->gdt));

}

// Return the address of the PTE in page table pgdir
// that corresponds to virtual address va.  If alloc!=0,
// create any required page table pages.

static pte_t *
walkpgdir(pde_t *pgdir, const void *va, int alloc) {
    pde_t *pde;
    pte_t *pgtab;

    pde = &pgdir[PDX(va)];
    if (*pde & PTE_P) {
        pgtab = (pte_t*) P2V(PTE_ADDR(*pde));
    } else {
        if (!alloc || (pgtab = (pte_t*) kalloc()) == 0)
            return 0;
        // Make sure all those PTE_P bits are zero.
        memset(pgtab, 0, PGSIZE);
        // The permissions here are overly generous, but they can
        // be further restricted by the permissions in the page table
        // entries, if necessary.
        *pde = V2P(pgtab) | PTE_P | PTE_W | PTE_U;
    }

    return &pgtab[PTX(va)];
}

// Create PTEs for virtual addresses starting at va that refer to
// physical addresses starting at pa. va and size might not
// be page-aligned.

static int
mappages(pde_t *pgdir, void *va, uint size, uint pa, int perm) {
    char *a, *last;
    pte_t *pte;

    a = (char*) PGROUNDDOWN((uint) va);
    last = (char*) PGROUNDDOWN(((uint) va) + size - 1);
    for (;;) {
        if ((pte = walkpgdir(pgdir, a, 1)) == 0)
            return -1;
        if (*pte & PTE_P)
            panic("remap");
        *pte = pa | perm | PTE_P;
        if (a == last)
            break;
        a += PGSIZE;
        pa += PGSIZE;
    }



    return 0;
}

// There is one page table per process, plus one that's used when
// a CPU is not running any process (kpgdir). The kernel uses the
// current process's page table during system calls and interrupts;
// page protection bits prevent user code from using the kernel's
// mappings.
//
// setupkvm() and exec() set up every page table like this:
//
//   0..KERNBASE: user memory (text+data+stack+heap), mapped to
//                phys memory allocated by the kernel
//   KERNBASE..KERNBASE+EXTMEM: mapped to 0..EXTMEM (for I/O space)
//   KERNBASE+EXTMEM..data: mapped to EXTMEM..V2P(data)
//                for the kernel's instructions and r/o data
//   data..KERNBASE+PHYSTOP: mapped to V2P(data)..PHYSTOP,
//                                  rw data + free physical memory
//   0xfe000000..0: mapped direct (devices such as ioapic)
//
// The kernel allocates physical memory for its heap and for user memory
// between V2P(end) and the end of physical memory (PHYSTOP)
// (directly addressable from end..P2V(PHYSTOP)).

// This table defines the kernel's mappings, which are present in
// every process's page table.

static struct kmap {
    void *virt;
    uint phys_start;
    uint phys_end;
    int perm;
} kmap[] = {
    { (void*) KERNBASE, 0, EXTMEM, PTE_W}, // I/O space
    { (void*) KERNLINK, V2P(KERNLINK), V2P(data), 0}, // kern text+rodata
    { (void*) data, V2P(data), PHYSTOP, PTE_W}, // kern data+memory
    { (void*) DEVSPACE, DEVSPACE, 0, PTE_W}, // more devices
};

// Set up kernel part of a page table.

pde_t*
setupkvm(void) {
    pde_t *pgdir;
    struct kmap *k;

    if ((pgdir = (pde_t*) kalloc()) == 0)
        return 0;
    memset(pgdir, 0, PGSIZE);
    if (P2V(PHYSTOP) > (void*) DEVSPACE)
        panic("PHYSTOP too high");
    for (k = kmap; k < &kmap[NELEM(kmap)]; k++)
        if (mappages(pgdir, k->virt, k->phys_end - k->phys_start,
                (uint) k->phys_start, k->perm) < 0) {
            freevm(pgdir);
            return 0;
        }


    return pgdir;
}

// Allocate one page table for the machine for the kernel address
// space for scheduler processes.

void
kvmalloc(void) {

    kpgdir = setupkvm();
    switchkvm();
}

// Switch h/w page table register to the kernel-only page table,
// for when no process is running.

void
switchkvm(void) {
    lcr3(V2P(kpgdir)); // switch to the kernel page table
}

// Switch TSS and h/w page table to correspond to process p.

void
switchuvm(struct proc *p) {
    if (p == 0)
        panic("switchuvm: no process");
    if (p->kstack == 0)
        panic("switchuvm: no kstack");
    if (p->pgdir == 0)
        panic("switchuvm: no pgdir");

    pushcli();
    mycpu()->gdt[SEG_TSS] = SEG16(STS_T32A, &mycpu()->ts,
            sizeof (mycpu()->ts) - 1, 0);
    mycpu()->gdt[SEG_TSS].s = 0;
    mycpu()->ts.ss0 = SEG_KDATA << 3;
    mycpu()->ts.esp0 = (uint) p->kstack + KSTACKSIZE;
    // setting IOPL=0 in eflags *and* iomb beyond the tss segment limit
    // forbids I/O instructions (e.g., inb and outb) from user space
    mycpu()->ts.iomb = (ushort) 0xFFFF;
    ltr(SEG_TSS << 3);
    lcr3(V2P(p->pgdir)); // switch to process's address space
    popcli();
}

// Load the initcode into address 0 of pgdir.
// sz must be less than a page.

void
inituvm(pde_t *pgdir, char *init, uint sz) {
    char *mem;

    if (sz >= PGSIZE)
        panic("inituvm: more than a page");
    mem = kalloc();
    memset(mem, 0, PGSIZE);
    mappages(pgdir, 0, PGSIZE, V2P(mem), PTE_W | PTE_U);
    memmove(mem, init, sz);
}

// Load a program segment into pgdir.  addr must be page-aligned
// and the pages from addr to addr+sz must already be mapped.

int
loaduvm(pde_t *pgdir, char *addr, struct inode *ip, uint offset, uint sz) {
    uint i, pa, n;
    pte_t *pte;

    if ((uint) addr % PGSIZE != 0)
        panic("loaduvm: addr must be page aligned");
    for (i = 0; i < sz; i += PGSIZE) {
        if ((pte = walkpgdir(pgdir, addr + i, 0)) == 0)
            panic("loaduvm: address should exist");
        pa = PTE_ADDR(*pte);
        if (sz - i < PGSIZE)
            n = sz - i;
        else
            n = PGSIZE;
        if (readi(ip, P2V(pa), offset + i, n) != n)
            return -1;
    }
    return 0;
}

//---------------------------------------------For printing page controller---------------------------------------------------------

void printPageMaster(struct pageMaster *pointer) {
    //    cprintf("Inside printPageMaster.... pointer = %p\n", pointer);
#if PRINT_PAGE_MASTER == 1
    if (pointer != 0) {
        cprintf("\n\nPage controller pid = %d: num_pg_phy = %d, num_pg_swapF = %d, idx_rep = %d, idx_arr = %d, index_swapped_file = %d\n",
                myproc()->pid, pointer->num_pages_in_physical_mem, pointer->num_pages_in_swap_file,
                pointer->index_to_replace, pointer->index_array_physical_mem, pointer->index_swapped_file);
        cprintf("Printing array physical_mem(only va): \n");
        for (int i = 0; i < MAX_PSYC_PAGES; i++) {
            cprintf("idx: %d, va = %x\n", i, pointer->physical_mem_pages[i].virtual_address);
        }
#if PRINT_PAGE_MASTER_SWAPFILE_ARR == 1
        cprintf("\n\nPrinting array swapped file: \n");
        for (int i = 0; i < pointer->index_swapped_file; i++) {
            cprintf("idx = %d, va = %x\n",
                    i, pointer->swapped_file_pages[i].virtual_address);
        }
#endif
        cprintf("\n\n");
    }
#endif
}

void printPageController(struct proc *p) {
    if (p)
        printPageMaster(&p->pagingController);
}
//------------------------------------------------------------------------------------------------------


// Allocate page tables and physical memory to grow process from oldsz to
// newsz, which need not be page aligned.  Returns new size or 0 on error.
// Allocate page tables and physical memory to grow process from oldsz to
// newsz, which need not be page aligned.  Returns new size or 0 on error.

int
allocuvm(pde_t *pgdir, uint oldsz, uint newsz) {
    char *mem;
    uint a;

    if (newsz >= KERNBASE)
        return 0;
    if (newsz < oldsz)
        return oldsz;

    a = PGROUNDUP(oldsz);
    for (; a < newsz; a += PGSIZE) {
        mem = kalloc();
        if (mem == 0) {
            cprintf("allocuvm out of memory\n");
            deallocuvm(pgdir, newsz, oldsz);
            return 0;
        }
        memset(mem, 0, PGSIZE);
        if (mappages(pgdir, (char*) a, PGSIZE, V2P(mem), PTE_W | PTE_U) < 0) {
            cprintf("allocuvm out of memory (2)\n");
            deallocuvm(pgdir, newsz, oldsz);
            kfree(mem);
            return 0;
        }
    }
    return newsz;
}
//------------------------------------------------------- My allocuvm( ) procedure -------------------------------------------

//------------------- For Second Chance ---------------

int getAndRemoveReferenceBit(char *virtual_addr) {
    uint accessedBit;
    pte_t *pte = walkpgdir(myproc()->pgdir, (void*) virtual_addr, 0);
    if (!*pte)
        panic("In getAndRemoveReferenceBit() pte is empty");
    accessedBit = (*pte) & PTE_A;
    (*pte) &= ~PTE_A; //Check for access bit ... if 1 -> return 1 BUT make 0 

#if DEBUG_PRINT >= 3
    //cprintf("==>>> In getAndRemoveReferenceBit(%x), returning %d", virtual_addr, accessedBit);
#endif
    return accessedBit;
}
// ----------------------------------------------------

int
my_allocuvm(pde_t *pgdir, uint oldsz, uint newsz) {
    char *mem;
    uint a;
    if (newsz >= KERNBASE)
        return 0;
    if (newsz < oldsz)
        return oldsz;

    struct proc *currProc = myproc();

    a = PGROUNDUP(oldsz);

#if DEBUG_PRINT >= 4
    if (currProc->pid > 0) {
        cprintf("Inside my_allocuvm( ) for pid = %d, oldsz = %d, newsz = %d, a = %d\n\n", currProc->pid, oldsz, newsz, a);
    }
#endif

    int will_use_new_page = 1; //Do we take new physical page by kalloc() or do we swap() ?? Question is answered in IF[physicalPage == MAX_PAGE] condition
    char *virtual_addr_to_replace = 0; //For replacing
    int index_replacing = -1; //For replacing

    for (; a < newsz; a += PGSIZE) {

        //Checking if num of pages in physical memory for this process EQUALS the MAX_PSYC
        if (currProc->pagingController.num_pages_in_physical_mem >= MAX_PSYC_PAGES) {
            will_use_new_page = 0; //DONOT TAKE NEW PAGE 

            index_replacing = currProc->pagingController.index_to_replace; //PLACE current idx_to_replace here 

            //Increment index to replace
            if (currProc->pagingController.index_to_replace == MAX_PSYC_PAGES - 1) {
                currProc->pagingController.index_to_replace = REPLACE_START_IDX;
            } else {
                currProc->pagingController.index_to_replace++;
            }
        }
        //end if

        mem = kalloc(); ///this is a virtual page pointer 

        if (mem == 0) {
            cprintf("my_allocuvm out of memory\n");
            my_deallocuvm(pgdir, newsz, oldsz);
            return 0;
        }
        if (will_use_new_page == 0) { //Do not allocate new page ... replace page
#ifdef FIFO
            //Step 1. Write the 'replaced page' to the swapFile ... Update the variables accordingly
            virtual_addr_to_replace = (char *) (currProc->pagingController.physical_mem_pages[index_replacing].virtual_address);

#if DEBUG_PRINT >= 4
            cprintf("==>>va to replace = %x, idx_replace = %d\n\n", (char *) virtual_addr_to_replace, index_replacing);
#endif

            char *_va_free_PTE = (char *) PTE_ADDR(P2V_WO(*walkpgdir(currProc->pgdir, (char *) virtual_addr_to_replace, 0)));
            int swapFileIdx = currProc->pagingController.index_swapped_file;
            int writeToSwapFile_ret = writeToSwapFile(currProc, (char *) virtual_addr_to_replace, (currProc->pagingController.index_swapped_file * PGSIZE), PGSIZE);

            if (writeToSwapFile_ret == -1) {
                panic("\nERROR IN WRITING TO SWAPPED PAGE FILE, returning -1\n\n");
                return -1;
            }
            currProc->pagingController.num_pages_in_swap_file++;
            currProc->pagingController.swapped_file_pages[swapFileIdx].virtual_address = virtual_addr_to_replace;
            currProc->pagingController.index_swapped_file++; //Increment by 1 , Decrement in my_deallocuvm( )//NO !! _virtual_addr_to_free; //Put virtual_addr here

            //Step 2. Replace in the physical_memory,  the new virtual addr 'a'
            currProc->pagingController.physical_mem_pages[index_replacing].virtual_address = (char *) a; //Put 

            //Step 3. Free the physical page translated by the temp variable #virtual_addr_to_replace
            //Just call kfree( ) since kfree takes virtual address as parameter

#if DEBUG_PRINT >= 4
            cprintf(">>> ABOUT TO CALL kfree(%x)\n", virtual_addr_to_replace/*_virtual_addr_to_free*/);
#endif
            kfree(_va_free_PTE); //works

            //Step 4. Flag setting in PTE ... PTE_P and PTE_PG should be 0
            pte_t *PTE_ENTRY_pointer = walkpgdir(currProc->pgdir, virtual_addr_to_replace, 0); //alloc = 0
            *PTE_ENTRY_pointer = PTE_W | PTE_U | PTE_PG; //Make Writable, allowed by USER, PG is set , PTE_P is automatically cleared
            lcr3(V2P(currProc->pgdir)); //To refresh the TLB after 'paging' operations

#if DEBUG_PRINT >= 4
            cprintf("\n >> Replacing pg in my_allocuvm() , *PTE_ENTRY_pointer = %x, kfree( ).. freed mem va = %x, printing array\n",
                    (char *) *PTE_ENTRY_pointer, (char *) virtual_addr_to_replace); //PTE_ENTRY print %s diya korte gele panic hoy

#endif      
            printPageMaster(&currProc->pagingController);

#endif 

#ifdef SECOND_CHANCE        
#if DEBUG_PRINT >= 3
            cprintf("--->> In secondChance()... replace first write ... headIdx = %d\n", index_replacing);
#endif
            int head_idx = index_replacing;
            int queue_size = currProc->pagingController.num_pages_in_physical_mem;
            while (1) {
                char *v_a_to_check = currProc->pagingController.physical_mem_pages[head_idx].virtual_address;
                if (getAndRemoveReferenceBit(v_a_to_check) != 0) { //if R = 1, make R = 0, advance pointer
                    head_idx = (head_idx + 1) % queue_size; //Circular queue                    
                } else { //R = 0 is found !!
                    break;
                }

            }
            index_replacing = head_idx;
#if DEBUG_PRINT >= 3
            cprintf("--->>> In secondChance()... after circular movement ... headIdx = %d, currReplace = %d\n", 
                index_replacing, myproc()->pagingController.index_to_replace);
#endif
//            currProc->pagingController.index_to_replace = index_replacing;
            virtual_addr_to_replace = (char *) currProc->pagingController.physical_mem_pages[index_replacing].virtual_address;
            //head_idx is the idx for which page R was 0 so replace it and advance head_idx
#if DEBUG_PRINT >= 4
            cprintf("==>> 2nd chance va to replace = %x, idx_replace = %d\n\n", (char *) virtual_addr_to_replace, index_replacing);
#endif

            char *_va_free_PTE = (char *) PTE_ADDR(P2V_WO(*walkpgdir(currProc->pgdir, (char *) virtual_addr_to_replace, 0)));
            int swapFileIdx = currProc->pagingController.index_swapped_file;
            int writeToSwapFile_ret = writeToSwapFile(currProc, (char *) virtual_addr_to_replace, (currProc->pagingController.index_swapped_file * PGSIZE), PGSIZE);

            if (writeToSwapFile_ret == -1) {
                panic("\nERROR IN WRITING TO SWAPPED PAGE FILE 2nd chance , returning -1\n\n");
                return -1;
            }
            currProc->pagingController.num_pages_in_swap_file++;
            currProc->pagingController.swapped_file_pages[swapFileIdx].virtual_address = virtual_addr_to_replace;
            currProc->pagingController.index_swapped_file++; //Increment by 1 , Decrement in my_deallocuvm( )//NO !! _virtual_addr_to_free; //Put virtual_addr here

            //Step 2. Replace in the physical_memory,  the new virtual addr 'a'
            currProc->pagingController.physical_mem_pages[index_replacing].virtual_address = (char *) a; //Put 

            //Step 3. Free the physical page translated by the temp variable #virtual_addr_to_replace
            //Just call kfree( ) since kfree takes virtual address as parameter

#if DEBUG_PRINT >= 4
            cprintf(">>> ABOUT TO CALL kfree(%x) 2nd chance \n", virtual_addr_to_replace/*_virtual_addr_to_free*/);
#endif
            kfree(_va_free_PTE); //works

            //Step 4. Flag setting in PTE ... PTE_P and PTE_PG should be 0
            pte_t *PTE_ENTRY_pointer = walkpgdir(currProc->pgdir, virtual_addr_to_replace, 0); //alloc = 0
            *PTE_ENTRY_pointer = PTE_W | PTE_U | PTE_PG; //Make Writable, allowed by USER, PG is set , PTE_P is automatically cleared
            lcr3(V2P(currProc->pgdir)); //To refresh the TLB after 'paging' operations

#if DEBUG_PRINT >= 4
            cprintf("\n >> Replacing pg 2nd chance in my_allocuvm() , *PTE_ENTRY_pointer = %x, kfree( ).. freed mem va = %x, printing array\n",
                    (char *) *PTE_ENTRY_pointer, (char *) virtual_addr_to_replace); //PTE_ENTRY print %s diya korte gele panic hoy

#endif      
            printPageMaster(&currProc->pagingController);


#endif 
        }//end if for swap out page after MAX_PSYC pages 
        else if (will_use_new_page == 1 && currProc->pagingController.ignore_pages_cnt == 0) { //Allocate new page
            currProc->pagingController.num_pages_in_physical_mem++; //Increment number of pages ...
            //Place virtual address in the array [FIFO QUEUE]
            currProc->pagingController.physical_mem_pages[currProc->pagingController.index_array_physical_mem].virtual_address = (char *) a;
            currProc->pagingController.index_array_physical_mem++;

#if DEBUG_PRINT >= 4
            cprintf("\n--==>> In my_allocuvm for loop, Incrementing physical page num now = %d for pid = %d, printing pageController\n",
                    currProc->pagingController.num_pages_in_physical_mem, currProc->pid);

#endif
            printPageMaster(&currProc->pagingController);
        } else if (will_use_new_page == 1 && currProc->pagingController.ignore_pages_cnt > 0) { //Ignore FIRST 8 pages [WHY ?? NO idea]
            currProc->pagingController.ignore_pages_cnt--;
        }

        //-------------------------------------------------DONE-------------------------------------------------------

        memset(mem, 0, PGSIZE);

        int ret_map_pages = mappages(pgdir, (char*) a, PGSIZE, V2P(mem), PTE_W | PTE_U);
        if (ret_map_pages < 0) {
            cprintf("allocuvm out of memory (2)\n");
            deallocuvm(pgdir, newsz, oldsz);

            kfree(mem); ///delete the physical address (Handled by time flag ...)
            return 0;
        }
    }


    return newsz;
}


// Deallocate user pages to bring the process size from oldsz to
// newsz.  oldsz and newsz need not be page-aligned, nor does newsz
// need to be less than oldsz.  oldsz can be larger than the actual
// process size.  Returns the new process size.

int
deallocuvm(pde_t *pgdir, uint oldsz, uint newsz) {
    pte_t *pte;
    uint a, pa;

    if (newsz >= oldsz)
        return oldsz;

    a = PGROUNDUP(newsz);
    for (; a < oldsz; a += PGSIZE) {
        pte = walkpgdir(pgdir, (char*) a, 0);
        if (!pte)
            a = PGADDR(PDX(a) + 1, 0, 0) - PGSIZE;
        else if ((*pte & PTE_P) != 0) {
            pa = PTE_ADDR(*pte);
            if (pa == 0)
                panic("kfree");
            char *v = P2V(pa);
            kfree(v);
            *pte = 0;
        }
    }
    return newsz;
}


//------------------------------------------------------- My deallocuvm( ) procedure -----------------------------------------------

//--------------------------------------- My functions ----------------------------------

int
getIndexInPhysicalMem(char *va) {
    for (int i = 0; i < myproc()->pagingController.num_pages_in_physical_mem; i++) {
        char *got_va = (char *) myproc()->pagingController.physical_mem_pages[i].virtual_address;
        //        if (strncmp(va, got_va, sizeof (got_va)) == 0) {  //WHY NOT WORKS ??? COZ sizeof(..) is 4KB !!!
        if (va == got_va) {
            //Match found
            return i;
        }
    }
    return -1;
}
//--------------------------------------- My functions ----------------------------------

int
my_deallocuvm(pde_t *pgdir, uint oldsz, uint newsz) {
    pte_t *pte;
    uint a, pa;

    if (newsz >= oldsz)
        return oldsz;


    struct proc *proc = myproc();
#if DEBUG_PRINT >= 4
    cprintf("Inside my_deallocuvm( ) oldsz = %x, newsz = %x\n\n\n", (uint) oldsz, (uint) newsz);
#endif

    a = PGROUNDUP(newsz);
    for (; a < oldsz; a += PGSIZE) {
        pte = walkpgdir(pgdir, (char*) a, 0);

        if (!pte) { //Not exist in physical memory (OS)
            a = PGADDR(PDX(a) + 1, 0, 0) - PGSIZE;

        } else if (*pte & PTE_PG && proc->pgdir == pgdir) {
            //Searching for in swap file ...
#if DEBUG_PRINT >= 3
            cprintf("In deallocuvm(), pid = %d, a = %x, PTE_PG flag so searching in swap file\n", proc->pid, a);
#endif

            int idx_swap_file_found = -1;
            for (int i = 0; i < proc->pagingController.num_pages_in_swap_file; i++) {
                if (proc->pagingController.swapped_file_pages[i].virtual_address == (char *) a) {
                    //found
#if DEBUG_PRINT >= 3
                    cprintf("In deallocuvm(), pid = %d, a = %x, found in swap file idx = %d\n", proc->pid, a, i);
#endif
                    break;
                }
            }
            if (idx_swap_file_found != -1) {
                proc->pagingController.swapped_file_pages[idx_swap_file_found].virtual_address = (char *) 0; //Write as 0 to give the illusion of 'removing' from swap file
                proc->pagingController.num_pages_in_swap_file--;
            }
        } else if ((*pte & PTE_P) != 0) {
            pa = PTE_ADDR(*pte);
            if (pa == 0)
                panic("kfree");
            char *v = P2V(pa);
#if DEBUG_PRINT >= 4
            cprintf("Inside my_deallocuvm( ) for loop [physical mem search] a = %x, pte FOUND, v = P2V(pa) = %x, and pa = %x\n", a, (uint) v, (uint) pa);
#endif
            char *virtual_addr_accessed = (char *) a;

            //Searching in our physical pages array
            int idx_page_to_dealloc = getIndexInPhysicalMem(virtual_addr_accessed);
            if (idx_page_to_dealloc < 0) { // ???? Either empty queue or something else ... ok let's panic ???
                cprintf("In my_dealloc(), idx is NOT in physical memory !! Calling exit()");
                exit();
            }
            if (proc->pagingController.num_pages_in_physical_mem <= 0) {
                cprintf("\n\nIn my_dealloc(), trying to deallocate pages NOT in memory!!Calling exit()\n\n");
                exit();
            }
#if DEBUG_PRINT >= 4
            cprintf("-->> Index in physical mem found = %d\n", idx_page_to_dealloc);
#endif
            //1. Shrink the array ,Decrement physical pg count, head movement ... head--(if head!=0) and one ghor left shift shobaai
            int total_pages_phys = proc->pagingController.num_pages_in_physical_mem;
            for (int i = 0; i < idx_page_to_dealloc; i++) {
                //Do nothing
            }
            for (int i = idx_page_to_dealloc; i < (total_pages_phys - 1); i++) { //Left side e newa Page(i) := Page(i+1)
                proc->pagingController.physical_mem_pages[i].virtual_address = proc->pagingController.physical_mem_pages[i + 1].virtual_address;
            }
            proc->pagingController.physical_mem_pages[total_pages_phys - 1].virtual_address = (char *) 0; //Make the right most side 0[empty]

            if (proc->pagingController.index_to_replace != 0) { //if head != 0 then head -- 
                proc->pagingController.index_to_replace--;
            }

            proc->pagingController.index_array_physical_mem--;
            proc->pagingController.num_pages_in_physical_mem--;
#if DEBUG_PRINT >= 2
            cprintf("After dealloc() , printing page controller pid = %d\n", proc->pid);
            printPageController(proc);
#endif

            kfree(v);
            *pte = 0; //2. Make PTE_P flag or something
        }
    }
    return newsz;
}

// Free a page table and all the physical memory pages
// in the user part.

void
freevm(pde_t *pgdir) {
    uint i;

    if (pgdir == 0)
        panic("freevm: no pgdir");
    deallocuvm(pgdir, KERNBASE, 0);
    for (i = 0; i < NPDENTRIES; i++) {
        if (pgdir[i] & PTE_P) {
            char * v = P2V(PTE_ADDR(pgdir[i]));
            kfree(v);
        }
    }
    kfree((char*) pgdir);
}

// Clear PTE_U on a page. Used to create an inaccessible
// page beneath the user stack.

void
clearpteu(pde_t *pgdir, char *uva) {
    pte_t *pte;

    pte = walkpgdir(pgdir, uva, 0);
    if (pte == 0)
        panic("clearpteu");
    *pte &= ~PTE_U;
}

// Given a parent process's page table, create a copy
// of it for a child.

pde_t*
copyuvm(pde_t *pgdir, uint sz) {
    pde_t *d;
    pte_t *pte;
    uint pa, i, flags;
    char *mem;

    if ((d = setupkvm()) == 0)
        return 0;
    for (i = 0; i < sz; i += PGSIZE) {
        if ((pte = walkpgdir(pgdir, (void *) i, 0)) == 0)
            panic("copyuvm: pte should exist");
        if (!(*pte & PTE_P))
            panic("copyuvm: page not present");

//        if (*pte & PTE_PG) {
//            // cprintf("copyuvm PTR_PG\n"); // TODO delete
//            pte = walkpgdir(d, (void*) i, 1);
//            *pte = PTE_U | PTE_W | PTE_PG;
//            continue;
//        }
        
        pa = PTE_ADDR(*pte);
        flags = PTE_FLAGS(*pte);
        if ((mem = kalloc()) == 0)
            goto bad;
        memmove(mem, (char*) P2V(pa), PGSIZE);
        if (mappages(d, (void*) i, PGSIZE, V2P(mem), flags) < 0) {
            kfree(mem);
            goto bad;
        }
    }
    return d;

bad:
    freevm(d);
    return 0;
}

//PAGEBREAK!
// Map user virtual address to kernel address.

char*
uva2ka(pde_t *pgdir, char *uva) {
    pte_t *pte;

    pte = walkpgdir(pgdir, uva, 0);
    if ((*pte & PTE_P) == 0)
        return 0;
    if ((*pte & PTE_U) == 0)
        return 0;
    return (char*) P2V(PTE_ADDR(*pte));
}

// Copy len bytes from p to user address va in page table pgdir.
// Most useful when pgdir is not the current page table.
// uva2ka ensures this only works for PTE_U pages.

int
copyout(pde_t *pgdir, uint va, void *p, uint len) {
    char *buf, *pa0;
    uint n, va0;

    buf = (char*) p;
    while (len > 0) {
        va0 = (uint) PGROUNDDOWN(va);
        pa0 = uva2ka(pgdir, (char*) va0);
        if (pa0 == 0)
            return -1;
        n = PGSIZE - (va - va0);
        if (n > len)
            n = len;
        memmove(pa0 + (va - va0), buf, n);
        len -= n;
        buf += n;
        va = va0 + PGSIZE;
    }
    return 0;
}

//PAGEBREAK!
// Blank page.
//PAGEBREAK!
// Blank page.
//PAGEBREAK!
// Blank page.

//----------------------------------------------- My Swap Pages Functions ------------------------------------------

int
getIndexInSwapFile(char *va) {
    for (int i = 0; i < myproc()->pagingController.index_swapped_file; i++) {
        char *got_va = (char *) myproc()->pagingController.swapped_file_pages[i].virtual_address;
        //        if (strncmp(va, got_va, sizeof (got_va)) == 0) {  //WHY NOT WORKS ??? COZ sizeof(..) is 4KB !!!
        if (va == got_va) {
            //Match found
            return i;
        }
    }
    return -1;
}

int
swap_SecondChance(char *va) {
    int pid = myproc()->pid;
#if DEBUG_PRINT >= 3
    cprintf("Inside swapSecondChance() , va = %x, size_swapped_file = %d, pid = %d\n", va, myproc()->pagingController.index_swapped_file, myproc()->pid);
#endif

    //Should replace page

    //1st. Find in array of swap_pages this va ... index is found
    int idx_pg_swapped_file = getIndexInSwapFile(va);
    if (idx_pg_swapped_file == -1) {
        panic("In vm.c, swapSecondChance(), Page va = %x does not exist in swap file, calling exit() [in vm.c]\n");

    }
#if DEBUG_PRINT >= 5
    cprintf("Inside swapSecondChance() , va = %x, idx_swapped_file = %d, pid = %d\n", va, idx_pg_swapped_file, pid);
#endif

    //2nd. Let's read from file
    char read_from_file[PGSIZE]; //an empty array (buffer)
    memset(read_from_file, 0, sizeof (read_from_file)); //Fill with zeroes
    char *read_from_swapped_file = read_from_file;

    if (readFromSwapFile(myproc(), read_from_swapped_file, (idx_pg_swapped_file * PGSIZE), PGSIZE) < 0) {
        panic("In vm.c, swapSecondChance() , READING FROM SWAP FILE ERROR, calling exit()\n"); //Freezes the CPUs
    }

#if DEBUG_PRINT >= 5
    cprintf("Inside swapSecondChance() , va = %x, read_from_file = %x, pid = %d\n", va, read_from_file, pid);
#endif
    //3rd. Let's find the page in physical memory to swap according to FIFO logic (head)
    int idx_pg_physical_mem = myproc()->pagingController.index_to_replace;

    int head_idx = idx_pg_physical_mem;
    int queue_size = myproc()->pagingController.num_pages_in_physical_mem;
    while (1) {
        char *v_a_to_check = myproc()->pagingController.physical_mem_pages[head_idx].virtual_address;
        if (getAndRemoveReferenceBit(v_a_to_check) == 1) { //if R = 1, make R = 0, advance pointer
            head_idx = (head_idx + 1) % queue_size; //Circular queue                    
        } else { //R = 0 is found !!
            break;
        }

    }
    idx_pg_physical_mem = head_idx;


    if (idx_pg_physical_mem == MAX_PSYC_PAGES - 1) {
        myproc()->pagingController.index_to_replace = 0;
    } else {
        myproc()->pagingController.index_to_replace++;
    }
#if DEBUG_PRINT >= 5
    cprintf("Inside swapSecondChance() , prev head = %d, new head = %d, pid = %d\n", idx_pg_physical_mem, myproc()->pagingController.index_to_replace, pid);
#endif

    //4th. In swapped file, Write this idx_pg_physical_mem in the index of the above one
    char *toWriteBuffer_va = myproc()->pagingController.physical_mem_pages[idx_pg_physical_mem].virtual_address;
    if (writeToSwapFile(myproc(), toWriteBuffer_va, (idx_pg_swapped_file * PGSIZE), PGSIZE) < 0) {
        panic("In vm.c, swapSecondChance(), writeToSwapFile error\n"); //Freezes the CPUs
    }

#if DEBUG_PRINT >= 5
    cprintf("In swapSecondChance(), Reading from and Writing to swap file done, pid = %d\n", pid);
#endif

    //5th. Place the swapped page from swapped file in the physical memory (and data structure)

    //5.1 Updating data structure
    char *prev_virtual_addr = myproc()->pagingController.physical_mem_pages[idx_pg_physical_mem].virtual_address; //From physical memory pages array
    char *future_virtual_addr = va; //Function parameter
    myproc()->pagingController.swapped_file_pages[idx_pg_swapped_file].virtual_address = (char *) prev_virtual_addr;
    myproc()->pagingController.physical_mem_pages[idx_pg_physical_mem].virtual_address = (char *) future_virtual_addr;

    //5.2 Updating physical memory (PTE_P flags etc)
    pte_t *pte_prev_physical_mem;
    pte_prev_physical_mem = walkpgdir(myproc()->pgdir, prev_virtual_addr, 0);
    if (!pte_prev_physical_mem) {
        panic("PTE from physical mem is null\n");
    }
#if DEBUG_PRINT >= 5
    cprintf("In swapSecondChance(), Previously, Physical mem page walkpgdir done, pid = %d\n", pid);
#endif    

    pte_t *pte_from_swapped_file;
    pte_from_swapped_file = walkpgdir(myproc()->pgdir, (void*) future_virtual_addr, 0);
    if (!pte_from_swapped_file) {
        panic("PTE from swap file got is null\n");
    }

    //5.3 Replace IN the physical memory location and clear PTE_P (present) field of previous physical memory PTE

    *pte_from_swapped_file = PTE_ADDR(*pte_prev_physical_mem) | PTE_U | PTE_W | PTE_P; //Explicitly set Present bit
    *pte_prev_physical_mem = PTE_U | PTE_W | PTE_PG; //Clear the PRESENT bit

#if DEBUG_PRINT >= 5
    cprintf("In swapSecondChance(), abt to call lcr3 refresh TLB, pid = %d\n", pid);
#endif

    lcr3(V2P(myproc()->pgdir)); //Refresh TLB

#if DEBUG_PRINT >= 5
    cprintf("Returning from swapSecondChance(), pid = %d\n", pid);
#endif
    //printing the pageController
    printPageController(myproc());

    return 1;
}
//////////////////////////////////////////////////// SWAP FIFO SC ends /////////////////////////////////////////////////////////////

//////////////////////////////////////////////////// SWAP FIFO  begins /////////////////////////////////////////////////////////////

int
swap_FIFO(char *va) {
    int pid = myproc()->pid;
#if DEBUG_PRINT >= 3
    cprintf("Inside swap_FIFO() , va = %x, size_swapped_file = %d, pid = %d\n", va, myproc()->pagingController.index_swapped_file, myproc()->pid);
#endif
    //Should replace page

    //1st. Find in array of swap_pages this va ... index is found
    int idx_pg_swapped_file = getIndexInSwapFile(va);
    if (idx_pg_swapped_file == -1) {
        panic("In vm.c, swap_FIFO(), Page va = %x does not exist in swap file, calling exit() [in vm.c]\n");

    }
#if DEBUG_PRINT >= 5
    cprintf("Inside swap_FIFO() , va = %x, idx_swapped_file = %d, pid = %d\n", va, idx_pg_swapped_file, pid);
#endif

    //2nd. Let's read from file
    char read_from_file[PGSIZE]; //an empty array (buffer)
    memset(read_from_file, 0, sizeof (read_from_file)); //Fill with zeroes
    char *read_from_swapped_file = read_from_file;

    if (readFromSwapFile(myproc(), read_from_swapped_file, (idx_pg_swapped_file * PGSIZE), PGSIZE) < 0) {
        panic("In vm.c, swap_FIFO() , READING FROM SWAP FILE ERROR, calling exit()\n"); //Freezes the CPUs
    }

#if DEBUG_PRINT >= 5
    cprintf("Inside swap_FIFO() , va = %x, read_from_file = %x, pid = %d\n", va, read_from_file, pid);
#endif
    //3rd. Let's find the page in physical memory to swap according to FIFO logic (head)
    int idx_pg_physical_mem = myproc()->pagingController.index_to_replace;

    if (idx_pg_physical_mem == MAX_PSYC_PAGES - 1) {
        myproc()->pagingController.index_to_replace = 0;
    } else {
        myproc()->pagingController.index_to_replace++;
    }
#if DEBUG_PRINT >= 5
    cprintf("Inside swap_FIFO() , prev head = %d, new head = %d, pid = %d\n", idx_pg_physical_mem, myproc()->pagingController.index_to_replace, pid);
#endif

    //4th. In swapped file, Write this idx_pg_physical_mem in the index of the above one
    char *toWriteBuffer_va = myproc()->pagingController.physical_mem_pages[idx_pg_physical_mem].virtual_address;
    if (writeToSwapFile(myproc(), toWriteBuffer_va, (idx_pg_swapped_file * PGSIZE), PGSIZE) < 0) {
        panic("In vm.c, swapFIFO(), writeToSwapFile error\n"); //Freezes the CPUs
    }

#if DEBUG_PRINT >= 5
    cprintf("In swapFifo(), Reading from and Writing to swap file done, pid = %d\n", pid);
#endif

    //5th. Place the swapped page from swapped file in the physical memory (and data structure)

    //5.1 Updating data structure
    char *prev_virtual_addr = myproc()->pagingController.physical_mem_pages[idx_pg_physical_mem].virtual_address; //From physical memory pages array
    char *future_virtual_addr = va; //Function parameter
    myproc()->pagingController.swapped_file_pages[idx_pg_swapped_file].virtual_address = (char *) prev_virtual_addr;
    myproc()->pagingController.physical_mem_pages[idx_pg_physical_mem].virtual_address = (char *) future_virtual_addr;

    //5.2 Updating physical memory (PTE_P flags etc)
    pte_t *pte_prev_physical_mem;
    pte_prev_physical_mem = walkpgdir(myproc()->pgdir, prev_virtual_addr, 0);
    if (!pte_prev_physical_mem) {
        panic("PTE from physical mem is null\n");
    }
#if DEBUG_PRINT >= 5
    cprintf("In swapFifo(), Previously, Physical mem page walkpgdir done, pid = %d\n", pid);
#endif    

    pte_t *pte_from_swapped_file;
    pte_from_swapped_file = walkpgdir(myproc()->pgdir, (void*) future_virtual_addr, 0);
    if (!pte_from_swapped_file) {
        panic("PTE from swap file got is null\n");
    }

    //5.3 Replace IN the physical memory location and clear PTE_P (present) field of previous physical memory PTE

    *pte_from_swapped_file = PTE_ADDR(*pte_prev_physical_mem) | PTE_U | PTE_W | PTE_P; //Explicitly set Present bit
    *pte_prev_physical_mem = PTE_U | PTE_W | PTE_PG; //Clear the PRESENT bit

#if DEBUG_PRINT >= 5
    cprintf("In swapFIFO(), abt to call lcr3 refresh TLB, pid = %d\n", pid);
#endif

    lcr3(V2P(myproc()->pgdir)); //Refresh TLB

#if DEBUG_PRINT >= 5
    cprintf("Returning from swapFIFO(), pid = %d\n", pid);
#endif
    //printing the pageController
    printPageController(myproc());

    return 1;
}



//----------------------------------------------- My Swap Pages Functions end ------------------------------------------
