#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1;
int numReader = 0;

void *writer(void *wno)
{
    sem_wait(&wrt);
    // Writing section
    cnt = cnt * 2;
    printf("\nWriter %d modified count to %d\n", (*((int *)wno)), cnt);
    // Writing ends
    sem_post(&wrt);
    return NULL;
}

void *reader(void *rno)
{
    pthread_mutex_lock(&mutex);
    numReader++;
    if (numReader == 1)
    {
        sem_wait(&wrt);
    }
    pthread_mutex_unlock(&mutex);

    // Reading section
    printf("\nReader %d read count as %d", (*((int *)rno)), cnt);
    // Reading ends

    pthread_mutex_lock(&mutex);
    numReader--;
    if (numReader == 0)
    {
        sem_post(&wrt);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t read[3], write[3];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt, 0, 1);
    int a[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(read[i], NULL);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);
    return 0;
}
