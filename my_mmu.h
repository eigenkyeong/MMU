#include <stdio.h>
#include <string.h>
#define my_mmu_SIZE 256

typedef struct _my_pte {
    char pte;
} my_pte;

typedef struct my_mmu_pcb {
    char pid;
    my_pte page_table[my_mmu_SIZE];
} my_mmu_PCB;

typedef struct pList {
    my_mmu_PCB** list;
    unsigned int size;
} my_mmu_plist;

typedef struct alloc_info {
    char a_pid;
    int a_vpn;
} my_mmu_aInfo;

typedef struct freeQueue {
    int *data;
    int front, rear;
} my_mmu_fQueue;

typedef struct allocQueue {
    my_mmu_aInfo *data;
    int front, rear;
} my_mmu_aQueue;



my_mmu_plist plist;
my_mmu_fQueue my_mmu_pFree_list;
my_mmu_aQueue my_mmu_pAlloc_list;
my_mmu_fQueue my_mmu_sFree_list;

unsigned int my_mmu_msize;
unsigned int my_mmu_ssize;

void initQueue(my_mmu_fQueue *q, unsigned int size) {
    q->front = 0;
    q->rear = 0;
    q->data = (int*)malloc(sizeof(int)*size);
}

int isEmpty(my_mmu_fQueue *q) {
    if (q->front == q->rear) {
        return 1;
    }
    else {
        return 0;
    }
}

int isFull(my_mmu_fQueue *q, unsigned int size) {
    if (((q->rear + 1) % size) == q->front) {
        return 1;
    }
    else {
        return 0;
    }
}

void enqueue(my_mmu_fQueue *q, int data, unsigned int size) {
    if (isFull(q, size)) {
        return;
    }
    else {
        q->rear = (q->rear + 1) % size;  //size!!!!!!!
        q->data[q->rear] = data;
    }
    return;
}

char dequeue(my_mmu_fQueue *q, unsigned int size) {
    if (!isEmpty(q)) {
        q->front = (q->front + 1) % size;  //size!!!!!!!
    }
    return q->data[q->front];
}

void initQueue2(my_mmu_aQueue *q, unsigned int size) {
    q->front = 0;
    q->rear = 0;
    q->data = (my_mmu_aInfo*)malloc(sizeof(my_mmu_aInfo)*size);
}

int isEmpty2(my_mmu_aQueue *q) {
    if (q->front == q->rear) {
        return 1;
    }
    else {
        return 0;
    }
}

int isFull2(my_mmu_aQueue *q, unsigned int size) {
    if (((q->rear + 1) % size) == q->front) {
        return 1;
    }
    else {
        return 0;
    }
}

void enqueue2(my_mmu_aQueue *q, char pid, int vpn, unsigned int size) {
    if (isFull2(q, size)) {
        return;
    }
    else {
        q->rear = (q->rear + 1) % size;
        q->data[q->rear].a_pid = pid;
        q->data[q->rear].a_vpn = vpn;
    }
    return;
}

my_mmu_aInfo dequeue2(my_mmu_aQueue *q, unsigned int size) {
    if (!isEmpty2(q)) {
        q->front = (q->front + 1) % size;
    }
    return q->data[q->front];
}

int findPid(my_mmu_plist *p, char fpid) {
    for (int i = 0; i < p->size; i++) {
        if (p->list[i]->pid == fpid) {
            return 1;
        }
    }
    return 0;
}

void initPList(my_mmu_plist *p) {
    memset(p, 0, sizeof(my_mmu_plist));
    p->size = 0;
    p->list = (my_mmu_PCB**)malloc(sizeof(my_mmu_PCB*));
}

void addPCB(my_mmu_plist *p, char pid) {
    p->list[p->size] = (my_mmu_PCB*)malloc(sizeof(my_mmu_PCB));
    (p->list[p->size])->pid = pid;
    for (int i = 0; i < my_mmu_SIZE; i++) {
        (p->list[p->size])->page_table[i].pte = 0;
    }
    p->size++;
}

int getPCB(my_mmu_plist *p, char fpid) {
    for (int i = 0; i < p->size; i++) {
        if (p->list[i]->pid == fpid) {
            return i;
        }
    }
    return -1;
}

void swap(char pid, int vpn) {
    my_mmu_aInfo info;
    int pfn;
    char pte;

    info = dequeue2(&my_mmu_pAlloc_list, my_mmu_msize);
    pte = plist.list[getPCB(&plist, info.a_pid)]->page_table[info.a_vpn].pte;
    plist.list[getPCB(&plist, pid)]->page_table[vpn].pte = pte;

    pfn = dequeue(&my_mmu_sFree_list, my_mmu_ssize);
    pte = pfn << 2;
    plist.list[getPCB(&plist, info.a_pid)]->page_table[info.a_vpn].pte = pte;
}

void allocPmem(char pid, int vpn) {
    int pfn;
    char pte;

    pfn = dequeue(&my_mmu_pFree_list, my_mmu_msize);
    pte = pfn << 2;
    pte = pte + 1;
    plist.list[getPCB(&plist, pid)]->page_table[vpn].pte = pte;
    enqueue2(&my_mmu_pAlloc_list, pid, vpn, my_mmu_msize);
}

void *my_mmu_init(unsigned int mem_size, unsigned int swap_size)
{
    int* pmem;
    int* smem;
    my_mmu_msize = mem_size / 4;
    my_mmu_ssize = swap_size / 4;

    pmem = (int*)malloc(mem_size);
    smem = (int*)malloc(swap_size);

    initQueue(&my_mmu_pFree_list, my_mmu_msize);
    initQueue2(&my_mmu_pAlloc_list, my_mmu_msize);
    initQueue(&my_mmu_sFree_list, my_mmu_ssize);

    for (int i = 1; i < my_mmu_msize; i++) {
        enqueue(&my_mmu_pFree_list, i, my_mmu_msize);
    }
    for (int i = 1; i < my_mmu_ssize; i++) {
        enqueue(&my_mmu_sFree_list, i, my_mmu_ssize);
    }

    initPList(&plist);
    
    if (pmem != NULL) {
        return pmem;
    }
    else {
        return 0;
    }
}

int my_run_proc(char fpid, void **my_cr3)
{
    // new pid
    if (findPid(&plist, fpid) == 0) {
        addPCB(&plist, fpid);
    }
    
    // context switch
    *my_cr3 = plist.list[getPCB(&plist, fpid)]->page_table;
    
    return 0;
}

int my_page_fault(char npid, char va)
{
    int pfn;
    char pte;
    int pte_offset = (va & 0xFC) >> 2;

    if (plist.list[getPCB(&plist, npid)]->page_table[pte_offset].pte == 0) {
        if (isEmpty(&my_mmu_pFree_list)) {
            if (isEmpty(&my_mmu_sFree_list)) {
                return -1;
            }
            swap(npid, pte_offset);
        }
        else {
            allocPmem(npid, pte_offset);
        }
    }
    else {
        // swap in
        pte = plist.list[getPCB(&plist, npid)]->page_table[pte_offset].pte;
        pfn = pte >> 2;
        enqueue(&my_mmu_sFree_list, pfn, my_mmu_ssize);

        if (isEmpty(&my_mmu_pFree_list)) {
            swap(npid, pte_offset);
        }
        else {
            allocPmem(npid, pte_offset);
        }
    }

    return 0;
}
