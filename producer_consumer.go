package main

import (
	"fmt"
	"sync"
	"time"
)

const bufferSize = 10
var buffer = make([]int, bufferSize)
var in, out int

var empty = make(chan struct{}, bufferSize)
var full = make(chan struct{}, bufferSize)
var mutex sync.Mutex

func produceItem(id, count int) int {
	return id*100 + count
}

func consumeItem(item, id int) {
	fmt.Printf("Consumer %d consumed item: %d\n", id, item)
}

func producer(id int, wg *sync.WaitGroup) {
	defer wg.Done()
	for i := 0; i < 10; i++ {
		item := produceItem(id, i)

		<-empty // Esperar a que haya una ranura vacía

		mutex.Lock()
		buffer[in] = item
		in = (in + 1) % bufferSize
		fmt.Printf("Producer %d produced item: %d\n", id, item)
		mutex.Unlock()

		full <- struct{}{} // Señal de ranura llena
		time.Sleep(500 * time.Millisecond)
	}
}

func consumer(id int, wg *sync.WaitGroup) {
	defer wg.Done()
	for i := 0; i < 10; i++ {
		<-full // Espere a que se llene el espacio

		mutex.Lock()
		item := buffer[out]
		out = (out + 1) % bufferSize
		mutex.Unlock()

		empty <- struct{}{} // Señal de ranura vacía
		consumeItem(item, id)

		time.Sleep(1 * time.Second)
	}
}

func main() {
	var wg sync.WaitGroup

	// Llene los espacios VACÍOS después de la inicialización del consumidor
	// (Inicialmente, todos los espacios están vacíos)
	for i := 0; i < bufferSize; i++ {
		empty <- struct{}{}
	}

	wg.Add(4) // 2 productores + 2 consumidores
	go producer(0, &wg)
	go producer(1, &wg)
	go consumer(0, &wg)
	go consumer(1, &wg)

	wg.Wait()
}
