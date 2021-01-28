#include <semaphore.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void writer(sem_t *sem_critical_access, sem_t *sem_reader_count)
{
    sem_wait(sem_critical_access);

    int reader_count;
    sem_getvalue(sem_reader_count, &reader_count);

    if (reader_count > 0)
    {
        printf("Reader's are in queue, please wait until they finish reading...\n\n");
        sem_post(sem_critical_access);
        sleep(1);
        return writer(sem_critical_access, sem_reader_count);
    }
}

int main()
{
    sem_t *sem_critical_access = sem_open("/critical_access", 0);
    sem_t *sem_reader_count = sem_open("/reader_count", 0);

    printf("Waiting for access of the file...\n\n");

    writer(sem_critical_access, sem_reader_count);
    
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
    fflush(stdout);
    // Write contents from file
    fputs("\n", fptr);
    fputs(string, fptr);

    fclose(fptr);

    char exit;

    //scans an extra character when scanning a string
    scanf("%c", &exit);

    while (exit != 'e')
    {
        printf("\n\nEnter e to exit writing mode   ");
        scanf("%c", &exit);
    }

    sem_post(sem_critical_access);
}