#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h> // for sleep function
#include <chrono>

using namespace std;

#define MAX_BUFFER_SIZE 10

// Shared data structures
queue<int> buffer;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_full = PTHREAD_COND_INITIALIZER;

// Function prototypes
void* producer(void* arg);
void* consumer(void* arg);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <num_producers> <num_consumers> <sleep_time>" << endl;
        return 1;
    }

    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);
    int sleep_time = atoi(argv[3]);

    pthread_t producer_threads[num_producers];
    pthread_t consumer_threads[num_consumers];

    // Create producer threads
    for (int i = 0; i < num_producers; ++i) {
        pthread_create(&producer_threads[i], NULL, producer, NULL);
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);
    }

    // Join producer threads
    for (int i = 0; i < num_producers; ++i) {
        pthread_join(producer_threads[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        pthread_join(consumer_threads[i], NULL);
    }

    return 0;
}

void* producer(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);

        // Produce item
        int item = rand() % 100;

        // Wait if buffer is full
        while (buffer.size() == MAX_BUFFER_SIZE) {
            pthread_cond_wait(&buffer_full, &mutex);
        }

        // Add item to buffer
        buffer.push(item);
        cout << "Produced: " << item << endl;

        // Signal consumers
        pthread_cond_signal(&buffer_empty);

        pthread_mutex_unlock(&mutex);

        sleep(1); // sleep for some time
    }
    return NULL;
}

void* consumer(void* arg) {
    while (true) {
        pthread_mutex_lock(&mutex);

        // Wait if buffer is empty
        while (buffer.empty()) {
            pthread_cond_wait(&buffer_empty, &mutex);
        }

        // Consume item
        int item = buffer.front();
        buffer.pop();
        cout << "Consumed: " << item << endl;

        // Signal producers
        pthread_cond_signal(&buffer_full);

        pthread_mutex_unlock(&mutex);

        sleep(1); // sleep for some time
    }
    return NULL;
}
