# MMU
Memory Management Unit (Memory Virtualization)


## Addressing
**8-bit addressing**

– Address space: 256 Bytes </br>
– Page size: 4 Bytes </br>
– PTE: 1 Byte </br>
![image](https://user-images.githubusercontent.com/76428047/175110308-0913cf2e-5de4-4dcd-9acd-b517bbe3175a.png)



## PTE
**PTE**
![image](https://user-images.githubusercontent.com/76428047/175110409-e48c08f7-51c0-45be-95ca-301c0e2e978d.png)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; – Unmapped PTE is filled with zeros

![image](https://user-images.githubusercontent.com/76428047/175110519-1f803c54-adde-41bc-99a2-0088b303b10f.png)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; – Swap space: 512 Bytes (=27 * 4 Bytes) </br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; – Offset starts from 1 </br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;    &nbsp; &nbsp; • 0th page in swap space is not used </br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; – Present bit is 0

</br>


## my_cpu.c
• Commend
```
$ my_cpu <input_file> <pmem_size> <swap_size> 
```

## Initialization
**void \*my_mmu_init (unsigned int mem_size, unsigned int swap_size)** 
</br></br>
– mem_size: physical memory size in bytes </br>
  &nbsp; • allocate a memory space and manage alloc/free lists </br></br>
– swap_size: swap disk size in bytes </br>
  &nbsp; • Allocate a memory space instead of real disk space </br>
  &nbsp; • manage alloc/free lists </br></br>
– Return value </br>
  &nbsp; • Pointer (i.e., address) to the allocated memory area that simulates the physical memory </br>
  &nbsp; • 0: fail </br></br>


## Context Switch
**int my_run_proc (char pid, struct my_pte \*\*my_cr3)** </br></br>
– Performs context switch </br>
  &nbsp; • If pid is new, the function creates a process (i.e., PCB) and its page table </br></br>
– pid: pid of the next process </br></br>
– my_cr3: stores the base address of the page table for the current process </br></br>
– Return value </br>
  &nbsp; • 0: success </br>
  &nbsp; • -1: fail </br></br>



## Page Fault Handling
**int my_page_fault (char pid, char va)** </br></br>
– Handling a page fault caused by demand paging or swapping </br>
  &nbsp; • Page replacement policy: FIFO </br></br>
– Managing swap space </br>
  &nbsp; • Alloc/free lists </br></br>
– pid: process id </br></br>
– va: virtual address </br></br>
– Return value </br>
  &nbsp; • 0: success </br>
  &nbsp; • -1: fail </br>

