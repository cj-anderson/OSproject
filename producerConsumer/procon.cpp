#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <queue>
#include <unistd.h> // for sleep function
#include <chrono>

using namespace std;

const int BUFFER_SIZE = 10;

queue<int> buffer;
sem_t empty_sem, full;
pthread_mutex_t mutex;

void *producer(void *param) {
    int item;
    while (true) {
        item = rand() % 100; // generate random item
        sem_wait(&empty_sem);
        pthread_mutex_lock(&mutex);
        buffer.push(item);
        cout << "Produced: " << item << endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(*((int *)param)); // sleep for specified time
    }
}

void *consumer(void *param) {
    int item;
    while (true) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        item = buffer.front();
        buffer.pop();
        cout << "Consumed: " << item << endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_sem);
        sleep(*((int *)param)); // sleep for specified time
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <#producers> <#consumers> <sleep_time>" << endl;
        return 1;
    }

    int numProducers = atoi(argv[1]);
    int numConsumers = atoi(argv[2]);
    int sleepTime = atoi(argv[3]);

    pthread_t producers[numProducers];
    pthread_t consumers[numConsumers];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_sem, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    srand(time(NULL));

    // Create producer threads
    for (int i = 0; i < numProducers; i++) {
        pthread_create(&producers[i], NULL, producer, (void *)&sleepTime);
    }

    // Create consumer threads
    for (int i = 0; i < numConsumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, (void *)&sleepTime);
    }

    // Join producer threads
    for (int i = 0; i < numProducers; i++) {
        pthread_join(producers[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < numConsumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_sem);
    sem_destroy(&full);

    return 0;
}

