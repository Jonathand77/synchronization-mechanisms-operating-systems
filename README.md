# synchronization-mechanisms-operating-systems

![GitHub repo size](https://img.shields.io/github/repo-size/jonathand77/synchronization-mechanisms-operating-systems)
![GitHub contributors](https://img.shields.io/github/contributors/jonathand77/synchronization-mechanisms-operating-systems)
![GitHub last commit](https://img.shields.io/github/last-commit/jonathand77/synchronization-mechanisms-operating-systems)
![License](https://img.shields.io/badge/license-BSD%202--Clause-blue)
![Languages](https://img.shields.io/github/languages/count/jonathand77/synchronization-mechanisms-operating-systems)

## 👥 Integrantes

| 👨‍💻 Nombre | 📧 Correo | 🐙 Usuario GitHub |
|---|---|---|
| **Jonathan David Fernandez Vargas** | jonathand.fernandez@udea.edu.co | [jonathand77](https://github.com/jonathand77) |
| **Valeria Alvarez Fernandez** | valeria.alvarezf@udea.edu.co | [vaf88](https://github.com/vaf88) |

---

## 🧠 Objective
Understand and implement synchronization mechanisms (locks, condition variables, and semaphores) to solve concurrency problems in multi-threaded programs.

## Introduction
In multi-threaded programs, shared resources must be accessed in a controlled  manner to avoid race conditions. This lab explores three synchronization mecha nisms:  Locks (Mutexes), Condition Variables: Allow threads to wait for a condition to become true, Semaphores: General synchronization primitives that can control access  to resources.

 ---

## 🔧 Task 1: Thread-Safe Queue (Using `pthread_mutex_t` and `pthread_cond_t`)

### ✅ Description
Implemented a fixed-size, thread-safe queue that supports:
- Multiple producer and consumer threads.
- Blocking consumers when the queue is empty.
- Blocking producers when the queue is full.

### 🛠 Mechanisms Used
- `pthread_mutex_t`: Ensures mutual exclusion when accessing the queue.
- `pthread_cond_t`: Used to wait for or signal conditions (`not_empty`, `not_full`).
- **C++:** `pthread_mutex_t`, `pthread_cond_t`
- **Go:** `sync.Mutex`, `sync.Cond`

### 💡 Key Features
- Circular buffer implementation.
- Supports multiple producers and consumers.
- Proper signaling to avoid busy waiting.

### 📁 Files
`queue_pthread.cpp`
`queue.go`

---

## 🔧 Task 2: Producer-Consumer Problem (Using `sem_t` and `pthread_mutex_t`)

### ✅ Description
Implemented the classic bounded-buffer problem:
- Multiple producers add items to a shared buffer.
- Multiple consumers remove items.
- No overflow or underflow.

### 🛠 Mechanisms Used
- `sem_t empty`: Counts available slots in the buffer.
- `sem_t full`: Counts filled slots.
- `pthread_mutex_t mutex`: Ensures exclusive access to buffer indexes.
- **C++:** `sem_t empty`, `sem_t full`, `pthread_mutex_t mutex`
- **Go:** Buffered/unbuffered channels for semaphores, `sync.Mutex`

### 💡 Key Features
- Producers wait if the buffer is full (`sem_wait(&empty)`).
- Consumers wait if the buffer is empty (`sem_wait(&full)`).
- Mutex ensures race-free access to the buffer.

### 📁 Files
`producer_consumer.cpp`
`producer_consumer.go`

---

## 🔧 Task 3: Dining Philosophers (Deadlock-Free with `sem_t` and `pthread_mutex_t`)

### ✅ Description
Implemented the Dining Philosophers problem ensuring:
- No deadlocks.
- No starvation.

### 🛠 Mechanisms Used
- `pthread_mutex_t forks[5]`: One mutex per fork.
- `sem_t max_eaters`: Limits the number of concurrent eating philosophers to N-1 (4), preventing deadlock.
- `pthread_mutex_t print_lock`: Ensures safe, readable console output.
- **C++:** `pthread_mutex_t forks[5]`, `sem_t max_eaters`, `pthread_mutex_t print_lock`
- **Go:** Array de `sync.Mutex` para tenedores, canal con capacidad N-1 para limitar filósofos comiendo, `sync.Mutex` para impresión

### 💡 Key Features
- Philosophers alternate between thinking and eating.
- Global semaphore enforces a concurrency constraint to break circular wait.
- Print output is synchronized for clarity.

### 📁 Files
`dining_philosophers.cpp`
`dining_philosophers.go`

---

## ▶️ How to Compile and Run

### C++
```bash
# Task 1
g++ -pthread queue_pthread.cpp -o queue_pthread.x
./queue_pthread.x

# Task 2
g++ -pthread producer_consumer.cpp -o producer_consumer.x
./producer_consumer.x

# Task 3
g++ -pthread dining_philosophers.cpp -o dining_philosophers
./dining_philosophers.x
```

### Go
```bash
# Task 1
go run queue.go

# Task 2
go run producer_consumer.go

# Task 3
go run dining_philosophers.go
```

---

## 📌 Requirements Met

- ✅ Use of `pthread_mutex_t` and `pthread_cond_t` in Task 1.
- ✅ Use of `sem_t` semaphores in Task 2.
- ✅ Deadlock-free, starvation-free solution for Dining Philosophers in Task 3.
- ✅ Tested with multiple threads.
- ✅ All source code written in C++ with POSIX synchronization primitives.

---

## 🧪 Example Outputs

### 🔹 Task 1: Thread-Safe Queue

**C++**
```
Enqueued: 0
Enqueued: 100
Dequeued: 0
Consumer 0 processed item: 0
Dequeued: 100
Consumer 1 processed item: 100
...
```

**Go**
```
Enqueued: 0
Enqueued: 100
Dequeued: 0
Consumer 0 processed item: 0
Dequeued: 100
Consumer 1 processed item: 100
...
```

### 🔹 Task 2: Producer-Consumer with Semaphores

**C++**
```
Producer 0 produced item: 0
Producer 1 produced item: 100
Consumer 0 consumed item: 0
Consumer 1 consumed item: 100
...
```

**Go**
```
Producer 0 produced item: 0
Producer 1 produced item: 100
Consumer 0 consumed item: 0
Consumer 1 consumed item: 100
...
```

### 🔹 Task 3: Dining Philosophers (Deadlock-Free)

**C++**
```
Philosopher 0 is thinking.
Philosopher 1 is thinking.
Philosopher 2 is thinking.
Philosopher 0 is eating.
Philosopher 1 is eating.
Philosopher 0 is thinking.
Philosopher 2 is eating.
...
Philosopher 4 is done eating.
```

**Go**
```
Philosopher 0 is thinking
Philosopher 1 is thinking
Philosopher 2 is thinking
Philosopher 0 is eating
Philosopher 1 is eating
Philosopher 0 finished eating
Philosopher 2 is eating
...
Philosopher 4 finished eating
```

---

## 🧾 Conclusions

- Proper use of synchronization primitives is essential for avoiding race conditions, deadlocks, and starvation in concurrent programs.
- Task 1 demonstrated how condition variables and mutexes can safely manage a bounded queue shared between multiple threads.
- Task 2 showed how semaphores can elegantly solve the producer-consumer problem by tracking buffer capacity and usage.
- Task 3 highlighted the importance of limiting resource access (via semaphore) to break potential circular wait and avoid deadlock in classic synchronization scenarios like the Dining Philosophers.
- Clear and synchronized output printing (using a mutex) is crucial in multithreaded applications to maintain readable logs and debug effectively.

This lab reinforces practical understanding of concurrency control, which is fundamental for building reliable multi-threaded systems.

---