#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define GB_SIZE 1073741824
#define CHUNK_SIZE 4096
#define MEM_SIZE_512MB (512 * 1024 * 1024)
#define MEM_SIZE_256MB (256 * 1024 * 1024)

void* base_ptr;

void* fast_write(void* args){
    time_t start_time = time(NULL);
    time_t elapsed_time;
    
    while (1) {
        size_t i;
        size_t num_pages = MEM_SIZE_256MB / PAGE_SIZE;

        for (i = 0; i < num_pages; ++i) {
            base_ptr[i * PAGE_SIZE] = 0xFF; // Perform a write operation to the page
        }
        elapsed_time = time(NULL) - start_time;

        if (elapsed_time >= 60) { // 1 minute (60 seconds) has passed
            break; // Exit the loop after 1 minute
        }
    }
}

void* slow_write(void* args){
    
    time_t start_time = time(NULL);
    time_t elapsed_time;
    
    while (1) {
        size_t i;
        size_t num_pages = MEM_SIZE_256MB / PAGE_SIZE;

        for (i = 0; i < num_pages; ++i) {
            base_ptr[i * PAGE_SIZE + MEM_SIZE_256MB] = 0xFF; // Perform a write operation to the page
        }

        elapsed_time = time(NULL) - start_time;

        if (elapsed_time >= 60) { // 1 minute (60 seconds) has passed
            break; // Exit the loop after 1 minute
        }

        usleep(1000); // Sleep for 1ms
    }
}




int main(){

    void* ptr = malloc(GB_SIZE);
    base_ptr = ptr;

    size_t total = GB_SIZE/CHUNK_SIZE;
    size_t i;

    for(i = 0; i < total; i++){
        //function
        // char command[128];
        // sprintf(command, "sudo bash  -c 'echo %p > /proc/cs5204'", (char*)ptr + i * CHUNK_SIZE);
        // system(command);
        
        *((char*)ptr + i * CHUNK_SIZE) = i;
    }

    printf("buffer offset: %p\n", ptr);

    char command[128];
    sprintf(command, "sudo bash  -c 'echo %p > /proc/cs5204'", (char*)ptr);
    system(command);

    pthread_t thread1, thread2;  
    pthread_create(&thread1, NULL, fast_write, NULL);
    pthread_create(&thread2, NULL, slow_write, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
}
