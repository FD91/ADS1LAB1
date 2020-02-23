/*
* File: pqsorted_array.c
* ---------------
* Author: Jonathan Thornander, Christoffer Ivarsson Orrelid, Fredrik Dhami
*
* Task: Detta program implementerar en prioritetskö
* i form av en dynamiskt minnesallokerad array. Listan
* räknar aktuellt antal element mot ett maxantal som
* anges via en förbestämd konstant och expanderar sedan
* vid behov för att förhindra att den blir full.
*
* Prioritetskön tar endast heltalsvärden som input
* och sorterar dem i stigande ordning där minst tal
* hamnar först och det största talet tas ut först vid
* Dequeue.
*/

#include "pqueue.h"
#include "genlib.h"

 /* Constant: MAX_ELEMENTS
  * ----------------------
  * Den här konstanten anger det initiala maxantalet
  * element i den dynamiskt allokerade vektorn.
  */

#define START_MAX_ELEMENTS 10

  /* Type definition: pqueueCDT
   * --------------------
   * Typen pqueueCDT är den konkreta implementeringen av
   * den exporterade typen pqueueADT. pqueueCDT innehåller
   * en int-pekare som fungerar som själva listan och två
   * variabler för aktuellt maxantal element (föränderligt
   * allteftersom listan expanderar) och faktiskt antal
   * element i listan.
   */
struct pqueueCDT {
	int *entries;
	int current_max_size;
	int numEntries;
};

/* Exported endries */
/*
* Function: NewPQueue
* Usage: pqueue = NewPQueue();
* -------------------------------
* Allokerar lämpligt minne till en kö
* av pekartyp pqueueADT och sätter
* initialvärden.
*/
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
* Function: Enqueue
* Usage: Enqueue(pqueue, newValue);
* -------------------------------
* Denna funktion fyller ut prioritetskön med ett nytt värde
* valt av användaren. Funktionen testar om kön är full för
* att då expandera den. Sedan läggs det nya värdet till på
* sista lediga plats.
*
* Funktionen sorterar därefter listan, inklusive det nya
* värdet, så att alla element sätts in i stigande ordning
* med minsta värde först.
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
			temp = pqueue->entries[i];
			pqueue->entries[i] = pqueue->entries[i - 1];
			pqueue->entries[i - 1] = temp;
		}
		else {
			break;
		}
	}

	
}

/*
* Function: DequeueMax
* Usage: DequeueMax(pqueue);
* -------------------------------
* Då elementen sparas osorterat i fältet måste en sökning göras
* för att finna det största elementet. Felhantering fångar upp
* om funktionen försöker köras om listan är tom då funktionen
* skall returnera, sen bortse från, det största elementet i hela
* prioritetskön.
*/
int DequeueMax(pqueueADT pqueue)
{
	if (IsEmpty(pqueue))
		Error((string)"Tried to dequeue max from an empty pqueue!");

	return pqueue->entries[--(pqueue->numEntries)];
}

/*
* Function: expandArray
* Usage: expandArray(pqueue);
* -------------------------------
* Denna funktion skapar en ny array dubbelt så
* stor som den föregående (pekare på dynamiskt
* allokerat minnesutrymme), skriver in alla
* element från den existerande prioritetskön
* på motsvarande index i den nya arrayn, frigör
* minnesutrymmet för den gamla arrayn och låter
* den nya ta dess plats.
* Funktionen är dold för klienten.
*/
static void expandArray(pqueueADT pqueue) {
	int i;
	int *expanded;

	expanded = NewArray(pqueue->current_max_size * 2, int);

	for (i = 0; i < pqueue->numEntries; i++) {
		expanded[i] = pqueue->entries[i];
	}

	free(pqueue->entries);
	pqueue->entries = expanded;
	pqueue->current_max_size *= 2;
}
