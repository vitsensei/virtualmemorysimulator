#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//this program reads an input file, put each line into a node,
//and add it inside an array
//io.c has been able to push element into blocks manually


struct Node
{
	char* instruction;
	int memory;
	struct Node* next;	
};

struct Page
{
	int size;
	int base;
	int limit;
	int sc;
	int mod;
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

struct Node* createBlock(char* instruction,int memo)
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

struct Page* createPage(int pageSize)
{
	struct Page* currentPage;
	currentPage = (struct Page*)malloc(sizeof(struct Page));

	currentPage->size = pageSize;
	currentPage->base = -1;
	currentPage->limit = -1;
	currentPage->sc = -1;
	currentPage->mod = -1;

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
	
	//display(q);
	
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

//exact from assignment 1
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
	int i = 0;
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
			printf("%s %i\n", curr->instruction, curr->memory);
			curr = curr->next;
			
		}

		// at this point, curr pointer is pointing to the last block
		printf("%s %i-->NULL\n", curr->instruction, curr->memory);

	}
	
}

void displayPageQueue(struct pageQueue* q)
{
	int i = 0;
	if(q->front == NULL)
	{
		printf("queue is empty\n");
	}
	else
	{
		struct Page* curr = q->front;

		while(curr->next != NULL)
		{
			printf("%i \n", curr->size);
			curr = curr->next;
			
		}
		printf("%i-->NULL\n", curr->size);
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


int hexToDec(char hex[])
{
	int len = strlen(hex);

	//initialise base value to 1,(16^0)
	int base = 1;
	int dec = 0;
	int i;

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

	}

	return dec;
}


int main(int argc, char* argv[])
{
	//reading input

	FILE * fp = fopen(argv[1], "r");

	if(fp == NULL)
	{
		printf("cannot open file\n");
	}

	char ins[3];
	char memo[10];
	char* insCopy;
	char* memoCopy;

	struct Queue* q = createQueue();


	while (!feof(fp))
	{
		//scanning
	 	fscanf(fp, "%s %s",ins,memo);

	 	//making copy to save the two slots in
	 	insCopy = malloc(sizeof(insCopy));
	 	strcpy(insCopy, ins);


	 	memoCopy = malloc(sizeof(memoCopy));
	 	strcpy(memoCopy, memo);

	 	//a function to transfer the memoCopy from hexa to decimal
	 	int memoDec = hexToDec(memoCopy);
	 	
	 	//create a block to push to queue
	 	struct Node* temp = createBlock(insCopy,memoDec);
	 	Push(q,temp);
	}

	printf("This is the input: \n");
	display(q);


	//Creating page base on the input.
	printf("\nCreate %s pages with the size of %s \n",argv[3],argv[2]);


	struct pageQueue* frames = createPageQueue();

	int num_of_pages = atoi(argv[3]);
	int page_size = atoi(argv[2]);
	int interval = atoi(argv[5]);
	int i;

	for(i = 0; i < num_of_pages; i++)
	{
		struct Page* my_page = createPage(page_size);
		PushPage(frames,my_page);
	}

	displayPageQueue(frames);



	printf("\n\n================Executing===============\n\n");

	struct Node* iter;
	iter = q->front; //pointer pointing to the instruction queue

	struct Page* iterF;
	iterF = frames -> front; //pointer pointing to the frame queue

	int i;
	int j;
	int skip = 0; //use to indicate whether we need page replacement or not.

	int my_mem; //my_mem = iter -> memory;
	int my_base; //my_base = iter -> base;
	int my_lim; //my_lim = iter -> limit;
	int my_size; //my_size = iter -> size;

	while (1)
	{
		if (strcmp(argv[3],"SC") == 0)
		{
			printf("USING SECOND CHANCE \n\n");
			//Second chance
			while (iter != NULL)	{ //iterate through all the instruction.

				//check if there is any existing memory that contain the address from the instruction
				for (i=0;i<num_of_pages;i++)
				{
					if ((iter -> memory <= iterF -> limit) && (iter -> memory >= iterF -> base))	
					{
						skip = 1;
						break;
					}
					
					iterF = iterF -> next;
				}

				//check if there is any free frame
				if (skip == 0)
				{
					for (i=0;i<num_of_pages;i++)
					{
						if (iterF -> base == -1 && skip == 0)
						{
							//calculate the base and limit of the frame
							my_mem = iter -> memory;
							my_size = iter -> size;
							my_base = (my_mem - my_mem % my_size)/(my_size);
							my_limit = my_base + my_size -1;

							//put the page into the free frame
							iter -> base = my_base;
							iter -> limit = my_limit;

							skip = 1;
							break;
						}

						iterF = iterF -> next;
					}
				}

				//fail 2 check, now have to replace the page
				if (skip == 0)
				{
					for (i=0;i<num_of_pages;i++)
					{
						//page replacement
					}
				}

				iter = iter -> next;
			}
		}

		else if (strcmp(argv[3],"ESC") == 0)
		{
			printf("USING ENHANCED SECOND CHANCE \n\n");
			//Enhanced second chance
		}

		else if (strcmp(argv[3],"ARB") == 0)
		{
			printf("USING ADDITIONAL REFERENCE BIT \n\n");
			//Additional reference bit
		}

		else if (strcmp(argv[3],"EARB") == 0)
		{
			printf("USING ENHANCED ADDITONAL REFERENCE BTI \n\n");
			//Enhanced additional reference bit
		}
	}

	return 0;
}







