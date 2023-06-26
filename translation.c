#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#include "translation.h"

void *table_root = NULL;

entry *allocate_table(){
	int frame_number = allocate_frame(1);
	if(frame_number == -1){
		return -1;
	}
	int frame_address = FRAME_ADDRESS(frame_number); 
	entry *table = (entry *) frame_address;	
	// set all entries to zero
	for(int i = 0; i < 512; i ++){
		// fill with 52 zeros
		table[i].address = 0; // what do I set this to?
		// unused flag on
		table[i].unused = 0; // make zero
		// set flag to zero
		table[i].flags = 0; // make zero
	}
	return table;
}

int vm_map(uint64_t page, uint64_t frame, int number, uint16_t flags) {
	if(&table_root == NULL){
		table_root = allocate_table();
		if(table_root == -1){
			return 0;
		}
	}
	// start at the root table
	entry *start = table_root;
	entry *prev_entry;
	int count = 0;
	int index = 0;
	while(count < 4){
		// calculate the number from the 9 bits
		index = *((int*) (((0x1FF << (count * 8)) && page)));
		// check if another pointer table does not exist
		if(start[index].unused == 0){
			// set first prev pointer
			if(count = 0){
				prev_entry = start;
			}
			else{
				// set prev pointer to new table entry
				prev_entry->address = (uint64_t) &start[index];
			}
			// make the index used
			start[index].unused = 1;
			// update prev pointer to next table entry
			prev_entry = start;
			// create new table
			entry *new_table  = allocate_table();
			if(new_table == -1){
				return 0;
			}
			start = new_table;

		}
		else{
			// set first prev pointer
			if(count = 0){
				prev_entry = start;
			}
			else{
				// set prev pointer to new table entry
				prev_entry->address = (uint64_t) &start[index];
			}
			// update prev pointer to next table entry
			prev_entry = start;
			// go to next table
			start = (entry *) start[address];
		}
		count++;

	}
	// in last table
	start[index].address = frame;
	start[index].flags = flags;
	start[index].unused = 1;

	return 1;
}

int vm_unmap(uint64_t page, int number) {
	// traverse down tree 
	if(table_root == NULL){
		return 0;
	}
	entry *start = table_root;
	int count = 0;
	int index = 0;
	while(count < 4){
		// calculate the number from the 9 bits
		index = *((int*) (((0x1FF << (count * 8)) && page)));
		// check if another pointer table does not exist
		if(start[index].unused == 0){
			// path does not exist...
			return 0;
		}
		else{
			// if not go to next table that the entry points to
			start = (entry *) start[address];
		}
		count++;
	}
	// make entry unused
	start[index].unused = 0;
	start[index].flags = 0;

	return 1;
}

uint64_t vm_locate(int number) {
	int count = 0;
	uint64_t first_page = 0;
	if(table_root == NULL){
		return UINT64_MAX;
	}
	int page_low = 0;
	entry *start = table_root;
	// search through first table
	for(int i = 0; i < 512; i ++){
		entry temp = start[i];
		// go through to leaf node
		for(int j = 0; j < 4; j ++){
			if(temp.unused != 0){
				temp = (entry) start->address;
			}
			else{
				break;
			}
		}
		// check if leaf node is empty
		if(temp.unused == 0){
			if(count == 0){
				// save page number from first entry
				first_page = temp.address;
			}
			count ++;
		}
		else{
			// reset count and first page variable
			count = 0;
			first_page = 0;
		}
	}
	if(first_page != 0){
		return first_page;
	}
	return -1;
}

uint64_t vm_translate(uint64_t virtual_address) {
	// Dummy code: there's no translation being done
	return virtual_address;
}
