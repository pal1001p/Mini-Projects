// Starting code version 1.1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>


#include "mmu.h"
#include "pagetable.h"
#include "memsim.h"


int pageToEvict = 1; //if using round robin eviction, this can be used to keep track of the next page to evict

#define SWAPSLOTS 32
int swapSlots[SWAPSLOTS]; // 0 = free, 1 = no

// Page table root pointer register values (one stored per process, would be "swapped in" to MMU with process, when scheduled)
typedef struct{
	int valid; //1 if exists, 0 if not
    int present; //1 if in physical, 0 if swapped
	int ptStartPA;
    int swapSlot;
} ptRegister;

// Page table root pointer register values 
// One stored for each process, swapped in to MMU when process is scheduled to run)
ptRegister ptRegVals[NUM_PROCESSES]; 

// Struct for a page table entry
typedef struct {
	unsigned int valid : 1;
	unsigned int protection: 1;
	unsigned int present : 1;
	unsigned int referenced : 1;
	unsigned int others: 4;
	uint8_t VPN;                
    uint8_t PFN; 
} PTE;

/*
 * Public Interface:
 */

/*
 * Sets the Page Table Entry (PTE) for the given process and VPN.
 * The PTE contains the PFN, valid bit, protection bit, present bit, and referenced bit.
 */
 void PT_SetPTE(int pid, int VPN, int PFN, int valid, int protection, int present, int referenced) {
    char* physmem = Memsim_GetPhysMem();
	// ensure PT exists
    assert(PT_PageTableExists(pid)); 

	// get starting address of PT
    int ptStartPA = PT_GetRootPtrRegVal(pid);
    if (ptStartPA == -1) {
        printf("Error: Page table for PID %d could not be swapped in.\n", pid);
        return;
    } 
    int offset = VPN * 3; // 3 bytes per entry

	// ensure PTE will fit within the PT
    if (ptStartPA + offset + 3 > ptStartPA + PAGE_SIZE) {
        printf("PTE exceeds page table!\n");
        return;
    }

    // organize all the informational bits
	// move each subsequent one (more) bit to the left
	uint8_t flags = 0;
    flags |= (valid & 0x01) << 0;      
    flags |= (protection & 0x01) << 1; 
    flags |= (present & 0x01) << 2;   
    flags |= (referenced & 0x01) << 3; 

    // put the PTE into physical memory, with each consecutive cell taking up a byte
    physmem[ptStartPA + offset] = flags;      
    physmem[ptStartPA + offset + 1] = VPN;    
    physmem[ptStartPA + offset + 2] = PFN;    
    //printf("Setting PTE for PID %d, VPN %d: valid=%d, protection=%d, present=%d, referenced=%d\n",pid, VPN, valid, protection, present, referenced);
}
/* 
 * Initializes the page table for the process, and sets the starting physical address for the page table.
 * 
 * After initialization, get the next free page in physical memory.
 * If there are no free pages, evict a page to get a newly freed physical address. //I THINK I HANDLED THIS IN MAP?
 * Finally, return the physical address of the next free page.
 */
 int PT_PageTableInit(int pid, int pa) {
    char* physmem = Memsim_GetPhysMem();

    if (PT_PageTableExists(pid)) {
        printf("Error: Page table for PID %d already exists.\n", pid);
        return -1;
    }

    // zero out the physical frame to be used
    for (int i = 0; i < PAGE_SIZE; i++) {
        physmem[pa + i] = 0;
    }

    // set the PT's root pointer register values; it is now present/valid
	// and we have the starting physical address of the page table within physical memory
    ptRegVals[pid].valid = 1; //now exists
    ptRegVals[pid].present = 1; //now in memory
    ptRegVals[pid].ptStartPA = pa;  
    ptRegVals[pid].swapSlot = -1; //not swapped

	// return the physical address of the PT
    return pa;
}
/* 
 * Check the ptRegVals to see if there is a valid starting PA for the given PID's page table.
 * Returns true (non-zero) or false (zero).
 */
 int PT_PageTableExists(int pid){
 	return ptRegVals[pid].valid; 
 }

 /* 
 * Check if the page table for the given PID is in memory.
 * Returns true (non-zero) if the page table is in memory or false (zero) otherwise.
 */
int PT_PageTableIsPresent(int pid) {
    return ptRegVals[pid].present;
}

/*
 * Swaps in a page table if VPN is called with -1
 * Swaps in a page otherwise
 * Returns 0 on success, -1 otherwise
 */
int PT_SwapIn(int pid, int VPN) {
    char* physmem = Memsim_GetPhysMem();
    FILE* swapFile = MMU_GetSwapFileHandle();

    if (VPN == -1) {
        // swap in a PT when function called with -1
        int freeFrame = Memsim_FirstFreePFN();
        if (freeFrame == -1) {
            if (PT_Evict() == -1) {
                printf("Error: No free frames after eviction for a PT.\n");
                return -1;
            }
            // try again after eviction
            freeFrame = Memsim_FirstFreePFN();
            if (freeFrame == -1) {
                return -1;
            }
        }

        int swapSlot = ptRegVals[pid].swapSlot;
        if (swapSlot == -1) {
            printf("Error: No swap slot for PID %d.\n", pid);
            return -1;
        }

        // read PT from swap file into free frame
        fseek(swapFile, swapSlot * PAGE_SIZE, SEEK_SET);
        fread(&physmem[freeFrame], 1, PAGE_SIZE, swapFile);

        // update registers (it should now be in memoery)
        ptRegVals[pid].present = 1;
        ptRegVals[pid].ptStartPA = freeFrame;
        ptRegVals[pid].swapSlot = -1;


        // frame is now used
        freePages[PFN(freeFrame)] = 1;

        printf("Swapped disk offset %d into Frame %d.\n", swapSlot * PAGE_SIZE, PFN(freeFrame));
        return 0;
    } else {
        // swap in a normal page otherwise
        // ensure the page table is present (bc we will need to update the PTE)
        int ptStartPA = PT_GetRootPtrRegVal(pid);
        if (ptStartPA == -1) {
            printf("Error: Failed to access page table for PID %d.\n", pid);
            return -1;
        }

        // get offset, other info
        int offset = VPN * 3;
        uint8_t flags = physmem[ptStartPA + offset];
        uint8_t storedVPN = physmem[ptStartPA + offset + 1];
        uint8_t swapSlot = physmem[ptStartPA + offset + 2]; // swap was stored here previously

        // is page already present?
        if (flags & (1 << 2)) { 
            printf("Error: Page %d for PID %d is already in memory.\n", VPN, pid);
            return -1;
        }

        // check swap slot
        if (swapSlot < 0 || swapSlot >= swapSlots || swapSlots[swapSlot] == 0) {
            printf("Error: Bad swap slot %d for PID %d, VPN %d.\n", swapSlot, pid, VPN);
            return -1;
        }

        // find free frame
        int freeFrame = Memsim_FirstFreePFN();
        if (freeFrame == -1) {
            if (PT_Evict() == -1) {
                printf("Error: No free frames after eviction for a page.\n");
                return -1;
            }
            // try again after eviction
            freeFrame = Memsim_FirstFreePFN();
            if (freeFrame == -1){
                 return -1;
            }
        }

        // read data into memory
        fseek(swapFile, swapSlot * PAGE_SIZE, SEEK_SET);
        fread(&physmem[freeFrame], 1, PAGE_SIZE, swapFile);

        // update PTE
        flags |= (1 << 2); // update present bit
        physmem[ptStartPA + offset] = flags;
        physmem[ptStartPA + offset + 2] = PFN(freeFrame); // update PFN to new frame

        // frame is used
        freePages[PFN(freeFrame)] = 1;

        printf("Swapped disk offset %d into Frame %d.\n", swapSlot * PAGE_SIZE, PFN(freeFrame));
        return 0;
    }
}

/* 
 * Returns the starting physical address of the page table for the given PID.
 * If the page table does not exist, returns -1.
 * If the page table is not in memory, first swaps it in to physical memory.
 * Finally, returns the starting physical address of the page table.
 */
 int PT_GetRootPtrRegVal(int pid) {
    // return if page table doesn't exist
    if (!PT_PageTableExists(pid)){
         return -1;
    }

    // try to swap in the table if it's not present but exists
    if (!PT_PageTableIsPresent(pid)) {
        if (PT_SwapIn(pid, -1) == -1) {
            printf("Failed to swap in PT for PID %d\n", pid);
            return -1;
        }
    }

    return ptRegVals[pid].ptStartPA;
}
/*
 * Gets a swap slot on the disk.
 * Iterates through the array of swap slots until it finds one that isn't marked and returns that as the one to be used
 */
int getswapSlot() {
    for (int i = 0; i < swapSlots; i++) {
        if (swapSlots[i] == 0) {
            swapSlots[i] = 1; 
            return i; 
        }
    }
    return -1; 
}

/*
 * Evicts the next page. 
 * Updates the corresponding information in the page table, returns the PA of the evicted page.
 * 
 * The supplied input and output used in autotest.sh *RR tests, uses the round-robin algorithm. 
 * You may also implement the simple and powerful Least Recently Used (LRU) policy, 
 * or another fair algorithm.
 */
 int PT_Evict() {
    char* physmem = Memsim_GetPhysMem();
    FILE* swapFile = MMU_GetSwapFileHandle();
    int originalPageToEvict = pageToEvict;
    int framePA;

    // ensures we can loop through all frames (safeguard for when a PT gets evicted)
    for (int i = 0; i < NUM_PAGES; i++) {
        int currentFrame = (pageToEvict + i) % NUM_PAGES;

        // empty the current frame if it is occupied
        if(freePages[currentFrame] == 1) {
            framePA = currentFrame * PAGE_SIZE;
            // is frame a page table?
            for (int pid = 0; pid < NUM_PROCESSES; pid++) {
                if (ptRegVals[pid].present && ptRegVals[pid].ptStartPA == framePA) {
                    int swapSlot = getswapSlot();
                    if (swapSlot == -1) {
                        return -1;
                    }
                    int offset = swapSlot * PAGE_SIZE;

                    // write to disk
                    fseek(swapFile, offset, SEEK_SET);
                    fwrite(&physmem[framePA], 1, PAGE_SIZE, swapFile);
                    fflush(swapFile);

                    // update PT info
                    ptRegVals[pid].present = 0;
                    ptRegVals[pid].swapSlot = swapSlot;
                    freePages[currentFrame] = 0;

                    printf("Swapped Frame %d to disk at offset %d.\n", currentFrame, offset);
                    // update hand to point at next page to evict
                    pageToEvict = (currentFrame + 1) % NUM_PAGES;
                    return framePA;
                }
            }

            // is frame a page?
            for (int pid = 0; pid < NUM_PROCESSES; pid++) {
                if (PT_PageTableExists(pid) && PT_PageTableIsPresent(pid)) {
                    // ensure PT is bought into memory
                    int ptStartPA = PT_GetRootPtrRegVal(pid);
                    // iterate over all the possible page table entries to look for a match
                    // where a stored pfn will be equal to the frame that needs to be evicted
                    // thus, we know which page table entry to update
                    for (int vpn = 0; vpn < 4; vpn++) {
                        int offset = vpn * 3;
                        uint8_t pfn = physmem[ptStartPA + offset + 2];
                        
                        //match found
                        if (pfn == currentFrame) {
                            int swapSlot = getswapSlot();
                            if (swapSlot == -1){
                                 return -1;
                            }
                            int swapOffset = swapSlot * PAGE_SIZE;

                            // write to disk
                            fseek(swapFile, swapOffset, SEEK_SET);
                            fwrite(&physmem[currentFrame * PAGE_SIZE], 1, PAGE_SIZE, swapFile);
                            fflush(swapFile);

                            // update PTE
                            uint8_t flags = physmem[ptStartPA + offset];
                            flags &= ~(1 << 2);
                            physmem[ptStartPA + offset] = flags;
                            physmem[ptStartPA + offset + 2] = swapSlot;

                            freePages[currentFrame] = 0;
                            printf("Swapped Frame %d to disk at offset %d.\n", currentFrame, swapOffset);
                            // update hand to point at next page to evict
                            pageToEvict = (currentFrame + 1) % NUM_PAGES;
                            return framePA;
                        }
                }
            }
        }
        // update just in case
        pageToEvict = (currentFrame + 1) % NUM_PAGES;
    }
}

    printf("SHOULDN'T REACH: No frames available to evict.\n");
    return -1;
}


/*
 * Searches through the process's page table. If an entry is found containing the specified VPN, 
 * return the address of the start of the corresponding physical page frame in physical memory. 
 *
 * If the physical page is not present, first swaps in the phyical page from the physical disk,
 * and returns the physical address.
 * 
 * Otherwise, returns -1.
 */
 int PT_VPNtoPA(int pid, int VPN) {
    char *physmem = Memsim_GetPhysMem();

    if (!PT_PageTableExists(pid)) {
		//printf("Page table does not yet exist! (pagetable.c)\n");
        return -1; // PT does not exist (gets handled by map)
    }

    // ensure the page table is in memory
    int ptStartPA = PT_GetRootPtrRegVal(pid);
    if (ptStartPA == -1) {
        return -1; // page table could not be swapped in
    }

    ptStartPA = ptRegVals[pid].ptStartPA;
    int offset = VPN * 3; // 3 bytes per entry

    // find the PTE in the PT using the starting physical address of the PT and the offset
    uint8_t flags = physmem[ptStartPA + offset];
    uint8_t vpn = physmem[ptStartPA + offset + 1];
    uint8_t pfn = physmem[ptStartPA + offset + 2];

	// set up the PTE that we are looking at right now
    PTE pte;
    pte.valid = (flags >> 0) & 0x01;
    pte.protection = (flags >> 1) & 0x01;
    pte.present = (flags >> 2) & 0x01;
    pte.referenced = (flags >> 3) & 0x01;

    if (!pte.valid) {
        //printf("PTE is not valid in PT_VPNtoPA\n");
        return -1; 
    }

    if (!pte.present) {
        // page is not present, so swap it in
        if (PT_SwapIn(pid, VPN) == -1) {
            printf("Failed to swap in in PT_VPNtoPA\n");
            return -1; 
        }
        // retry after swapping in
        // swap in function should probably set present bit to 0
        flags = physmem[ptStartPA + offset];
        pfn = physmem[ptStartPA + offset + 2];
    }


    return pfn * PAGE_SIZE;
}

/*
 * Finds the page table entry corresponding to the VPN, and checks
 * to see if the protection bit is set to 1 (readable and writable).
 * If it is 1, it returns TRUE, and FALSE if it is not found or is 0.
 */
int PT_PIDHasWritePerm(int pid, int VPN) {
    char* physmem = Memsim_GetPhysMem();
    
    // PT should be in memory
    int ptStartPA = PT_GetRootPtrRegVal(pid);
    if (ptStartPA == -1) {
        // page table didnt get bought into memory. bad
        printf("Error: Page table for PID %d is not in memory.\n", pid);
        return 0; 
    }

    int offset = VPN * 3; 
    uint8_t flags = physmem[ptStartPA + offset];
    //printf("The permission for PID %d, VPN %d is: protection bit=%d\n",pid, VPN, (flags >> 1) & 0x01);

    // if present bit is 1, return true
    return (flags >> 1) & 0x01;
}

/* 
 * Initialize the register values for each page table location (per process).
 * For example, -1 for the starting physical address of the page table, and FALSE for present.
 */
 void PT_Init() {
    for(int i = 0; i < NUM_PROCESSES; i++){
        ptRegVals[i].valid = 0;
        ptRegVals[i].present = -1;
        ptRegVals[i].ptStartPA = -1;
        ptRegVals[i].swapSlot = -1;
    }
}
