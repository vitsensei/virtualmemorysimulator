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
	currentPage->sc = 1; // when first created, sc is always equal 1
	currentPage->mod = 0; // when first created, the page is not modified yet

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

		while(curr->next != NULL)
		{
			if(curr->base == -1)
			{
				printf("mod: NULL, base: NULL\n");
			}
			else
			{
				printf("mod: %i, base: %i \n", curr->mod,((curr -> base) - (curr->base)%(curr->size))/(curr->size));
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

void printOutput(int events, int diskReads, int diskWrites)
{
	printf("events in trace:\t%i\n",events);
	printf("total disk reads:\t%i\n",diskReads);
	printf("total disk writes:\t%i\n",diskWrites);
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
			//printf("stuck\n");
			continue;
		}
		else
		{
			//here we get a line
			//strtok strip out the tokens one by one, so we need a double strip each line
			int counter;
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
					int memoDec = hexToDec(memoCopy);
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

	printf("This is the input: \n");
	display(q);


	// Creating page base on the input.
	printf("\nCreate %s pages with the size of %s \n",argv[3],argv[2]);


	struct pageQueue* frames = createPageQueue();

	int num_of_pages = atoi(argv[3]);
	int page_size = atoi(argv[2]);

	if (argv[5] != NULL)	{
		int interval = atoi(argv[5]);
	}

	int a;

	for(a = 0; a < num_of_pages; a++)
	{
		struct Page* my_page = createPage(page_size);
		PushPage(frames,my_page);
	}

	//displayPageQueue(frames);
	displayP(frames);


	printf("\n\n================Executing===============\n\n");

	struct Node* iter; //pointer pointing to the instruction queue
	iter = q->front; 

	struct Page* iterF; //pointer pointing to the frame, use to iterating through all the frame to look for existing frame/free frame
	iterF = frames -> front; 

	struct Page* victim; //pointer pointing to the victim
	victim = frames -> front;

	int i; //i and j use for the for loop, no pun intended.
	int j;
	int skip = 0; //use to indicate whether we need page replacement or not.

	int my_mem; //my_mem = iter -> memory;
	int my_base; //my_base = iter -> base;
	int my_lim; //my_lim = iter -> limit;
	int my_size; //my_size = iter -> size;
	my_size = atoi(argv[2]);

	int totalR = 0;
	int totalW = 0;
	int event = 0;


	if (strcmp(argv[4],"SC") == 0)
	{
		printf("USING SECOND CHANCE \n\n");
		//Second chance
		while (iter != NULL)	//iterate through all the instruction.
		{
			event++;
			printf("Event: %i \n", event);
			
			printf("Accessing memory %i \n", iter -> memory);
			//check if there is any existing memory that contain the address from the instruction
			for (i=0;i<num_of_pages;i++) // if you have 4 pages to iterate through, you only need the iterF = iterF -> next instruction to run 3 times.
			{
				if ((iter -> memory <= iterF -> limit) && (iter -> memory >= iterF -> base))	
				{
					printf("The memory is already available in the virtual memory. \n");
					iterF -> sc = 1;
					if (strcmp(iter -> instruction, "W") == 0)
					{
						printf("The page with the base of %i is modified \n", iterF -> base);
						iterF -> mod = 1;
					}

					skip = 1;
					break;
				}

				iterF = iterF -> next; //move to the next frame
			}

			iterF = frames -> front; //reset frame pointer to the front of the queue

			//check if there is any free frame
			if (skip == 0)
			{
				for (i=0;i<num_of_pages;i++) // if you have 4 pages to iterate through, you only need the iterF = iterF -> next instruction to run 3 times.
				{
					if (iterF -> base == -1 && skip == 0) //check if the frame is free or not. Free frame has base and other variable equal -1
					{
						printf("Found a free frame. \n");

						//since we just read the disk, increase totalR
						totalR++;
						printf("Total read = %i \n", totalR);

						//calculate the base and limit of the frame
						my_mem = iter -> memory;
						my_base = (my_mem - my_mem % my_size);
						my_lim = my_base + my_size -1;

						//put the page into the free frame
						iterF -> base = my_base;
						iterF -> limit = my_lim;
						iterF -> sc = 1;

						if (strcmp(iter -> instruction, "W") == 0)
						{
							printf("The page with the base of %i is modified \n", iterF -> base);
							iterF -> mod = 1;
						}

						skip = 1;
						break;
					}

					iterF = iterF -> next; //move to the next frame
				}
			}

			iterF = frames -> front; //reset frame pointer to the front of the queue

			//fail 2 check, now have to replace the page
			if (skip == 0)
			{
				printf("Page replacement activate \n");
				while (1) // if you have 4 pages to iterate through, you only need the iterF = iterF -> next instruction to run 3 times.
				{
					//page replacement

					if (victim -> sc == 1)
					{
						//second chance! set reference bit to 0
						printf("Page with the base of %i has a second chance. Page -> mod = %i \n", victim -> base, victim -> mod);
						victim -> sc = 0;
						if (victim -> next != NULL)
						{
							victim = victim -> next;

						}
						else if (victim -> next == NULL)
						{
							victim = frames -> front;
						}

					}
					else if (victim -> sc == 0)
					{
						//since we just read the disk, increase totalR. I did not put it right after the comment //page replacement
						//because if I do so, everytime a page gets a second chance, totalR would increase.
						totalR++;
						printf("Total read = %i \n", totalR);

						//replace this page
						printf("Page with the base of %i is the victim. Page -> mod = %i \n", victim -> base, victim -> mod);

						//check for dirty bit
						if (victim -> mod == 1)	// has dirty bit
						{	
							//write back to memory
							printf("Page with the base of %i is dirty \n", victim -> base);
							totalW++;
							victim -> mod = 0;
						}

						//calculate the base and limit of the frame
						my_mem = iter -> memory;
						my_base = (my_mem - my_mem % my_size);
						my_lim = my_base + my_size -1;

						//put the page into the free frame
						victim -> base = my_base;
						victim -> limit = my_lim;
						victim -> sc = 1;

						if (strcmp(iter -> instruction, "W") == 0)
						{
							printf("The page with the base of %i is modified \n", iterF -> base);
							victim -> mod = 1;
						}

						if (victim -> next != NULL)
						{
							victim = victim -> next;

						}
						else if (victim -> next == NULL)
						{
							victim = frames -> front;
						}

						break;
					}
					else
					{
						printf("Undefined case at Second Chance/Page replacement. \n");
						break;
					}

				}
			}

			skip = 0;
			iter = iter -> next; //move to the next instruction
			printf("Current table: \n");
			displayP(frames);
			printf ("\n\n");
		}

	}

	else if (strcmp(argv[4],"ESC") == 0)
	{
		printf("USING ENHANCED SECOND CHANCE \n\n");
		//Enhanced second chance
	}

	else if (strcmp(argv[4],"ARB") == 0)
	{
		printf("USING ADDITIONAL REFERENCE BIT \n\n");
		//Additional reference bit
	}

	else if (strcmp(argv[4],"EARB") == 0)
	{
		printf("USING ENHANCED ADDITONAL REFERENCE BTI \n\n");
		//Enhanced additional reference bit
	}

	printOutput(event, totalR, totalW);
	return 0;
}