#include <semaphore.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    sem_t *sem_reader_count = sem_open("/reader_count", 0);
    sem_t *sem_critical_access = sem_open("/critical_access", 0);
    sem_t *sem_reader_count_access = sem_open("/reader_count_access", 0);

    int reader_count;
    printf("Waiting for access of the file...\n\n");

    sem_wait(sem_reader_count_access);

    sem_post(sem_reader_count);
    
    sem_getvalue(sem_reader_count, &reader_count);

    if(reader_count == 1){
        printf("First Reader : Waiting for access of the file...\n\n");
        sem_wait(sem_critical_access);
    }

    sem_post(sem_reader_count_access);

    // Reading of file

    FILE *fptr; 
  
    char c; 
    // Open file 
    fptr = fopen("file.txt", "r"); 
    if (fptr == NULL) 
    { 
        printf("Cannot open file \n"); 
        exit(0); 
    } 

    printf("Reading file...\n\n");
  
    // Read contents from file 
    c = fgetc(fptr); 
    while (c != EOF) 
    { 
        printf ("%c", c); 
        c = fgetc(fptr); 
    } 
  
    fclose(fptr); 

    char exit;

    while(exit != 'e'){
        printf("\n\nEnter 'e' to exit reading mode : ");
        scanf("%c", &exit);
    }

    sem_wait(sem_reader_count_access);
    sem_trywait(sem_reader_count);
    sem_getvalue(sem_reader_count, &reader_count);
    if(reader_count==0){
        sem_post(sem_critical_access);
    }
    sem_post(sem_reader_count_access);

}