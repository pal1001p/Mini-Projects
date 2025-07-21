// Starting code version 1.0

#include <stdio.h>
#include <stdint.h>

#include "memsim.h"
#include "pagetable.h"
#include "mmu.h"

/*
 * Searches the memory for a free page, and assigns it to the process's virtual address. If value is
 * 0, the page has read only permissions, and if it is 1, it has read/write permissions.
 * If the process does not already have a page table, one is assigned to it. If there are no more empty
 * pages, the mapping fails.
 */
int Instruction_Map(int pid, int va, int value_in) {
    int pa;
    char* physmem = Memsim_GetPhysMem();

    // check process id
    if (pid > 3 || pid < 0) {
        printf("Process id %d is invalid.\n", pid);
        return 1;
    }

    // check value_in
    if (value_in != 0 && value_in != 1) {
        printf("Invalid value for map instruction. Value must be 0 or 1.\n");
        return 1;
    }

    // check virtual addr
    if (va > 63 || va < 0) {
        printf("Provided virtual address %d is invalid.\n", va);
        return 1;
    }

    int vpn = VPN(va); 

    // is vpn already mapped?
    if (PT_PageTableExists(pid)) {
        // PT must be swapped in
        if (PT_PageTableIsPresent(pid) == 0) {
            if (PT_SwapIn(pid, -1) == -1) {
                printf("Error: Failed to swap in page table for PID %d.\n", pid);
                return 1;
            }
        }

        int ptStartPA = PT_GetRootPtrRegVal(pid);
        int offset = vpn * 3;

        uint8_t flags = physmem[ptStartPA + offset];
        int valid = (flags >> 0) & 0x01;
		// if it's already mapped, may need to update permissions (if not same virtual page with same permissions)
        if (valid) {
            int protection = (flags >> 1) & 0x01;
            if (protection == value_in) {
                printf("Error: virtual page %d is already mapped with rw_bit=%d.\n", vpn, value_in);
                return 0;
            } else {
                // permissions not same so..
                // update permissions 
                uint8_t pfn = physmem[ptStartPA + offset + 2];
                int present = (flags >> 2) & 0x01;
                int referenced = (flags >> 3) & 0x01;

                PT_SetPTE(pid, vpn, pfn, 1, value_in, present, referenced);
                printf("Updating permissions for virtual page %d (frame %d)\n", vpn, pfn);
                return 0;
            }
        }
    }

    // create PT if it doesn't exist
    if (!PT_PageTableExists(pid)) {
        int ptFrame = Memsim_FirstFreePFN();
        if (ptFrame == -1) {
            if (PT_Evict() == -1) {
                printf("Couldn't evict a page in map function.\n");
                return 1;
            }
            ptFrame = Memsim_FirstFreePFN();
        }

        if (PT_PageTableInit(pid, ptFrame) == -1) {
            printf("Failed to initialize page table for PID %d.\n", pid);
            return 1;
        }
        printf("Put page table for PID %d into physical frame %d.\n", pid, PFN(ptFrame));
    }

    // PT must be in memory
    if (PT_PageTableExists(pid) && PT_PageTableIsPresent(pid) == 0) {
        if (PT_SwapIn(pid, -1) == -1) {
            printf("Error: Failed to swap in page table for PID %d.\n", pid);
            return 1;
        }
    }

    // get a new frame
    int freeFrame = Memsim_FirstFreePFN();
    if (freeFrame == -1) {
        if (PT_Evict() == -1) {
            printf("Couldn't evict a page in map function.\n");
            return 1;
        }
        freeFrame = Memsim_FirstFreePFN();
    }

    // update new PTE
    PT_SetPTE(pid, vpn, PFN(freeFrame), 1, value_in, 1, 0);
    printf("Mapped virtual address %d (page %d) into physical frame %d.\n", va, vpn, PFN(freeFrame));
    return 0;
}


/**
* If the virtual address is valid and has write permissions for the process, store
* value in the virtual address specified.
*/
int Instruction_Store(int pid, int va, int value_in){
	int pa;
	char* physmem = Memsim_GetPhysMem();
	// check if pid is valid
	if (pid > 3 || pid < 0){
		printf("Process id %d is invalid.\n", pid);
	}
	//check for a valid value (instructions validate the value_in)
	if (value_in < 0 || value_in > MAX_VA) { 
		printf("Invalid value for store instruction. Value must be 0-255.\n"); 
		return 1;
	}
	// check if virtual address is valid
	if(va > 63 || va < 0){
		printf("Provided virtual address %d is invalid.\n", va);
	}
	// check if memory is writable
	if (!PT_PIDHasWritePerm(pid, VPN(va))) { 
		printf("Error: virtual address %d does not have write permissions.\n", va);
		return 1;
	}
	// translate the virtual address into its physical address for the process
	int offset = va % PAGE_SIZE;
	pa = MMU_TranslateAddress(pid,VPN(va),offset);
	// if translation failed, might have to swap in page
	if (pa == -1) {
		if(PT_SwapIn(pid, VPN(va)) ==-1){
			printf("Error: Failed to swap in page for virtual address in store function%d.\n", va);
            return 1;
    	}
	}
	//retry
	pa = MMU_TranslateAddress(pid, VPN(va), offset);
		if (pa == -1) {
			printf("Error: Failed to translate for virtual address %d in store function (2ND TIME).\n", va);
            return 1;
    	}
	// update the byte in physical memory to store the value
	physmem[pa] = value_in;

	printf("Stored value %u at virtual address %d (physical address %d)\n", value_in, va, pa);

	return 0;
	}

/*
 * Translate the virtual address into its physical address for
 * the process. If the virutal memory is mapped to valid physical memory, 
 * return the value at the physical address. Permission checking is not needed,
 * since we assume all processes have (at least) read permissions on pages.
 */
int Instruction_Load(int pid, int va){
	int pa;
	char* physmem = Memsim_GetPhysMem(); 

	// check if virtual address is valid
	if(va > 63 || va < 0){
		printf("Provided virtual address %d is invalid.\n", va);
	}
	// check if pid is valid
	if (pid > 3 || pid < 0){
		printf("Process %d is invalid.\n", pid);
	}

	// check if PT exists
	if (!PT_PageTableExists(pid)) {
        printf("Page table for process %d does not exist.\n", pid);
        return 1; 
    }

	// find necessary offset into page
	// translate the virtual address to the physical address
	int offset = va % PAGE_SIZE;
	pa = MMU_TranslateAddress(pid,VPN(va),offset);

	// page might not be in memory, so swap in
	if (pa == -1) {
		if(PT_SwapIn(pid, VPN(va)) == -1){
        	printf("Error: couldnt swap in for virtual address %d in load function.\n", va);
        	return 1; 
    	}
		pa = MMU_TranslateAddress(pid, VPN(va), offset);
		if (pa == -1) {
        	printf("Error: couldnt translate for virtual address %d in load function (2ND TIME).\n", va);
        	return 1; 
    	}
	}

	// this value would be copied to the user program's register
	uint8_t value = physmem[pa];
	printf("The value %u was found at virtual address %d.\n", value, va);
	return 0;

}