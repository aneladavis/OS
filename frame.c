#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "frame.h"
#include "ll_double.h"

uint64_t frames_allocated;

uint64_t frames_available;

// No need to change or initialize this. This is "empty" space that simulate your physical memory.
_Alignas(4096) char memory[MEMORY_SIZE];
char bitmap[(MEMORY_SIZE / 4096) / 8];

// stack of free frames to use
struct list free_frames;

// keep track of total deallocated
int deallocated_frames = 0;

// You should define a bitmap to keep track of the allocated frames here.
// Look at the handout for details on size
/*
* Creates a bitmap that represents which frames of the memory chunk memory are allocated or available.
* A chunk of 4MB of memory has 1024 frames of 4096B (4KB) each. Your bitmap must have therefore 1024
* entries (one per page), with each byte taking care of 8 entries. Hence, your bitmap should be of
* size 128B. The function also initializes the variables frames_available and frames_allocated.
*/
void frame_init() {
    // Initialize the bitmap
    for(int i = 0; i < ((MEMORY_SIZE / 4096)/ 8); i++) {
        bitmap[i] = (char) 0;
    }

    // Initialize global variables
    frames_allocated = (uint64_t) 0;

    // Make all frames avalible
    frames_available = (uint64_t) (MEMORY_SIZE / 4096);

    ll_init(&free_frames);
}

void set_bit(int position) {
    // calculate byte and bit position
    int byte_position = (int) floor((double) position / 8.0);
    int bit_position = position % 8;

    // or the newly allocated bit to the existing allocated bits
    char temp = (1 << bit_position) | bitmap[byte_position];
    // update bitmap
    bitmap[byte_position] = temp;
}

void unset_bit(int position) {
    // calculate byte and bit position
    int byte_position = (int) floor((double) position / 8.0);
    int bit_position = position % 8;
    // shift to represent the correct bit arrangement
    char temp = 1 << bit_position;
    // or the newly allocated bit to the existing allocated bits
    temp = ~(1 << bit_position) & bitmap[byte_position];
    // update bitmap
    bitmap[byte_position] = temp;
}

int check_bit(int byte_position, int bit_position) {
    // check to see if the bit is avalible
    if((1 << bit_position) & bitmap[byte_position]) {
        // bit is not avalible 
        return 0;
    }
    // bit is avalible
    return 1;
}

int search_bit(int number_frames) {
    int count = 0;
    // go through all bytes in memory
    for(int i = 0; i < ((MEMORY_SIZE / 4096)); i++) {
        // check if a bit is available and increase counter
        if(check_bit(floor(i / 8), i % 8) == 1) {
            count++;
            // if all frames found return starting point
            if(count == number_frames) {
                return i - (number_frames - 1);
            }
        }
        // if consecutive bits not found, restart
        else {
            count = 0;
        }
    }
    return -1;
}

int64_t allocate_frame(int number_frames) {
    // Consult the bitmap and return the first available frame number, marking it as allocated
    // Increase the frames_allocated, decrease frames_available

    // implement stack 
    if((free_frames.head != NULL) && (number_frames == 1)) {
        struct node *free = ll_remove_head(&free_frames);
        // get bit location
        int free_bit = *((int *) free->data);
        int byte_position = (int) floor((double) free_bit / 8.0);
        int bit_position = free_bit % 8;
        if(check_bit(byte_position, bit_position)){
            set_bit(free_bit);
            // update variables
            frames_allocated += number_frames;
            frames_available -= number_frames;
            return free_bit;
        }
    }

    int start = search_bit(number_frames);
    int start_frame = start;
    int count = 0;
    if(start != -1) {
        while(count != number_frames) {
            // set the bit to be occupied
            set_bit(start);
            count++;
            start ++;
        }
        frames_allocated += number_frames;
        frames_available -= number_frames;
        return start_frame;
    }

    return -1; // Return according to what's documented in the header file for this module
}

int64_t deallocate_frame(uint64_t frame_number, int number_frames) {
    // Mark the frame as deallocated in the bitmap
    // Decrease the frames_allocated, increase frames_available
    int count = 0;
    if(number_frames > 0) {
        while(count != number_frames) {
            // set the bit to be occupied
            unset_bit(frame_number);
            count++;
            frame_number--;
            ll_insert_head(&free_frames, &frame_number);
        }
        frames_allocated -= number_frames;
        frames_available += number_frames;
        return number_frames;
    }

    return -1; // Return according to what's documented in the header file for this module
}
