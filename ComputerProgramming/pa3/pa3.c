/**********************************************************************
 * Copyright (c) 2020-2021
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "types.h"
#include "list_head.h"
#include "vm.h"

/**
 * Ready queue of the system
 */
extern struct list_head processes;

/**
 * Currently running process
 */
extern struct process *current;

/**
 * Page Table Base Register that MMU will walk through for address translation
 */
extern struct pagetable *ptbr;


/**
 * The number of mappings for each page frame. Can be used to determine how
 * many processes are using the page frames.
 */
extern unsigned int mapcounts[];

unsigned int least_pfn=0;
unsigned int backup_pfn[NR_PAGEFRAMES];
struct process head;
/**
 * alloc_page(@vpn, @rw)
 *
 * DESCRIPTION
 *   Allocate a page frame that is not allocated to any process, and map it
 *   to @vpn. When the system has multiple free pages, this function should
 *   allocate the page frame with the **smallest pfn**.
 *   You may construct the page table of the @current process. When the page
 *   is allocated with RW_WRITE flag, the page may be later accessed for writes.
 *   However, the pages populated with RW_READ only should not be accessed with
 *   RW_WRITE accesses.
 *
 * RETURN
 *   Return allocated page frame number.
 *   Return -1 if all page frames are allocated.
 */
unsigned int alloc_page(unsigned int vpn, unsigned int rw)
{
	unsigned int temp;
	int check= -1;
	int range = vpn/NR_PTES_PER_PAGE;
	int row = vpn % NR_PTES_PER_PAGE;
	//printf("vpn is and rw is : %u, %u\n", vpn, rw);
	//printf("pid of current: %d\n", current->pid);
	if(current->pagetable.outer_ptes[range] == NULL){
		current->pagetable.outer_ptes[range] = malloc(sizeof(struct pte)*NR_PTES_PER_PAGE);
	}
	if(current->pagetable.outer_ptes[range]->ptes[row].valid ==false){
		current->pagetable.outer_ptes[range]->ptes[row].valid = true;
		if(rw == 3){
			current->pagetable.outer_ptes[range]->ptes[row].writable =true;
		}
		else{
			current->pagetable.outer_ptes[range]->ptes[row].writable = false;
		}
			for(int i=0;i<NR_PAGEFRAMES;i++){
				if(backup_pfn[i] == 0){
					current->pagetable.outer_ptes[range]->ptes[row].pfn = i;
					backup_pfn[i]++;
					break;
				}
			}
		check=0;
		mapcounts[current->pagetable.outer_ptes[range]->ptes[row].pfn]++;
	}
	if(check==-1)
		return -1;
	else
		return current->pagetable.outer_ptes[range]->ptes[row].pfn;

}


/**
 * free_page(@vpn)
 *
 * DESCRIPTION
 *   Deallocate the page from the current processor. Make sure that the fields
 *   for the corresponding PTE (valid, writable, pfn) is set @false or 0.
 *   Also, consider carefully for the case when a page is shared by two processes,
 *   and one process is to free the page.
 */
void free_page(unsigned int vpn)
{
	unsigned int range= vpn/NR_PTES_PER_PAGE;
	unsigned int row = vpn % NR_PTES_PER_PAGE;

	mapcounts[current->pagetable.outer_ptes[range]->ptes[row].pfn]--;
	if(mapcounts[current->pagetable.outer_ptes[range]->ptes[row].pfn]>0){
		backup_pfn[current->pagetable.outer_ptes[range]->ptes[row].pfn]--;
		current->pagetable.outer_ptes[range]->ptes[row].pfn = 0;
		current->pagetable.outer_ptes[range]->ptes[row].private = 0;
		current->pagetable.outer_ptes[range]->ptes[row].writable = false;
		current->pagetable.outer_ptes[range]->ptes[row].valid = false;
	}
	else{
		backup_pfn[current->pagetable.outer_ptes[range]->ptes[row].pfn]--;
		current->pagetable.outer_ptes[range]->ptes[row].pfn = 0;
		current->pagetable.outer_ptes[range]->ptes[row].private = 0;
		current->pagetable.outer_ptes[range]->ptes[row].writable = false;
		current->pagetable.outer_ptes[range]->ptes[row].valid = false;
	}
	
}


/**
 * handle_page_fault()
 *
 * DESCRIPTION
 *   Handle the page fault for accessing @vpn for @rw. This function is called
 *   by the framework when the __translate() for @vpn fails. This implies;
 *   0. page directory is invalid
 *   1. pte is invalid
 *   2. pte is not writable but @rw is for write
 *   This function should identify the situation, and do the copy-on-write if
 *   necessary.
 *
 * RETURN
 *   @true on successful fault handling
 *   @false otherwise
 */
bool handle_page_fault(unsigned int vpn, unsigned int rw)
{
	unsigned int *pfn;
	unsigned int range = vpn/NR_PTES_PER_PAGE;
	unsigned int row = vpn%NR_PTES_PER_PAGE;
	//printf("what is rw: %u\n", rw);
	if(current->pagetable.outer_ptes[range]->ptes[row].private==1){
		if(mapcounts[current->pagetable.outer_ptes[range]->ptes[row].pfn]>1){
			free_page(vpn);
			alloc_page(vpn, rw+1);
			return true;
		}
		else{
			free_page(vpn);
			alloc_page(vpn, rw+1);
			return true;
		}
	}
	return false;
}


/**
 * switch_process()
 *
 * DESCRIPTION
 *   If there is a process with @pid in @processes, switch to the process.
 *   The @current process at the moment should be put into the @processes
 *   list, and @current should be replaced to the requested process.
 *   Make sure that the next process is unlinked from the @processes, and
 *   @ptbr is set properly.
 *
 *   If there is no process with @pid in the @processes list, fork a process
 *   from the @current. This implies the forked child process should have
 *   the identical page table entry 'values' to its parent's (i.e., @current)
 *   page table. 
 *   To implement the copy-on-write feature, you should manipulate the writable
 *   bit in PTE and mapcounts for shared pages. You may use pte->private for 
 *   storing some useful information :-)
 */
void switch_process(unsigned int pid)
{
	int check =-1;
	unsigned int vpn;
	head.list = processes;
	struct process* temp;
	temp = &head;
	struct process* child;
	list_for_each_entry_continue(temp, &processes, list){
		if(pid== temp->pid){
			list_del_init(&temp->list);
			list_add_tail(&current->list, &processes);
			current = temp;
			ptbr = &current->pagetable;
			check =0;
		}
	}
	if(check!=0){
		child = malloc(sizeof(struct process));
		list_add_tail(&current->list, &processes);
		child->pid = pid;
		temp = current;
		current = child;
		for(int i=0;i<NR_PTES_PER_PAGE;i++){
			for(int j=0;j<NR_PTES_PER_PAGE;j++){
				if(temp->pagetable.outer_ptes[i] != NULL){
					if(temp->pagetable.outer_ptes[i]->ptes[j].valid ==true){
						vpn = i*NR_PTES_PER_PAGE + j;
						alloc_page(vpn, RW_READ);
						mapcounts[current->pagetable.outer_ptes[i]->ptes[j].pfn]--;
						backup_pfn[current->pagetable.outer_ptes[i]->ptes[j].pfn]--;
						current->pagetable.outer_ptes[i]->ptes[j].pfn = temp->pagetable.outer_ptes[i]->ptes[j].pfn;
						mapcounts[temp->pagetable.outer_ptes[i]->ptes[j].pfn]++;
						backup_pfn[temp->pagetable.outer_ptes[i]->ptes[j].pfn]++;
						if(temp->pagetable.outer_ptes[i]->ptes[j].writable == true || temp->pagetable.outer_ptes[i]->ptes[j].private ==1){
							current->pagetable.outer_ptes[i]->ptes[j].private = 1;
							temp->pagetable.outer_ptes[i]->ptes[j].private = 1;
							temp->pagetable.outer_ptes[i]->ptes[j].writable = false;
						}
					}
				}
			}
		}
	ptbr = &current->pagetable;
	}
}
