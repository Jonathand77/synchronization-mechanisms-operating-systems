package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const numPhilosophers = 5
const eatCount = 3

var forks [numPhilosophers]sync.Mutex
var eatLimit = make(chan struct{}, numPhilosophers-1)
var printLock sync.Mutex

func log(msg string) {
	printLock.Lock()
	fmt.Println(msg)
	printLock.Unlock()
}

func philosopher(id int, wg *sync.WaitGroup) {
	left := id
	right := (id + 1) % numPhilosophers
	for i := 0; i < eatCount; i++ {
		log(fmt.Sprintf("Philosopher %d is thinking", id))
		time.Sleep(time.Duration(rand.Intn(2)+1) * time.Second)

		eatLimit <- struct{}{} // espera
		forks[left].Lock()
		forks[right].Lock()

		log(fmt.Sprintf("Philosopher %d is eating", id))
		time.Sleep(time.Duration(rand.Intn(2)+1) * time.Second)

		forks[right].Unlock()
		forks[left].Unlock()
		<-eatLimit // Liberar

		log(fmt.Sprintf("Philosopher %d finished eating", id))
	}
	wg.Done()
}

func main() {
	rand.Seed(time.Now().UnixNano())
	var wg sync.WaitGroup

	for i := 0; i < numPhilosophers; i++ {
		wg.Add(1)
		go philosopher(i, &wg)
	}

	wg.Wait()
}
