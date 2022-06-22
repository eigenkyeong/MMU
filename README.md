# KU_MMU
Memory Management Unit (Memory Virtualization)


## Addressing
• 8-bit addressing
– Address space: 256 Bytes
– Page size: 4 Bytes
– PTE: 1 Byte

![image](https://user-images.githubusercontent.com/76428047/175110308-0913cf2e-5de4-4dcd-9acd-b517bbe3175a.png)


## PTE
• PTE
![image](https://user-images.githubusercontent.com/76428047/175110409-e48c08f7-51c0-45be-95ca-301c0e2e978d.png)
– Unmapped PTE is filled with zeros

![image](https://user-images.githubusercontent.com/76428047/175110519-1f803c54-adde-41bc-99a2-0088b303b10f.png)
– Swap space: 512 Bytes (=27 * 4 Bytes)
– Offset starts from 1
   • 0th page in swap space is not used
– Present bit is 0

</br></br>

• Examples
![image](https://user-images.githubusercontent.com/76428047/175110604-1108dc60-8207-4ae2-a647-06191e8d0aa2.png)
• Virtual page is neither mapped nor swapped out
![image](https://user-images.githubusercontent.com/76428047/175110675-b1ad9f70-8722-4a4b-a9fb-6a9483233ba1.png)
• Virtual page is mapped to page frame 0 (occupied by OS)
![image](https://user-images.githubusercontent.com/76428047/175110746-5f3618ac-c655-41a6-9c34-43f18258aaa4.png)
• Virtual page is swapped out to 6th page in swap space


## ku_cpu.c
• Commend
– ku_cpu <input_file> <pmem_size> <swap_size> 


## Initialization
• void *ku_mmu_init (unsigned int mem_size, unsigned int swap_size)
– Resource initialization function
  • Will be called only once at the initialization phase
– mem_size: physical memory size in bytes
  • allocate a memory space and manage alloc/free lists
– Assume that page frame 0 is occupied by OS
  • Do not consider the memory space consumed by page table and internal data structures (e.g., alloc/free lists and PCBs)
– swap_size: swap disk size in bytes
  • Allocate a memory space instead of real disk space
– pmem and swap spaces are dummy; no need of actual data copies between pmem and swap spaces
– Return value
  • Pointer (i.e., address) to the allocated memory area that simulates the physical memory
  • 0: fail










