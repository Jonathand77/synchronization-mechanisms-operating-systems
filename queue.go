package main

import (
	"fmt"
	"sync"
	"time"
)

type ThreadSafeQueue struct {
	items     []int
	capacity  int
	lock      sync.Mutex
	notEmpty  *sync.Cond
	notFull   *sync.Cond
}

func NewQueue(capacity int) *ThreadSafeQueue {
	q := &ThreadSafeQueue{
		items:    make([]int, 0),
		capacity: capacity,
	}
	q.notEmpty = sync.NewCond(&q.lock)
	q.notFull = sync.NewCond(&q.lock)
	return q
}

func (q *ThreadSafeQueue) Enqueue(item int) {
	q.lock.Lock()
	for len(q.items) == q.capacity {
		q.notFull.Wait()
	}
	q.items = append(q.items, item)
	fmt.Println("Enqueued:", item)
	q.notEmpty.Signal()
	q.lock.Unlock()
}

func (q *ThreadSafeQueue) Dequeue() int {
	q.lock.Lock()
	for len(q.items) == 0 {
		q.notEmpty.Wait()
	}
	item := q.items[0]
	q.items = q.items[1:]
	fmt.Println("Dequeued:", item)
	q.notFull.Signal()
	q.lock.Unlock()
	return item
}

func producer(q *ThreadSafeQueue, id int) {
	for i := 0; i < 10; i++ {
		q.Enqueue(i + id*100)
		time.Sleep(500 * time.Millisecond)
	}
}

func consumer(q *ThreadSafeQueue, id int) {
	for i := 0; i < 10; i++ {
		item := q.Dequeue()
		fmt.Printf("Consumer %d processed item: %d\n", id, item)
		time.Sleep(1 * time.Second)
	}
}

func main() {
	q := NewQueue(5)

	go producer(q, 0)
	go producer(q, 1)
	go consumer(q, 0)
	go consumer(q, 1)

	time.Sleep(15 * time.Second)
}
