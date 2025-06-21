#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_THREAD 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int produce_item(int id, int count) {
    return id * 100 + count;
}

void consume_item(int item, int id) {
    std::cout << "Consumer " << id << " consumed item: " << item << std::endl;
}

void* producer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; ++i) {
        int item = produce_item(id, i);
        sem_wait(&empty);  // Esperar si el buffer esta lleno
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        std::cout << "Producer " << id << " produced item: " << item << std::endl;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);  // Señalar que un articulo esta disponible
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < ITEMS_PER_THREAD; ++i) {
        sem_wait(&full);  // Esperar si el buffer está vacío
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);  // Señal de que una ranura esta libre

        consume_item(item, id);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int ids[NUM_PRODUCERS > NUM_CONSUMERS ? NUM_PRODUCERS : NUM_CONSUMERS];

    // Inicialización
    sem_init(&empty, 0, BUFFER_SIZE); // buffer vacío inicialmente
    sem_init(&full, 0, 0);            // nada lleno
    pthread_mutex_init(&mutex, NULL);

    // Crear productores y consumidores
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &ids[i]);
    }

    // Esperar a que terminen
    for (int i = 0; i < NUM_PRODUCERS; ++i) pthread_join(producers[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; ++i) pthread_join(consumers[i], NULL);

    // Liberar recursos
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
