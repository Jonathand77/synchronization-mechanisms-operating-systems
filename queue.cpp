#include <pthread.h>
#include <queue>
#include <iostream>
#include <unistd.h> // for sleep()

#define QUEUE_CAPACITY 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_THREAD 10

class ThreadSafeQueue {
private:
    std::queue<int> queue;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

public:
    ThreadSafeQueue() {
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&not_empty, NULL);
        pthread_cond_init(&not_full, NULL);
    }

    ~ThreadSafeQueue() {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&not_empty);
        pthread_cond_destroy(&not_full);
    }

    void enqueue(int item) {
        pthread_mutex_lock(&lock);
        while (queue.size() >= QUEUE_CAPACITY) {
            pthread_cond_wait(&not_full, &lock);
        }
        queue.push(item);
        std::cout << "Enqueued: " << item << std::endl;
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }

    int dequeue() {
        pthread_mutex_lock(&lock);
        while (queue.empty()) {
            pthread_cond_wait(&not_empty, &lock);
        }
        int item = queue.front();
        queue.pop();
        std::cout << "Dequeued: " << item << std::endl;
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
        return item;
    }
};

ThreadSafeQueue q;

void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; ++i) {
        int item = i + id * 100;
        q.enqueue(item);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; ++i) {
        int item = q.dequeue();
        std::cout << "Consumer " << id << " got item: " << item << std::endl;
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int ids[NUM_PRODUCERS > NUM_CONSUMERS ? NUM_PRODUCERS : NUM_CONSUMERS];

    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_join(consumers[i], NULL);
    }

    return 0;
}
