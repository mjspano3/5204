#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define GB_SIZE 1073741824
#define CHUNK_SIZE 4096

int main(){

    void* ptr = malloc(GB_SIZE);

    // size_t total = GB_SIZE/CHUNK_SIZE;
    // size_t i;

    for(i = 0; i < total; i++){
        //function
        // char command[128];
        // sprintf(command, "sudo bash  -c 'echo %p > /proc/cs5204'", (char*)ptr + i * CHUNK_SIZE);
        // system(command);
        
        *((char*)ptr + i * CHUNK_SIZE) = i;
    }

        char command[128];
        sprintf(command, "sudo bash  -c 'echo %p > /proc/cs5204'", (char*)ptr);
        system(command);
}
