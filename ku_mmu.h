#include <stdio.h>
#include <string.h>
#define ku_mmu_SIZE 256

typedef struct _ku_pte {
    char pte;
} ku_pte;

typedef struct ku_mmu_pcb {
    char pid;
    ku_pte page_table[ku_mmu_SIZE];
} ku_mmu_PCB;

typedef struct pList {
    ku_mmu_PCB** list;
    unsigned int size;
} ku_mmu_plist;

typedef struct alloc_info {
    char a_pid;
    int a_vpn;
} ku_mmu_aInfo;

typedef struct freeQueue {
    int *data;
    int front, rear;
} ku_mmu_fQueue;

typedef struct allocQueue {
    ku_mmu_aInfo *data;
    int front, rear;
} ku_mmu_aQueue;



ku_mmu_plist plist;
ku_mmu_fQueue ku_mmu_pFree_list;
ku_mmu_aQueue ku_mmu_pAlloc_list;
ku_mmu_fQueue ku_mmu_sFree_list;

unsigned int ku_mmu_msize;
unsigned int ku_mmu_ssize;

void initQueue(ku_mmu_fQueue *q, unsigned int size) {
    q->front = 0;
    q->rear = 0;
    q->data = (int*)malloc(sizeof(int)*size);
}

int isEmpty(ku_mmu_fQueue *q) {
    if (q->front == q->rear) {
        return 1;
    }
    else {
        return 0;
    }
}

int isFull(ku_mmu_fQueue *q, unsigned int size) {
    if (((q->rear + 1) % size) == q->front) {
        return 1;
    }
    else {
        return 0;
    }
}

void enqueue(ku_mmu_fQueue *q, int data, unsigned int size) {
    if (isFull(q, size)) {
        return;
    }
    else {
        q->rear = (q->rear + 1) % size;  //size!!!!!!!
        q->data[q->rear] = data;
    }
    return;
}

char dequeue(ku_mmu_fQueue *q, unsigned int size) {
    if (!isEmpty(q)) {
        q->front = (q->front + 1) % size;  //size!!!!!!!
    }
    return q->data[q->front];
}

void initQueue2(ku_mmu_aQueue *q, unsigned int size) {
    q->front = 0;
    q->rear = 0;
    q->data = (ku_mmu_aInfo*)malloc(sizeof(ku_mmu_aInfo)*size);
}

int isEmpty2(ku_mmu_aQueue *q) {
    if (q->front == q->rear) {
        return 1;
    }
    else {
        return 0;
    }
}

int isFull2(ku_mmu_aQueue *q, unsigned int size) {
    if (((q->rear + 1) % size) == q->front) {
        return 1;
    }
    else {
        return 0;
    }
}

void enqueue2(ku_mmu_aQueue *q, char pid, int vpn, unsigned int size) {
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

ku_mmu_aInfo dequeue2(ku_mmu_aQueue *q, unsigned int size) {
    if (!isEmpty2(q)) {
        q->front = (q->front + 1) % size;
    }
    return q->data[q->front];
}

int findPid(ku_mmu_plist *p, char fpid) {
    for (int i = 0; i < p->size; i++) {
        if (p->list[i]->pid == fpid) {
            return 1;
        }
    }
    return 0;
}

void initPList(ku_mmu_plist *p) {
    memset(p, 0, sizeof(ku_mmu_plist));
    p->size = 0;
    p->list = (ku_mmu_PCB**)malloc(sizeof(ku_mmu_PCB*));
}

void addPCB(ku_mmu_plist *p, char pid) {
    p->list[p->size] = (ku_mmu_PCB*)malloc(sizeof(ku_mmu_PCB));
    (p->list[p->size])->pid = pid;
    for (int i = 0; i < ku_mmu_SIZE; i++) {
        (p->list[p->size])->page_table[i].pte = 0;
    }
    p->size++;
}

int getPCB(ku_mmu_plist *p, char fpid) {
    for (int i = 0; i < p->size; i++) {
        if (p->list[i]->pid == fpid) {
            return i;
        }
    }
    return -1;
}

void swap(char pid, int vpn) {
    ku_mmu_aInfo info;
    int pfn;
    char pte;

    info = dequeue2(&ku_mmu_pAlloc_list, ku_mmu_msize);
    pte = plist.list[getPCB(&plist, info.a_pid)]->page_table[info.a_vpn].pte;
    plist.list[getPCB(&plist, pid)]->page_table[vpn].pte = pte;

    pfn = dequeue(&ku_mmu_sFree_list, ku_mmu_ssize);
    pte = pfn << 2;
    plist.list[getPCB(&plist, info.a_pid)]->page_table[info.a_vpn].pte = pte;
}

void allocPmem(char pid, int vpn) {
    int pfn;
    char pte;

    pfn = dequeue(&ku_mmu_pFree_list, ku_mmu_msize);
    pte = pfn << 2;
    pte = pte + 1;
    plist.list[getPCB(&plist, pid)]->page_table[vpn].pte = pte;
    enqueue2(&ku_mmu_pAlloc_list, pid, vpn, ku_mmu_msize);
}

void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size)
{
    int* pmem;
    int* smem;
    ku_mmu_msize = mem_size / 4;
    ku_mmu_ssize = swap_size / 4;

    pmem = (int*)malloc(mem_size);
    smem = (int*)malloc(swap_size);

    initQueue(&ku_mmu_pFree_list, ku_mmu_msize);
    initQueue2(&ku_mmu_pAlloc_list, ku_mmu_msize);
    initQueue(&ku_mmu_sFree_list, ku_mmu_ssize);

    for (int i = 1; i < ku_mmu_msize; i++) {
        enqueue(&ku_mmu_pFree_list, i, ku_mmu_msize);
    }
    for (int i = 1; i < ku_mmu_ssize; i++) {
        enqueue(&ku_mmu_sFree_list, i, ku_mmu_ssize);
    }

    initPList(&plist);
    
    if (pmem != NULL) {
        return pmem;
    }
    else {
        return 0;
    }
}

int ku_run_proc(char fpid, void **ku_cr3)
{
    // new pid
    if (findPid(&plist, fpid) == 0) {
        addPCB(&plist, fpid);
    }
    
    // context switch
    *ku_cr3 = plist.list[getPCB(&plist, fpid)]->page_table;
    
    return 0;
}

int ku_page_fault(char npid, char va)
{
    int pfn;
    char pte;
    int pte_offset = (va & 0xFC) >> 2;

    if (plist.list[getPCB(&plist, npid)]->page_table[pte_offset].pte == 0) {
        if (isEmpty(&ku_mmu_pFree_list)) {
            if (isEmpty(&ku_mmu_sFree_list)) {
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
        enqueue(&ku_mmu_sFree_list, pfn, ku_mmu_ssize);

        if (isEmpty(&ku_mmu_pFree_list)) {
            swap(npid, pte_offset);
        }
        else {
            allocPmem(npid, pte_offset);
        }
    }

    return 0;
}