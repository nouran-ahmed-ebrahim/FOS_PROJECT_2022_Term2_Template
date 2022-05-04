#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//2022: NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)

void* lastAllocated = (void *)KERNEL_HEAP_START;


struct allocation{
	void* allocationAddres;
	struct Frame_Info* allocationframe;
	uint32 numOfPages;
}allocations[40960];

int getAllocationNumber(void* address)
{
	return (address - (void *)KERNEL_HEAP_START)/PAGE_SIZE;
}

int countEmptySize(void * address , uint32 wantedSize)
{
	uint32* pageTable = NULL;
	uint32 numOfEmptyFrames = 0;
	for(void * i = address ; i< (void *)KERNEL_HEAP_MAX; i+=PAGE_SIZE)
	{
		get_page_table(ptr_page_directory,i,&pageTable);
        if(pageTable != NULL && (pageTable[PTX(i)] & 0x001))
          {
        	return numOfEmptyFrames;
          }

        numOfEmptyFrames++;

        if(wantedSize == numOfEmptyFrames)
        {
        	return numOfEmptyFrames;
        }
	}

	return numOfEmptyFrames;
}

void* findSuitableEmptyBlock(void* startAddress, int numOfPages)
{
	uint32* pageTable = NULL;
	uint32 blockSize;
    for(void* i = startAddress ; i < (void *)KERNEL_HEAP_MAX ;)
       {
    	  get_page_table(ptr_page_directory,i,&pageTable);
          if(pageTable != NULL && (pageTable[PTX(i)] & 0x001))
            {
        	   i+=PAGE_SIZE;
               continue;
            }
          blockSize = countEmptySize(i, numOfPages);
          if(numOfPages == blockSize)
          {
        	  return i;
          }
       	  i+=PAGE_SIZE * blockSize;
       }
	return NULL;
}

void deallocate(void * address)
{
   int allocationIdx = getAllocationNumber(address);

   int lastAllocation = allocations[allocationIdx].numOfPages + allocationIdx;
   for(int i = allocationIdx ; i < lastAllocation ;i++)
   {
	   unmap_frame(ptr_page_directory, allocations[i].allocationAddres);
	   free_frame(allocations[i].allocationframe);

	   allocations[allocationIdx].allocationAddres = NULL;
	   allocations[allocationIdx].allocationframe = NULL;
	   allocations[allocationIdx].numOfPages = 0;
   }

}

void* allocatePages(uint32 numOfPages, void* allocationAdd)
{
	int allocationIdx;
	uint32 numOfAllocatedPages;
	int ret;
	int allocatedAll = 1;
	struct Frame_Info *ptr_frame_info;
	void* i = allocationAdd;

	for(uint32 j = 0 ; j <numOfPages ;i+=PAGE_SIZE, j++)
	{
		ret = allocate_frame(&ptr_frame_info);
		if (ret == E_NO_MEM)
		{
			i-=PAGE_SIZE;
			allocatedAll = 0;
	        break;
		}
		ret = map_frame(ptr_page_directory, ptr_frame_info, i, (0x002 | 0x0001));
		if (ret == E_NO_MEM)
		   {
			  allocatedAll = 0;
			  break;
		   }
		allocationIdx = getAllocationNumber(i);
		allocations[allocationIdx].allocationAddres = i;
		allocations[allocationIdx].allocationframe = ptr_frame_info;
		if(j == 0)
		{
			allocations[allocationIdx].numOfPages = numOfPages;
		}
	}

	if(allocatedAll == 0)
	{
		numOfAllocatedPages = (i - allocationAdd )/PAGE_SIZE ;
		if(numOfAllocatedPages !=0)
		{
		 deallocate(allocationAdd);
		}
		return NULL;
	}

	lastAllocated += PAGE_SIZE * numOfPages;
	return (allocationAdd);
}

void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT 2022 - [1] Kernel Heap] kmalloc()
	// Write your code here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//NOTE: Allocation using NEXTFIT strategy
	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	size = ROUNDUP(size,PAGE_SIZE);
    uint32 numOfPages = size / PAGE_SIZE;
	void* allocationAdd = findSuitableEmptyBlock(lastAllocated, numOfPages);

	if(allocationAdd == NULL)
	{
		allocationAdd = findSuitableEmptyBlock((uint32*)KERNEL_HEAP_START, numOfPages);
	}

	if(allocationAdd == NULL)
	{
	    return NULL;
	}

	return allocatePages(numOfPages, allocationAdd);




	//TODO: [PROJECT 2022 - BONUS1] Implement a Kernel allocation strategy
	// Instead of the Next allocation/deallocation, implement
	// BEST FIT strategy
	// use "isKHeapPlacementStrategyBESTFIT() ..."
	// and "isKHeapPlacementStrategyNEXTFIT() ..."
	//functions to check the current strategy
	//change this "return" according to your answer

}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT 2022 - [2] Kernel Heap] kfree()
	// Write your code here, remove the panic and write your code
	//panic("kfree() is not implemented yet...!!");
	//you need to get the size of the given allocation using its address
	//refer to the project presentation and documentation for details
    //cprintf("%x\n",virtual_address);
	deallocate(virtual_address);cprintf("bb");
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT 2022 - [3] Kernel Heap] kheap_virtual_address()
	// Write your code here, remove the panic and write your code
	panic("kheap_virtual_address() is not implemented yet...!!");

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer

	return 0;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT 2022 - [4] Kernel Heap] kheap_physical_address()
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer
	 uint32 * pageTable1;
	 uint32 off;
	 pageTable1 = NULL;
	 get_page_table(ptr_page_directory,(const void *) virtual_address,&pageTable1);

     if(pageTable1 !=NULL)
	   {
     	 uint32 frameNum = pageTable1[PTX(virtual_address)];
     	 if(frameNum & PERM_PRESENT)
    	 {
    		 return -1;
    	 }
         frameNum >>= 12;
    	 frameNum <<=12;

    	 off = (virtual_address<<20);
    	 off>>=20;
 	        return  frameNum + off  ;
	   }
	return -1;
}

