#include <pthread.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int reader_count = 0, writer_count = 0, data = 0;
sem_t critical_access;
sem_t reader_count_access;
sem_t writer_count_access;
sem_t priority_access;

void *reader()
{
    printf("\nReader incoming\n");
    sem_wait(&priority_access);
    sem_wait(&reader_count_access);
    reader_count++;
    if (reader_count == 1)
    {
        sem_wait(&critical_access);
    }
    sem_post(&reader_count_access);
    sem_post(&priority_access);

    FILE *fptr;

    char c;
    // Open file
    fptr = fopen("file.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    char file_data[100];
    // Read contents from file
    c = fgetc(fptr);
    int count = 0;
    while (c != EOF)
    {
        file_data[count] = c;
        count++;
        c = fgetc(fptr);
    }
    file_data[count] = '\0';

    printf("\nReading file... : %s\n", file_data);

    fclose(fptr);

    //Post Reader Count Critical Section Starts
    sem_wait(&reader_count_access);
    reader_count--;
    if (reader_count == 0)
    {
        sem_post(&critical_access);
    }
    sem_post(&reader_count_access);
    //Post Reader Count Critical Section Ends
}

//W1(active) W2 R1 R2

void *writer()
{
    printf("\nWriter incoming\n");
    sem_wait(&writer_count_access);
    writer_count++;
    if (writer_count == 1)
    {
        sem_wait(&priority_access);
    }
    sem_post(&writer_count_access);

    sem_wait(&critical_access);
    data++;
    FILE *fptr;

    fptr = fopen("file.txt", "w");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    printf("\nWriting to file... : %d\n", data);
    fprintf(fptr, "%d", data);
    fflush(stdout);

    fclose(fptr);

    sem_post(&critical_access);

    sem_wait(&writer_count_access);
    writer_count--;
    if (writer_count == 0)
    {
        sem_post(&priority_access);
    }
    sem_post(&writer_count_access);
}

int main()
{
    sem_init(&critical_access, 0, 1);
    sem_init(&reader_count_access, 0, 1);
    sem_init(&writer_count_access, 0, 1);
    sem_init(&priority_access, 0, 1);

    pthread_t rd[5], wr[5];

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&rd[i], NULL, writer, NULL);
        pthread_create(&wr[i], NULL, reader, NULL);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(rd[i], NULL);
        pthread_join(wr[i], NULL);
    }

    sem_destroy(&critical_access);
    sem_destroy(&reader_count_access);
}
