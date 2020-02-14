/*
 * File: pqueue.c
 * --------------
 * Den här filen implementerar en prioritetskö med en
 * vektor med konstant storlek. Implementeringen gör det lätt
 * att sätta in nya element med svårt att plocka ut det största.
 *
 * Cecilia Sönströd, Algoritmer och Datastrukturer 1, vt2020
 */

#include "pqueue.h"
#include "genlib.h"

 /* Constant: MAX_ELEMENTS
  * ----------------------
  * Den här konstanten anger antalet element i den vektor som
  * utgör representationen av prioritetskön.
  */

#define MAX_ELEMENTS 1200000

typedef struct cellT {
	int value;
	struct cellT* link;
} *cellT;

struct pqueueCDT {
	cellT* head;
};

/* Exported endries */

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);
	pqueue->head = NULL;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return pqueue->head == NULL;
}

bool IsFull(pqueueADT pqueue)
{
	return FALSE;
}

void Enqueue(pqueueADT pqueue, int newValue)
{
	cellT newNode, lastNode, currentNode;

	newNode = New(cellT);
	newNode->value = newValue;
	newNode->link = NULL;

	currentNode = NULL;
	lastNode = pqueue->head; // antagligen fel, kanske NULL

	if (IsEmpty(pqueue)) {
		pqueue->head = newNode;
	}
	else {
		currentNode = pqueue->head;
		while (currentNode != NULL) {
			if (currentNode->value <= newNode->value) {
				newNode->link = currentNode;
				lastNode = newNode;
			}
			else {
				lastNode = currentNode;
				currentNode = currentNode->link;
			}
		}
		lastNode->link = newNode;
	}
}

/*
 * Implementation notes: DequeueMax
 * -------------------------------------------------
 * Då elementen sparas osorterat i fältet måste en sökning göras
 * för att finna det största elementet. För att ta bort det största
 * elementet flyttas det sista elementet i fältet till den position
 * i vilken det största elementet återfanns samtidigt som antalet
 * element i fältet minskas med 1. Det värde som tas bort returneras från
 * funktionen.
 */

int DequeueMax(pqueueADT pqueue)
{
	cellT temp;
	int value = 0;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");
	else{
		temp = pqueue->head;
		value = temp->value;
		pqueue->head = temp->link;
		free(temp);
	}
	return value;
}
