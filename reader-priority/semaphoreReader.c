#include <semaphore.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    sem_t *sem_reader_count = sem_open("/reader_count", 0);
    sem_t *sem_critical_access = sem_open("/critical_access", 0);
    sem_t *sem_reader_count_access = sem_open("/critical_access", 0);

    int reader_count, critical_access, reader_count_access;

    sem_getvalue(sem_reader_count_access, &reader_count_access);
    sem_getvalue(sem_reader_count, &reader_count);
    sem_getvalue(sem_critical_access, &critical_access);

    printf("\nsem_reader_count = %d \n\nsem_critical_access = %d \n\nsem_reader_count_access = %d \n", reader_count, critical_access, reader_count_access);

}