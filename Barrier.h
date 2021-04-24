#ifndef BARRIER_H_
#define BARRIER_H_

#include <semaphore.h>

class Barrier {
public:
    Barrier(unsigned int num_of_threads);
    void wait();
	unsigned int waitingThreads();
    ~Barrier();

	// TODO: define the member variables
	// Remember: you can only use semaphores!
	sem_t sema1, sema2, lock1, lock2;
	int num_of_threads, counter1, counter2;
	bool flag;
};

#endif // BARRIER_H_

