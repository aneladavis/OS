#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include "frame.h"

int main(void) {
    frame_init();

    if(allocate_frame(1) < 0){
        perror("ERROR IN ALLOCATE FRAME");
    }
    // allocate all memory 
    // should fail
    // if(allocate_frame(1024) < 0){
    //     perror("ERROR IN ALLOCATE FRAME");
    // }
    if(allocate_frame(1) < 0){
        perror("ERROR IN ALLOCATE FRAME SECOND TIME");
    }

    if(allocate_frame(3) < 0){
        perror("ERROR IN ALLOCATE FRAME WITH MANY FRAMES");
    }

    if(deallocate_frame(1, 1) < 0){
        perror("ERROR IN DEALLOCATE FRAME");
    }

    if(allocate_frame(1) < 0){
        perror("COULDN'T ALLOCATE AFTER DEALLOCATING");
    }

    EXIT_SUCCESS;

}