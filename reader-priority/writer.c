#include <semaphore.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    sem_t *sem_critical_access = sem_open("/critical_access", 0);

    printf("Waiting for access of the file...\n\n");
        sem_wait(sem_critical_access);

    FILE *fptr; 
  
    char c; 
    // Open file 
    fptr = fopen("file.txt", "a"); 
    if (fptr == NULL) 
    { 
        printf("Cannot open file \n"); 
        exit(0); 
    }

    char *string;

    printf("Enter a string to write to the file : ");
    scanf("%s", string);

    printf("Writing \"%s\" to file...", string);
  
    // Write contents from file 
    fputs("\n", fptr);
    fputs(string, fptr);
  
    fclose(fptr); 

    char exit;

    //scans an extra character when scanning a string
    scanf("%c", &exit);

    while(exit != 'e'){
        printf("\n\nEnter e to exit writing mode   ");
        scanf("%c", &exit);
    }

    sem_post(sem_critical_access);

}