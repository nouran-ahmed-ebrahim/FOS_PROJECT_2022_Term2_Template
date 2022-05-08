#ifndef FOS_KERN_KHEAP_H_
#define FOS_KERN_KHEAP_H_

#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif

//Values for user heap placement strategy
#define KHP_PLACE_CONTALLOC 0x0
#define KHP_PLACE_FIRSTFIT 	0x1
#define KHP_PLACE_BESTFIT 	0x2
#define KHP_PLACE_NEXTFIT 	0x3
#define KHP_PLACE_WORSTFIT 	0x4

void* kmalloc(unsigned int size);
void kfree(void* virtual_address);
void* findSuitableEmptyBlock(void* startAddress, int numOfPages);
void* allocatePages(uint32 numOfPages, void* allocationAdd);

unsigned int kheap_virtual_address(unsigned int physical_address);
unsigned int kheap_physical_address(unsigned int virtual_address);

int numOfKheapVACalls ;
int getAllocationNumber(void* address);
int countEmptySize(void * address , uint32 wantedSize);

#endif // FOS_KERN_KHEAP_H_
