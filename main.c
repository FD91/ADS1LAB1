#include "pqueue.h";
#include "random.h";

main() {
	int i;
	int temp;

	pqueueADT queue;
	queue = NewPQueue();
	Randomize();
	Randomize();

	for (i = 0; i < 100000; i++) {
		temp = RandomInteger(1, 100);
		printf("Adding: %d\n", temp);
		Enqueue(queue, temp);
	}
	
	for (i = 0; i < 10000; i*=1000) {
		printf("Dequeue returned: %d\n", DequeueMax(queue));
	}

	if (IsEmpty(queue)) {
		printf("Empty");
	}
	else (printf("Not empty"));
}
