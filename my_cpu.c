#include <stdio.h>
#include <stdlib.h>
#include "./my_mmu.h"

char my_traverse(void *my_cr3, char va);

void mmu_fin(FILE *fd, void *pmem)
{
	if(fd) fclose(fd);
	if(pmem) free(pmem);
}

int main(int argc, char *argv[])
{
	FILE *fd=NULL;
	char fpid, pid=0, va, pa;
	unsigned int pmem_size, swap_size;
	void *my_cr3, *pmem=NULL;

	if(argc != 4){
		printf("my_cpu: Wrong number of arguments\n");
		return 1;
	}

	fd = fopen(argv[1], "r");
	if(!fd){
		printf("my_cpu: Fail to open the input file\n");
		return 1;
	}

	pmem_size = strtol(argv[2], NULL, 10);
	swap_size = strtol(argv[3], NULL, 10);
	pmem = my_mmu_init(pmem_size, swap_size);
	if(!pmem){
		printf("my_cpu: Fail to allocate the physical memory\n");
		mmu_fin(fd, pmem);
		return 1;
	}

	while(fscanf(fd, "%hhd %hhd", &fpid, &va) != EOF){

		if(pid != fpid){
			if(my_run_proc(fpid, &my_cr3) == 0)
				pid = fpid; /* context switch */
			else{
				printf("my_cpu: Context switch is failed\n");
				mmu_fin(fd, pmem);
				return 1;
			} 
		}

		pa = my_traverse(my_cr3, va);
		if(pa == 0){
			if(my_page_fault(pid, va) != 0){
				printf("my_cpu: Fault handler is failed\n");
				mmu_fin(fd, pmem);
				return 1;
			}
			printf("[%d] VA: %hhd -> Page Fault\n", pid, va);

			/* Retry after page fault */
			pa = my_traverse(my_cr3, va); 
			if(pa == 0){
				printf("my_cpu: Addr translation is failed\n");
				mmu_fin(fd, pmem);
				return 1;
			}
		}

		printf("[%d] VA: %hhd -> PA: %hhd\n", pid, va, pa);
	}

	mmu_fin(fd, pmem);
	return 0;
}
