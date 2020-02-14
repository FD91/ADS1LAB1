/*
 * File: pqueue.c
 * --------------
 * Den h�r filen implementerar en prioritetsk� med en
 * vektor med konstant storlek. Implementeringen g�r det l�tt
 * att s�tta in nya element med sv�rt att plocka ut det st�rsta.
 *
 * Cecilia S�nstr�d, Algoritmer och Datastrukturer 1, vt2020
 */

#include "pqueue.h"
#include "genlib.h"

 /* Constant: MAX_ELEMENTS
  * ----------------------
  * Den h�r konstanten anger antalet element i den vektor som
  * utg�r representationen av prioritetsk�n.
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
 * D� elementen sparas osorterat i f�ltet m�ste en s�kning g�ras
 * f�r att finna det st�rsta elementet. F�r att ta bort det st�rsta
 * elementet flyttas det sista elementet i f�ltet till den position
 * i vilken det st�rsta elementet �terfanns samtidigt som antalet
 * element i f�ltet minskas med 1. Det v�rde som tas bort returneras fr�n
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
