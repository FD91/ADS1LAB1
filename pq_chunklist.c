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

#define MAX_ELEMS_PER_BLOCK 3

typedef struct cellT {
	int nElements;
	int* array;
	struct cellT* link;
} *cellT;

struct pqueueCDT {
	cellT head;
};

static splitCellT(cellT oldNode) {
	cellT newNode;
	int i, j;

	newNode = New(cellT);
	newNode->nElements = 1;
	newNode->array = NewArray(MAX_ELEMS_PER_BLOCK, int);

	j = 0;
	for (i = MAX_ELEMS_PER_BLOCK / 2; i < MAX_ELEMS_PER_BLOCK; i++) {
		newNode->array[j] = oldNode->array[i];
		j++;
	}
	newNode->link = oldNode->link;
	oldNode->link = newNode;
	
	oldNode->nElements = MAX_ELEMS_PER_BLOCK / 2;
	newNode->nElements = MAX_ELEMS_PER_BLOCK - oldNode->nElements;
}

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
	int i, temp;

	currentNode = pqueue->head;
	lastNode = NULL;

	if (IsEmpty(pqueue)) {
		// Lägga in i egen funktion?
		newNode = New(cellT);
		newNode->nElements = 1;
		newNode->array = NewArray(MAX_ELEMS_PER_BLOCK, int);
		newNode->array[0] = newValue;
		newNode->link = NULL;
		pqueue->head = newNode;
	}
	else {
		while (currentNode) {
			if (currentNode->array[currentNode->nElements - 1] <= newValue) {
				// Kolla om man behöver expandera och iså fall Länka om lastNode
				if (currentNode->nElements == MAX_ELEMS_PER_BLOCK) {
					splitCellT(currentNode);
					currentNode = currentNode->link; // Ny
				}
				currentNode->array[(currentNode->nElements)++] = newValue;
				for (i = currentNode->nElements - 1; i > 0; i--) {
					if (currentNode->array[i] > currentNode->array[i - 1]) {
						temp = currentNode->array[i];
						currentNode->array[i] = currentNode->array[i - 1];
						currentNode->array[i - 1] = temp;
					}
					else break;
				}
				return;
			}
			else { // gå till nästa
				lastNode = currentNode;
				currentNode = currentNode->link;
			}
		}
		// Lägga in i egen funktion?
		newNode = New(cellT);
		newNode->nElements = 1;
		newNode->array = NewArray(MAX_ELEMS_PER_BLOCK, int);
		newNode->array[0] = newValue;
		newNode->link = NULL;
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
	int i;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");
	else {
		value = pqueue->head->array[0];
		pqueue->head->nElements--;
		for (i = 0; i < pqueue->head->nElements; i++) { // changed <= to <
			pqueue->head->array[i] = pqueue->head->array[i + 1];
		}
		if (pqueue->head->nElements == 0) {
			temp = pqueue->head;
			pqueue->head = pqueue->head->link;
			free(temp);
		}
	}
	return value;
}
