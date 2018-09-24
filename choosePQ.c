/*
* This file is created by :
Luu Viet Anh Tran a1713568
Le Dinh Minh Long a1701729

Final Version Created on 23th September 2018

For Assignment 2 Operating System

*/



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct Node
{
	char* instruction;
	long int memory;
	struct Node* next;	
};

struct Page
{
	long int size;
	long int base;
	long int limit;
	long int sc;
	long int mod;
	int r_bit[8]; // a pointer to an array of int, that represent a reference bit
	int dec_bit; // a value that represent the decimal value of the rbit array of a page
	struct Page* next;
};

struct Queue
{
	struct Node* front;
	struct Node* back;
};

struct pageQueue
{
	struct Page* front;
	struct Page* back;
};

struct Node* createBlock(char* instruction,long int memo)
{
	
	struct Node* currentNode;
	currentNode = (struct Node*)malloc(sizeof(struct Node));


	//copying in string instruction
	char* insCopy;
	insCopy = malloc(sizeof(insCopy));
	strcpy(insCopy, instruction);
	currentNode->instruction = insCopy;

	currentNode->memory = memo;

	currentNode->next = NULL;

	return currentNode;

}

struct Page* createPage(long int pageSize)
{
	struct Page* currentPage;
	currentPage = (struct Page*)malloc(sizeof(struct Page));

	currentPage->size = pageSize;
	currentPage->base = -1;
	currentPage->limit = -1;
	currentPage->sc = 1; // when first created, sc is always equal 1
	currentPage->mod = 0; // when first created, the page is not modified yet

	//intialise all elements to 0
	int i;
	for(i = 0;i < 8; i++)
	{
		currentPage -> r_bit[i] = 0;
	}

	currentPage->dec_bit = 0; //intialise dec_bit to 0

	currentPage->next = NULL;

	return currentPage;

}

struct Queue* createQueue()
{
	struct Queue* q;
	q = (struct Queue*)malloc(sizeof(struct Queue));

	q->front = NULL;
	q->back = NULL;
	return q;
}

struct pageQueue* createPageQueue()
{
	struct pageQueue* q;
	q = (struct pageQueue*)malloc(sizeof(struct pageQueue));

	q->front = NULL;
	q->back = NULL;
	return q;
}

//exact from assignment1
struct Queue* Push(struct Queue* q, struct Node* block)
{	
	struct Node* curr = block;

	if(q->back == NULL)
	{
		q->back = curr;
		q->front = curr;
		return q;
	}
	else
	{
		q->back->next = curr;
		q->back = curr;
		curr->next = NULL; //in case we push someone from a middle of a queue

	}
	

	
	return q;
}

struct pageQueue* PushPage(struct pageQueue* q, struct Page* page)
{
	struct Page* curr = page;

	if(q->back == NULL)
	{
		q->back = curr;
		q->front = curr;
		return q;
	}
	else
	{
		q->back->next = curr;
		q->back = curr;
		curr->next = NULL; //in case we push someone from a middle of a queue

	}

	return q;
}


struct Node* Pop(struct Queue* q)
{
	if(q->front == NULL) 
	{
		return NULL;
	}


	struct Node* curr = q->front;
	q->front = q->front->next;
	

	if(q->front == NULL) 
	{
		q->back = NULL;
	}

	return curr;

}

struct Page* PopPage(struct pageQueue* q)
{
	if(q->front == NULL) 
	{
		return NULL;
	}

	struct Page* curr = q->front;
	q->front = q->front->next;

	if(q->front == NULL) 
	{
		q->back = NULL;
	}

	return curr;

}


void display(struct Queue* q)
{
	long int i = 0;
	if(q->front == NULL)
	{
		printf("queue is empty\n");
	}
	else
	{
		struct Node* curr = q->front;

		//traversing the list
		while(curr->next != NULL)
		{
			printf("%s %li\n", curr->instruction, curr->memory);
			curr = curr->next;
			
		}

		// at this point, curr pointer is pointing to the last block
		printf("%s %li-->NULL\n", curr->instruction, curr->memory);

	}
	
}

void displayPageQueue(struct pageQueue* q)
{
	long int i = 0;
	if(q->front == NULL)
	{
		printf("queue is empty\n");
	}
	else
	{
		struct Page* curr = q->front;

		while(curr->next != NULL)
		{
			printf("%li \n", curr->size);
			curr = curr->next;
			
		}
		printf("%li-->NULL\n", curr->size);
	}

}

void displayRBit(struct pageQueue* q)
{
	long int i = 0;
	if(q->front == NULL)
	{
		printf("queue is empty\n");
	}
	else
	{
		struct Page* curr = q->front;

		while(curr->next != NULL)
		{
			printf("%i \n", curr->r_bit[0]);
			curr = curr->next;
			
		}
		printf("%i-->NULL\n", curr->r_bit[0]);
	}
}


//a function to display mod and mem
void displayP(struct pageQueue* q)
{
	if(q->front == NULL && q->back == NULL)
	{
		printf("queue is empty !\n");
	}
	else
	{
		struct Page* curr = q->front;

		while(curr != NULL)
		{
			if(curr->base == -1)
			{
				printf("mod: NULL, base: NULL\n");
			}
			else
			{
				printf("base: %li, sc: %li, mod: %li \n", ((curr -> base) - (curr->base)%(curr->size))/(curr->size), curr -> sc, curr->mod);
			}
			curr = curr->next;
		}
	}
}


int isEmpty(struct Queue* q)
{
	if(q->front == NULL)
	{
		return 1;
	}
	return 0;
}


long int hexToDec(char hex[])
{
	long int len = strlen(hex);

	//initialise base value to 1,(16^0)
	long int base = 1;
	long int dec = 0;
	long int i;

	for(i=len-1; i>=0; i--)
	{
		//if char lies in 0-9, convert by subtracting 48
		if(hex[i] >= '0' && hex[i] <= '9')
		{
			dec += (hex[i] - 48) * base;

			//incrementing base by power
			base = base * 16;
		}

		//if char lies in A-F, convert by subtract 55
		else if(hex[i] >= 'A' && hex[i] <= 'F')
		{
			dec += (hex[i] - 55) * base;

			//incrementing base by power
			base = base * 16;
		}
		else if(hex[i] >= 'a' && hex[i] <= 'f')
		{
			dec += (hex[i] - 87) * base;

			//incrementing base by power
			base = base * 16;
		}

	}

	return dec;
}


void printOutput(long int events, long int diskReads, long int diskWrites)
{
	printf("events in trace:\t%li\n",events);
	printf("total disk reads:\t%li\n",diskReads);
	printf("total disk writes:\t%li\n",diskWrites);
}


//a function to pick the page with the lowest dec_bit and then modify its base and limit
struct Page* pickLowestDecBit(struct pageQueue* q, int new_base, int new_limit)
{
	struct Page* curr;
	curr = q->front;

	struct Page* lowestPage;
	lowestPage = curr;
	


	//lowest at start is the front's dec_bit
	int lowest_dec_bit = curr->dec_bit;

	while(curr != NULL)
	{
		if(curr->dec_bit < lowest_dec_bit)
	 	{
	 		lowest_dec_bit = curr->dec_bit;
	 		curr->base = new_base;
	 		curr->limit = new_limit;
	 		lowestPage = curr;
	 	}
	 	curr = curr->next;
	}

	return lowestPage;
}


int main(int argc, char* argv[])
{
	//reading input

	FILE * fp = fopen(argv[1], "r");

	if(fp == NULL)
	{
		printf("cannot open file\n");
	}

	char line[256];
	char* token;

	char* insCopy;
	insCopy = malloc(sizeof(insCopy));

	char* memoCopy;
	memoCopy = malloc(sizeof(memoCopy));


	struct Queue* q = createQueue();

	//it has to create the queue on the go, because there might be millions of line
	while(fgets(line,sizeof(line),fp))
	{
		if(line[0] != 'R' && line[0] != 'W')
		{
			continue;
		}
		else
		{
			//here we get a line
			//strtok strip out the tokens one by one, so we need a double strip each line
			long int counter;
			counter = 0;

			token = strtok(line," ");
			while(token)
			{	
				counter++;
				if(counter == 1) // this is instruction
				{
					strcpy(insCopy,token);
				}
				else if(counter == 2) // this is memory
				{
					//create block and push to queue
					strcpy(memoCopy,token);
					long int memoDec = hexToDec(memoCopy);
					struct Node* temp = createBlock(insCopy,memoDec);
	 				Push(q,temp);

	 				//clearing
	 				memset(insCopy,0,strlen(insCopy));
					counter = 0;
				}

				//here we get token, 2 times per loop

				token = strtok(NULL," ");
			}

		}
	}

	fclose(fp);

	


	// Creating page base on the input.



	struct pageQueue* frames = createPageQueue();

	long int num_of_pages = atoi(argv[3]);
	long int page_size = atoi(argv[2]);
	int interval = atoi(argv[5]);

	if (argv[5] != NULL)	{
		long int interval = atoi(argv[5]);
	}

	long int a;

	for(a = 0; a < num_of_pages; a++)
	{
		struct Page* my_page = createPage(page_size);
		PushPage(frames,my_page);
	}


	struct Page* iterF;
	iterF = frames->front;
	iterF = iterF->next;
	iterF = iterF->next;
	iterF = iterF->next;
	iterF->r_bit[0] = 12;
	iterF->dec_bit = -1;

	displayRBit(frames);
	struct Page* lowest = pickLowestDecBit(frames, 0, 2);
	printf("lowest: %i\n",lowest->r_bit[0]); //chua set dec_bit
	return 0;
}