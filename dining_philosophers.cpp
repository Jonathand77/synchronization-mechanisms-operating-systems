#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>

#define NUM_PHILOSOPHERS 5
#define EAT_TIMES 3

pthread_mutex_t forks[NUM_PHILOSOPHERS];
sem_t max_eaters;
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;  // Mutex para impresión segura

void safe_print(const std::string& msg) {
    pthread_mutex_lock(&print_lock);
    std::cout << msg << std::endl;
    pthread_mutex_unlock(&print_lock);
}

void think(int id) {
    safe_print("Philosopher " + std::to_string(id) + " is thinking.");
    sleep(rand() % 2 + 1); // 1-2 segundos
}

void eat(int id) {
    safe_print("Philosopher " + std::to_string(id) + " is eating.");
    sleep(rand() % 2 + 1); // 1-2 segundos
}

void* philosopher(void* arg) {
    int id = *(int*)arg;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < EAT_TIMES; ++i) {
        think(id);

        // Esperar permiso para intentar comer
        sem_wait(&max_eaters);

        // Tomar tenedores (en orden)
        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);

        eat(id);

        // Soltar tenedores
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);

        // Señalar que terminó de comer
        sem_post(&max_eaters);
    }

    safe_print("Philosopher " + std::to_string(id) + " is done eating.");
    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    srand(time(NULL));

    // Inicializar tenedores
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Inicializar semáforo (N-1 filósofos pueden comer al mismo tiempo)
    sem_init(&max_eaters, 0, NUM_PHILOSOPHERS - 1);

    // Crear hilos de filósofos
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // Esperar a que terminen
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Limpiar recursos
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_destroy(&forks[i]);
    }
    sem_destroy(&max_eaters);
    pthread_mutex_destroy(&print_lock);

    return 0;
}
