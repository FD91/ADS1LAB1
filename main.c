#include "pqueue.h";
#include "random.h";

main() {
	int i;

	pqueueADT queue;
	queue = NewPQueue();
	Randomize();
	Randomize();

	printf("Adds 100 items\n\n");
	for (i = 0; i < 3; i++) {
		Enqueue(queue, RandomInteger(0, 100));
	}
	
	printf("Dequeue 90 items\n\n");
	for (i = 0; i < 2; i++) {
		printf("%d\n", DequeueMax(queue));
	}

	printf("Adds 5 items\n\n");
	for (i = 0; i < 1; i++) {
		Enqueue(queue, RandomInteger(0, 100));
	}

	printf("Dequeue 15 items\n\n");
	for (i = 0; i < 1; i++) {
		printf("%d\n", DequeueMax(queue));
	}

	if (IsEmpty(queue)) {
		printf("Empty");
	}
	else (printf("Not empty"));
}