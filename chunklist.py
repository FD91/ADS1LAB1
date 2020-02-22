///*
//* File: pq_chunklist.c
//* ---------------
//* Author: Jonathan Thornander, Christoffer Ivarsson Orrelid, Fredrik Dhami
//*
//* Task: Detta program implementerar en prioritetskö
//* i form av en chunk list. Listan består structer av
//* statiskt allokerade arrayer vars storlek kan ändras 
//* av klienten genom en förbestämd konstant.
//*
//* Prioritetskön tar endast heltalsvärden som input
//* och sorterar arrayerna i kön i fallande ordning där
//* den array med störst tal hamnar först och det största
//* talet i den första arrayn tas ut först vid Dequeue.
//*
//* Internt sorteras dock elementen i enskilda arrays i
//* stigande ordning med det största heltalsvärdet lagrat
//* på den sista lediga platsen. Blir en array full skapas
//* en ny array med den "övre halvan" (lägre talen) av den
//* förutvarande fulla arrayn.
//*/

#include "pqueue.h"
#include "genlib.h"

/* Constant: MAX_ELEMS_PER_BLOCK
 * ----------------------
 * Den här konstanten anger antalet element i vektorerna som
 * utgör representationen av prioritetskön.
 */

#define MAX_ELEMS_PER_BLOCK 4

 /* Type definition: cellT
  * --------------------
  * Typen cellT pekar på en struct innehållandes en array av
  * heltal, en variabel för antalet element lagrade för stunden
  * samt en pekare till nästföljande struct i prioritetskön.
  */
typedef struct cellT {
	int nElements;
	int array[MAX_ELEMS_PER_BLOCK];
	struct cellT* link;
} *cellT;

/* Type definition: pqueueCDT
 * --------------------
 * Typen pqueueCDT är den konkreta implementeringen av
 * den exporterade typen pqueueADT. pqueueCDT innehåller
 * endast en cellT-struct som symboliserar början på kön.
 */
struct pqueueCDT {
	cellT head;
};

/* Function: creatCellT
* Usage : newNode = creatCellT(newValue);
*------------------------------ -
*Denna funktion skapar en ny cellT,
* allokerar minnesutrymme till structen den
* pekar på och sätter initialvärden.
* Funktionen är dold för klienten.
*/
static cellT creatCellT(int newValue) {
	cellT newNode;

	newNode = New(cellT);
	newNode->nElements = 1;
	newNode->array[0] = newValue;
	newNode->link = NULL;
}

/* Function: splitCellT
* Usage: splitCellT(currentNode);
* -------------------------------
* Denna funktion skapar en ny cellT,
* allokerar minnesutrymme till structen den
* pekar på och fyller hälften av arrayns
* element med värden från den cellT som
* klienten skickar in som argument. Den
* skriver över den första halvan av den
* inskickade arrayn i cellT (currentNode)
* med den senare halvan så att effekten blir
* att de lägre heltalen nu enbart syns i
* den nyskapade cellT-arrayn.
* Funktionen är dold för klienten.
*/
static void splitCellT(cellT currentNode) {
	cellT newNode;
	int i, j;

	newNode = New(cellT);

	for (i = 0; i < MAX_ELEMS_PER_BLOCK / 2; i++) {
		newNode->array[i] = currentNode->array[i];
	}
	j = 0;
	for (i = i; i < MAX_ELEMS_PER_BLOCK; i++) {
		currentNode->array[j] = currentNode->array[i];
		j++;
	}

	newNode->link = currentNode->link;
	currentNode->link = newNode;

	newNode->nElements = MAX_ELEMS_PER_BLOCK / 2;
	currentNode->nElements = MAX_ELEMS_PER_BLOCK - newNode->nElements;
}

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
* Denna funktion fyller ut prioritetskön med ett ytt värde
* valt av användaren. Funktionen använder sig av tre cellT-
* pekare där newNode håller användarens nya värde och
* lastNode och currentNode används för att löpa igenom kön
* för att placera in det nya värdet på rätt plats i de fall
* kön inte är tom eller det nya värdet skall sättas in i den
* första existerande arrayn.
*/
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
					splitCellT(currentNode);
					if (newValue <= currentNode->link->array[currentNode->link->nElements - 1]) {
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
* Function: DequeueMax
* Usage: DequeueMax(pqueue);
* -------------------------------
* Då elementen sparas osorterat i fältet måste en sökning göras
* för att finna det största elementet. Felhantering fångar upp
* om funktionen försöker köras om listan är tom då funktionen
* skall returnera, och plocka bort, det största elementet i hela
* prioritetskön. Oanvänt minne frigörs inom funktionen.
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
