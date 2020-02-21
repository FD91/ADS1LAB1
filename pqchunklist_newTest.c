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

#define MAX_ELEMS_PER_BLOCK 4

/* Prototypes for static functions
 * cellT creatCellT(int newValue);
 * void splitCellT(cellT oldNode);
 */



typedef struct cellT {
	int nElements;
	int array[MAX_ELEMS_PER_BLOCK];
	struct cellT* link;
} *cellT;

struct pqueueCDT {
	cellT head;
};

static cellT creatCellT(int newValue) {
	cellT newNode;

	newNode = New(cellT);
	newNode->nElements = 1;
	newNode->array[0] = newValue;
	newNode->link = NULL;
}


static void splitCellT(cellT currentNode) {
	cellT newNode;
	int i, j;

	newNode = New(cellT);
	newNode->nElements = 1;

	j = 0;
	for (i = MAX_ELEMS_PER_BLOCK / 2; i < MAX_ELEMS_PER_BLOCK; i++) {
		newNode->array[j] = currentNode->array[i];
		j++;
	}
	newNode->link = currentNode;
	currentNode = newNode;

	currentNode->nElements = MAX_ELEMS_PER_BLOCK / 2;
	newNode->nElements = MAX_ELEMS_PER_BLOCK - currentNode->nElements;
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
		newNode = creatCellT(newValue);
		pqueue->head = newNode;
	}
	else {
		while (currentNode) {
			if (currentNode->array[0] <= newValue || !currentNode->link) {
				if (currentNode->nElements == MAX_ELEMS_PER_BLOCK) {
					splitCellT(currentNode, lastNode);
					if (newValue <= currentNode->link->array[currentNode->nElements - 1]) {
						currentNode = currentNode->link;
					}
				}
				currentNode->array[(currentNode->nElements)++] = newValue;
				for (i = currentNode->nElements - 1; i > 0; i--) {
					if (currentNode->array[i] < currentNode->array[i - 1]) {
						temp = currentNode->array[i];
						currentNode->array[i] = currentNode->array[i - 1];
						currentNode->array[i - 1] = temp;
					}
					else break;
				}
				return;
			}
			else {
				lastNode = currentNode;
				currentNode = currentNode->link;
			}
		}
		newNode = creatCellT(newValue);
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
		value = pqueue->head->array[--(pqueue->head->nElements)];
		if (pqueue->head->nElements == 0) {
			temp = pqueue->head;
			pqueue->head = pqueue->head->link;
			free(temp);
		}
	}
	return value;
}
