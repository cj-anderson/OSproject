#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <queue>
#include <chrono>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

const int BUFFER_SIZE = 10;
const int NUM_ITEMS = 100; // Total number of items to be produced
queue<int> buffer;
sem_t empty_sem, full;
pthread_mutex_t mutex;
int numItemsProduced = 0;
int numItemsConsumed = 0;

void *producer(void *param) 
{
    int sleepTime = *((int *)param);

    while (true) 
    {
        if (numItemsProduced >= NUM_ITEMS) 
        {
            pthread_exit(NULL);
        }

        sem_wait(&empty_sem);
        pthread_mutex_lock(&mutex);
        
        if (numItemsProduced < NUM_ITEMS) 
        {
            int item = numItemsProduced++;
            buffer.push(item);
            cout << "Produced: " << item << endl;
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(sleepTime); // sleep for specified time
    }
}

void *consumer(void *param) 
{
    int sleepTime = *((int *)param);

    while (true) {
        if (numItemsConsumed >= NUM_ITEMS) 
        {
            pthread_exit(NULL);
        }

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (!buffer.empty()) 
        {
            int item = buffer.front();
            buffer.pop();
            numItemsConsumed++;
            cout << "Consumed: " << item << endl;
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_sem);
        sleep(sleepTime); //sleep for specified time
    }
}

int main(int argc, char *argv[]) 
{
    
    //Parameters: <producers> <consumers> <sleep_time>
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <Number of Producers:> <Number of Consumers:> <Sleep_Time:>" << endl;
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
    auto start = high_resolution_clock::now();

    //Create producer threads
    for (int i = 0; i < numProducers; i++) 
    {
        pthread_create(&producers[i], NULL, producer, (void *)&sleepTime);
    }

    //Creates consumer threads
    for (int i = 0; i < numConsumers; i++) 
    {
        pthread_create(&consumers[i], NULL, consumer, (void *)&sleepTime);
    }

    //Joins the producer threads
    for (int i = 0; i < numProducers; i++) 
    {
        pthread_join(producers[i], NULL);
    }

    //Join consumer threads
    for (int i = 0; i < numConsumers; i++) 
    {
        pthread_join(consumers[i], NULL);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Total turnaround time: " << duration.count() << " microseconds" << endl;
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_sem);
    sem_destroy(&full);

    return 0;
}
