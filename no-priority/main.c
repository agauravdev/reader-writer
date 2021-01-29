#include <pthread.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int reader_count = 0, data = 0;
sem_t critical_access;
sem_t reader_count_access;
pthread_mutex_t order_lock;

void *reader(void *arg)
{
    int num = *((int *)arg);

    printf("\n%d Reader incoming\n", num);
    pthread_mutex_lock(&order_lock);

    sem_wait(&reader_count_access);
    reader_count++;
    if (reader_count == 1)
    {
        sem_wait(&critical_access);
    }
    pthread_mutex_unlock(&order_lock);

    sem_post(&reader_count_access);

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

    printf("\n%d Reading file... : %s\n", num, file_data);

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

void *writer(void *arg)
{
    int num = *((int *)arg);

    printf("\n%d Writer incoming\n", num);

    pthread_mutex_lock(&order_lock);
    sem_wait(&critical_access);
    pthread_mutex_unlock(&order_lock);

    data++;
    FILE *fptr;

    fptr = fopen("file.txt", "w");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }

    printf("\n%d Writing to file... : %d\n",num, data);
    fprintf(fptr, "%d", data);
    fflush(stdout);

    fclose(fptr);

    sem_post(&critical_access);
}

int main()
{
    sem_init(&critical_access, 0, 1);
    sem_init(&reader_count_access, 0, 1);

    if (pthread_mutex_init(&order_lock, NULL) != 0)
    {
        printf("\n pthread_mutex_init failed\n");
        return 0;
    }

    pthread_t rd[5], wr[5];

    for (int i = 0; i < 5; i++)
    {
        int *arg = malloc(sizeof(*arg));
        *arg = i;

        pthread_create(&rd[i], NULL, writer, arg);
        pthread_create(&wr[i], NULL, reader, arg);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(rd[i], NULL);
        pthread_join(wr[i], NULL);
    }

    sem_destroy(&critical_access);
    sem_destroy(&reader_count_access);
}
