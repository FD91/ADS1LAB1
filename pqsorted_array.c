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

//Static Functions - OBS kolla med Cecilia om vi ska l�gga dessa i H-filen

static void expandArray(pqueueADT pqueue);

 /* Constant: MAX_ELEMENTS
  * ----------------------
  * Den h�r konstanten anger antalet element i den vektor som
  * utg�r representationen av prioritetsk�n.
  */

#define START_MAX_ELEMENTS 1

struct pqueueCDT {
	int *entries;
	int current_max_size;
	int numEntries;
};

/* Exported endries */

pqueueADT NewPQueue(void)
{
	pqueueADT pqueue;

	pqueue = New(pqueueADT);

	pqueue->current_max_size = START_MAX_ELEMENTS;
	pqueue->entries = NewArray(pqueue->current_max_size, int);
	pqueue->numEntries = 0;
	return (pqueue);
}

void FreePQueue(pqueueADT pqueue)
{
	FreeBlock(pqueue);
}

bool IsEmpty(pqueueADT pqueue)
{
	return (pqueue->numEntries == 0);
}

bool IsFull(pqueueADT pqueue)
{
	return (pqueue->numEntries == pqueue->current_max_size);
}

/*
 * Implementation notes: Enqueue
 * -----------------------------
 * D� elementen sparas osorterat i f�ltet beh�ver endast nya
 * elementet placeras i slutet av f�ltet.
 */

void Enqueue(pqueueADT pqueue, int newValue)
{
	int i, temp;

	if (IsFull(pqueue)) {
		expandArray(pqueue);
	}

	pqueue->entries[(pqueue->numEntries)++] = newValue;

	for (i = pqueue->numEntries - 1; i > 0; i--) {
		if (pqueue->entries[i] < pqueue->entries[i - 1]) {
			// Switchar elementen
			temp = pqueue->entries[i];
			pqueue->entries[i] = pqueue->entries[i - 1];
			pqueue->entries[i - 1] = temp;
		}
		else {
			break;
		}
	}

	
}

int DequeueMax(pqueueADT pqueue)
{
	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");

	return pqueue->entries[--(pqueue->numEntries)];
}

static void expandArray(pqueueADT pqueue) {
	int i;
	int* expanded;

	expanded = NewArray(pqueue->current_max_size * 2, int);

	for (i = 0; i < pqueue->numEntries; i++) {
		expanded[i] = pqueue->entries[i];
	}

	free(pqueue->entries);
	pqueue->entries = expanded;
	pqueue->current_max_size *= 2;
}