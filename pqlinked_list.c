/*
* File: pqlinked_list.c
* ---------------
* Author: Jonathan Thornander, Christoffer Ivarsson Orrelid, Fredrik Dhami
*
* Task: Detta program implementerar en prioritetskö
* i form av en länkad lista. Listan består structer av
* innehållandes ett heltalsvärde samt en pekare till
* nästföljande struct i listans ordning.
*
* Prioritetskön tar endast heltalsvärden som input
* och sorterar dem i fallande ordning där störst tal 
* hamnar först och det största talet tas ut först vid
* Dequeue.
*/
#include "pqueue.h"
#include "genlib.h"

/* Type definition: cellT
 * --------------------
 * Typen cellT pekar på en struct innehållandes en variabel
 * för ett heltalsvärde samt en pekare till nästföljande
 * struct i prioritetskön.
 */
typedef struct cellT {
	int value;
	struct cellT* link;
} *cellT;

/* Type definition: pqueueCDT
 * --------------------
 * Typen pqueueCDT är den konkreta implementeringen av
 * den exporterade typen pqueueADT. pqueueCDT innehåller
 * endast en cellT-struct som symboliserar början på kön.
 */
struct pqueueCDT {
	cellT* head;
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

/*
* Function: Enqueue
* Usage: Enqueue(pqueue, newValue);
* -------------------------------
* Denna funktion fyller ut prioritetskön med ett nytt värde
* valt av användaren. Funktionen använder sig av tre cellT-
* pekare där newNode håller användarens nya värde och
* lastNode och currentNode används för att löpa igenom kön
* för att placera in det nya värdet på rätt plats i de fall
* kön inte är tom eller det nya värdet skall sättas in på
* första plats i en ej tom kö.
*/
void Enqueue(pqueueADT pqueue, int newValue)
{
	cellT newNode, lastNode, currentNode;

	newNode = New(cellT);
	newNode->value = newValue;
	newNode->link = NULL;

	currentNode = NULL;
	lastNode = NULL;

	if (IsEmpty(pqueue)) {
		pqueue->head = newNode;
	}
	else {
		currentNode = pqueue->head;

		if (currentNode->value <= newNode->value) {
			pqueue->head = newNode;
			newNode->link = currentNode;
			return;
		}
		while (currentNode != NULL) {
			if (currentNode->value <= newNode->value) {
				lastNode->link = newNode;
				newNode->link = currentNode;
				return;
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
 * elementet, som alltid är först i kön, pekar en temporär cellT
 * på dess minnesutrymme, dess heltalsvärde hålls av en int
 * och köns huvud sätts att peka på nästföljande cellT-struct.
 * Det oanvända minnesutrymmet frigörs och heltalet returneras.
 */

int DequeueMax(pqueueADT pqueue)
{
	cellT temp;
	int value = 0;

	if (IsEmpty(pqueue))
		Error("Tried to dequeue max from an empty pqueue!");
	else {
		temp = pqueue->head;
		value = temp->value;
		pqueue->head = temp->link;
		free(temp);
	}
	return value;
}
