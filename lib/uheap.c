
#include <inc/lib.h>

// malloc()
//	This function use NEXT FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.


//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

int num_of_pages = (USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE;
int userHeap[131072];
int lastAllocated = 0;

int Get_Index(void* Address)
{
	int index ;
	index = (Address-(void *)USER_HEAP_START)/PAGE_SIZE;
	return index;
}

void* Get_add (int index)
{
	void* add ;
	add = (index*PAGE_SIZE)+(void *)USER_HEAP_START;
	return add;
}
void* malloc(uint32 size)
{
	void* virtual_address;
	int pages;
	int count = 0;
	int counter_of_userHeap = 0;
	size = ROUNDUP(size,PAGE_SIZE);
	pages = size/PAGE_SIZE;
	bool once = 0;
	//uint32* ptrCount = 0;
	for(int i = lastAllocated; i < 131072;i++)
	{
		if(userHeap[i] != 0)
		{
			i+=userHeap[i]-1;
			continue;
		}

		int count = 0;
		for(int j = i ; j < pages+i && j<131072 ; j++)
		{

			if(userHeap[j]== 0)
			{
				count++;
			}else {
				i+=userHeap[j]-1;
				break;
			}
		}

		if(count == pages)
		{
			virtual_address = Get_add(i);
			cprintf("%d %x\n",i, virtual_address);
			sys_allocateMem((uint32)virtual_address, pages);
			userHeap[i]=pages;
			i+=pages;
			lastAllocated = i;
			break;
		}

		if((i == 131071) & (once == 0))
		{
			i = 0;
			once = 1;

		}
	}

	return virtual_address;
	//TODO: [PROJECT 2022 - [9] User Heap malloc()] [User Side]
	// Write your code here, remove the panic and write your code
	//panic("malloc() is not implemented yet...!!");

	// Steps:
	//	1) Implement NEXT FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_allocateMem to invoke the Kernel for allocation
	// 	4) Return pointer containing the virtual address of allocated space,
	//

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyNEXTFIT() and
	//sys_isUHeapPlacementStrategyBESTFIT() for the bonus
	//to check the current strategy

}

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	panic("smalloc() is not required ..!!");
	return NULL;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	panic("sget() is not required ..!!");
	return 0;
}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2022 - [11] User Heap free()] [User Side]
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");

	//you shold get the size of the given allocation using its address
	//you need to call sys_freeMem()
	//refer to the project presentation and documentation for details
	int idx = Get_Index(virtual_address);
	int size = userHeap[idx];

	userHeap[idx] = 0;
	sys_freeMem((uint32)virtual_address,size);
}


void sfree(void* virtual_address)
{
	panic("sfree() is not requried ..!!");
}


//===============
// [2] realloc():
//===============

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void *realloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT 2022 - BONUS3] User Heap Realloc [User Side]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");

	return NULL;
}
