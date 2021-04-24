#include "Barrier.h"

Barrier::Barrier(unsigned int num_of_threads) : num_of_threads(num_of_threads), counter1(1), counter2(1), flag(true) {
	sem_init(&sema1, 0, 0);
	sem_init(&sema2, 0, 0);
	sem_init(&lock1, 0, 1);
	sem_init(&lock2, 0, 1);
}

void Barrier::wait() {
	if (flag) {
		sem_wait(&lock1);
		if (counter1 != num_of_threads) {
			counter1++;
			sem_post(&lock1);
			sem_wait(&sema1);
		} else {
			sem_post(&lock1);
			flag = false;
			for (int i = 1; i < num_of_threads; i++) {
				sem_post(&sema1);
				sem_wait(&lock1);
				counter1--;
				sem_post(&lock1);
			}
		}
	} else {
		sem_wait(&lock2);
		if (counter2 != num_of_threads) {
			counter2++;
			sem_post(&lock2);
			sem_wait(&sema2);
		} else {
			sem_post(&lock2);
			flag = true;
			for (int i = 1; i < num_of_threads; i++) {
				sem_post(&sema2);
				sem_wait(&lock2);
				counter2--;
				sem_post(&lock2);
			}
		}
	}
}

unsigned int Barrier::waitingThreads() {
	return num_of_threads;
}

Barrier::~Barrier() {
	sem_destroy(&sema1);
	sem_destroy(&sema2);
	sem_destroy(&lock1);
	sem_destroy(&lock2);
}

