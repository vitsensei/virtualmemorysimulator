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

	int arrivalTime;
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
	currentPage->arrivalTime = -1;

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


//use to display the last 2 algorithm
void displayP2(struct pageQueue* q)
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
				printf("base: NULL, mod: NULL, r_bit: NULL\n");
			}
			else
			{
				printf("base: %li, mod: %li ", ((curr -> base) - (curr->base)%(curr->size))/(curr->size), curr->mod);
				printf("r_bit: |");
				for (int i=0; i<8; i++)
				{
					printf("%i|",curr -> r_bit[i]);
				}
				printf(" arrivalTime: %i", curr -> arrivalTime);
				printf("\n");
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

void shiftRight(int num[], int size)
{
	int i;

	for(i = size - 1; i > 0; i --) // starting from the last element, move down to the a[0] element
	{
		num[i] = num[i-1]; // the current element is equal to the element to the left of it (shift right)
	}
	num[0] = 0; // assigning the first element to zero, because when we shift right, the first always gonna be zero
}

int convert(int dec)
{
	if (dec == 0)
    {
        return 0;
    }
    else
    {
        return (dec % 2 + 10 * convert(dec / 2));
    }
}

//a function to convert from an int array to a char array, and then from a char to an int
int bin2dec(int num[], int size)
{
	int i;
	int dec = 0;
	int base = 1;

	for(i = size - 1; i >= 0; i--)
	{
		dec = base * num[i] + dec;
		base = base * 2;  
	}

	return dec;
}

void resetArray(int num[], int size)
{
	int i;

	for(i=0; i<size; i++)
	{
		num[i] = 0;
	}

}

void pickLowestDecBit(struct pageQueue* q, int new_base, int new_limit,long int* totalWp, int clock)
{
	struct Page* curr;
	curr = q->front;

	//lowest at start is the front's dec_bit
	int lowest_dec_bit = 255;
	int lowest_arrival_time = 1000001; //to be honest, I would set this to int max but I'm lazy.
	int cdec;

	//find what is the lowest_dec_bit
	while (curr != NULL)
	{
		cdec = bin2dec(curr -> r_bit, 8);
		// printf("base %li's cdec is %i \n",((curr -> base) - (curr->base)%(curr->size))/(curr->size), cdec);
		if(cdec < lowest_dec_bit)
	 	{
	 		lowest_dec_bit = cdec;
	 	}
	 	// printf("lowest_dec_bit is %i \n", lowest_dec_bit);
	 	curr = curr->next;
	}

	curr = q -> front;

	//find who has the lowest_arrival_time
	while (curr != NULL)
	{
		cdec = bin2dec(curr -> r_bit, 8);
		if (cdec == lowest_dec_bit)
		{
			//printf("Ok the fucking victim is %li! \n",((curr -> base) - (curr->base)%(curr->size))/(curr->size));

			// if (curr -> mod == 1)
			// {
			// 	printf("This page is dirty! \n");
			// 	(*totalWp)++;
			// 	curr -> mod = 0;
			// }

			// curr->base = new_base;
	 	// 	curr->limit = new_limit;
	 	// 	curr->arrivalTime = clock;

	 		// resetArray(curr -> r_bit, 8);
	 		// curr -> r_bit[0] = 1;
	 		// break;

	 		if (curr -> arrivalTime < lowest_arrival_time)
	 		{
	 			lowest_arrival_time = curr -> arrivalTime;
	 		}
		}
		curr = curr -> next;
	}

	curr = q -> front;

	while (curr != NULL)
	{
		cdec = bin2dec(curr -> r_bit, 8);
		if ((cdec == lowest_dec_bit) && ((curr -> arrivalTime) == lowest_arrival_time))
		{
			printf("Ok the fucking victim is %li! \n",((curr -> base) - (curr->base)%(curr->size))/(curr->size));

			if (curr -> mod == 1)
			{
				printf("This page is dirty! \n");
				(*totalWp)++;
				curr -> mod = 0;
			}

			curr->base = new_base;
	 		curr->limit = new_limit;
	 		curr->arrivalTime = clock;

	 		resetArray(curr -> r_bit, 8);
	 		curr -> r_bit[0] = 1;
	 		break;
		}

		curr = curr -> next;
	}

}

void printOutput(long int events, long int diskReads, long int diskWrites)
{
	printf("events in trace:\t%li\n",events);
	printf("total disk reads:\t%li\n",diskReads);
	printf("total disk writes:\t%li\n",diskWrites);
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
	long int interval;

	if (argv[5] != NULL)	{
		interval = atoi(argv[5]);
	} 
	else if (argv[5] == NULL) 
	{
		interval = -1;
	}

	long int a;

	for(a = 0; a < num_of_pages; a++)
	{
		struct Page* my_page = createPage(page_size);
		PushPage(frames,my_page);
	}

	struct Node* iter; //pointer pointing to the instruction queue
	iter = q->front; 

	struct Page* iterF; //pointer pointing to the frame, use to iterating through all the frame to look for existing frame/free frame
	iterF = frames -> front; 

	struct Page* victim; //pointer pointing to the victim
	victim = frames -> front;

	long int i; //i and j use for the for loop, no pun intended.
	long int j;
	long int skip = 0; //use to indicate whether we need page replacement or not.
	int skip2 = 0;


	long int my_mem; //my_mem = iter -> memory;
	long int my_base; //my_base = iter -> base;
	long int my_lim; //my_lim = iter -> limit;
	long int my_size; //my_size = iter -> size;
	my_size = atoi(argv[2]);

	long int totalR = 0;
	long int totalW = 0;
	long int event = 0;


	if (strcmp(argv[4],"SC") == 0)
	{
		//Second chance
		while (iter != NULL)	//iterate through all the instruction.
		{
			event++;			
			
			//check if there is any existing memory that contain the address from the instruction
			for (i=0;i<num_of_pages;i++) 
			{
				if ((iter -> memory <= iterF -> limit) && (iter -> memory >= iterF -> base))	
				{
					
					iterF -> sc = 1;
					if (strcmp(iter -> instruction, "W") == 0)
					{
						
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
				for (i=0;i<num_of_pages;i++) 
				{
					if (iterF -> base == -1 && skip == 0) //check if the frame is free or not. Free frame has base and other variable equal -1
					{
						

						//since we just read the disk, increase totalR
						totalR++;
						

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
				
				while (1) 
				{
					//page replacement

					if (victim -> sc == 1)
					{
						//second chance! set reference bit to 0
						
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
						

						//replace this page
						

						//check for dirty bit
						if (victim -> mod == 1)	// has dirty bit
						{	
							//write back to memory
							
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
			
		}

	}


	else if (strcmp(argv[4],"ESC") == 0)
	{
		
		//Enhanced second chance
		while (iter != NULL)	//iterate through all the instruction.
		{
			event++;
				
			//check if there is any existing memory that contain the address from the instruction
			for (i=0;i<num_of_pages;i++) 
			{
				if ((iter -> memory <= iterF -> limit) && (iter -> memory >= iterF -> base))	
				{
					
					iterF -> sc = 1;
					if (strcmp(iter -> instruction, "W") == 0)
					{
						
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
				for (i=0;i<num_of_pages;i++) 
				{
					if (iterF -> base == -1 && skip == 0) //check if the frame is free or not. Free frame has base and other variable equal -1
					{
						
						//since we just read the disk, increase totalR
						totalR++;						

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
				//we just read the memory, increase totalR.
				totalR++;
				
				//first loop, looking for (0,0)
				for (i = 0; i<num_of_pages; i++)	
				{
					
					if (victim -> sc == 0 && victim -> mod == 0)
					{
						//find the victim, now replace the page

						//check if the page is dirty. Ohhh, dirty dirty little boy.
						if ( victim -> mod == 1)
						{
							totalW++;
						}

						//calculate the base and limit of the frame
						my_mem = iter -> memory;
						my_base = (my_mem - my_mem % my_size);
						my_lim = my_base + my_size -1;

						//put the page into the free frame
						victim -> base = my_base;
						victim -> limit = my_lim;
						victim -> sc = 1;

						//check if the instruction is write, if yes, mod = 1.

						if (strcmp(iter -> instruction, "W") == 0)
						{							
							victim -> mod = 1;
						}

						else if (strcmp(iter -> instruction, "R") == 0)
						{
							victim -> mod = 0;
						}

						else //error handler
						{
							printf("Error. Unknown instruction. \n");
						}

						//move the victim pointer to the front of the frames queue
						if (victim -> next != NULL)	
						{
							victim = victim -> next;
						}

						else if (victim -> next == NULL)
						{
							victim = frames -> front;
						}

						skip2 = 1;
						break;
					}

					else //for the case where (sc = 0)/(sc = 1) and (mod = 1), we will check this case later
					{
						if (victim -> next != NULL)	
						{
							victim = victim -> next;
						}

						else if (victim -> next == NULL)
						{
							victim = frames -> front;
						}

						continue;
					}
				}

				//second loop, looking for (0,1)
				
				if (skip2 == 0)
				{
					for (i = 0; i<num_of_pages; i++)
					{
						
						if (victim -> sc == 0 && victim -> mod == 1)
						{
							//find the victim, now replace the page

							//check if the page is dirty.
							if ( victim -> mod == 1)
							{
								totalW++;
							}

							//calculate the base and limit of the frame
							my_mem = iter -> memory;
							my_base = (my_mem - my_mem % my_size);
							my_lim = my_base + my_size -1;

							//put the page into the free frame
							victim -> base = my_base;
							victim -> limit = my_lim;
							victim -> sc = 1;

							//check if the instruction is write, if yes, mod = 1.
							if (strcmp(iter -> instruction, "W") == 0)
							{								
								victim -> mod = 1;
							}

							else if (strcmp(iter -> instruction, "R") == 0)
							{
								victim -> mod = 0;
							}

							else //error handler
							{
								printf("Error. Unknown instruction. \n");
							}

							//move the victim pointer to the front of the frames queue
							if (victim -> next != NULL)	
							{
								victim = victim -> next;
							}

							else if (victim -> next == NULL)
							{
								victim = frames -> front;
							}

							skip2 = 1;

							break;
						}

						else if (victim -> sc == 1)
						{
							//second chance, set sc back to 0
							victim -> sc = 0;

							//move the victim pointer to the front of the frames queue
							if (victim -> next != NULL)	
							{
								victim = victim -> next;
							}

							else if (victim -> next == NULL)
							{
								victim = frames -> front;
							}

							continue;
						}
					}
				}
				

				//if all fail (now, all the sc bit has set to 0), repeat the first 2 loop.

				//(0,0)
				if (skip2 == 0)
				{					
					//we just read the memory, increase totalR.

					for (i = 0; i<num_of_pages; i++)
					{
						
						if (victim -> sc == 0 && victim -> mod == 0)
						{
							//find the victim, now replace the page

							//check if the page is dirty.
							if ( victim -> mod == 1)
							{
								totalW++;
							}

							//calculate the base and limit of the frame
							my_mem = iter -> memory;
							my_base = (my_mem - my_mem % my_size);
							my_lim = my_base + my_size -1;

							//put the page into the free frame
							victim -> base = my_base;
							victim -> limit = my_lim;
							victim -> sc = 1;

							//check if the instruction is write, if yes, mod = 1.
							if (strcmp(iter -> instruction, "W") == 0)
							{
								victim -> mod = 1;
							}

							else if (strcmp(iter -> instruction, "R") == 0)
							{
								victim -> mod = 0;
							}

							else //error handler
							{
								printf("Error. Unknown instruction. \n");
							}


							//move the victim pointer to the front of the frames queue
							if (victim -> next != NULL)	
							{
								victim = victim -> next;
							}

							else if (victim -> next == NULL)
							{
								victim = frames -> front;
							}

							skip2 = 1;

							break;
						}


						else //for the case where (sc = 0) and (mod = 1), we will check this case later
						{
							if (victim -> next != NULL)	
							{
								victim = victim -> next;
							}

							else if (victim -> next == NULL)
							{
								victim = frames -> front;
							}

							continue;
						}
					}
				}

				//(0,1)
				if (skip2 == 0)
				{
					for (i = 0; i<num_of_pages; i++)
					{
						
						if (victim -> sc == 0 && victim -> mod == 1)
						{
							//find the victim, now replace the page

							//check if the page is dirty.
							if ( victim -> mod == 1)
							{
								totalW++;
							}

							//calculate the base and limit of the frame
							my_mem = iter -> memory;
							my_base = (my_mem - my_mem % my_size);
							my_lim = my_base + my_size -1;

							//put the page into the free frame
							victim -> base = my_base;
							victim -> limit = my_lim;
							victim -> sc = 1;

							//check if the instruction is write, if yes, mod = 1.
							if (strcmp(iter -> instruction, "W") == 0)
							{
								
								victim -> mod = 1;
							}

							else if (strcmp(iter -> instruction, "R") == 0)
							{
								victim -> mod = 0;
							}

							else //error handler
							{
								printf("Error. Unknown instruction \n");
							}

							//move the victim pointer to the front of the frames queue
							if (victim -> next != NULL)	
							{
								victim = victim -> next;
							}

							else if (victim -> next == NULL)
							{
								victim = frames -> front;
							}

							skip2 = 1;

							break;
						}

						else if (victim -> sc == 1)
						{
							//second chance, set sc back to 0
							victim -> sc = 0;

							//move the victim pointer to the front of the frames queue
							if (victim -> next != NULL)	
							{
								victim = victim -> next;
							}

							else if (victim -> next == NULL)
							{
								victim = frames -> front;
							}

							continue;
						}
					}
				}
				
			}
			
			skip = 0;
			skip2 = 0;
			iter = iter -> next; //move to the next instruction
			
		}
	}

	else if (strcmp(argv[4],"ARB") == 0)
	{
		printf("USING ADDITIONAL REFERENCE BIT \n\n");
		//Additional reference bit

		int clock;
		clock = 0;

		while (iter != NULL)	//iterate through all the instruction.
		{
			event++;
			clock++;

			printf("Event: %li, clock: %i \n", event,clock);
			printf("Accessing memory %li \n", (iter -> memory - iter -> memory%page_size)/page_size);

			iterF = frames -> front;

			if (((clock % interval == 1) && (clock != 1)) || (interval == 1)) //right before the start of a new interval
			{
				printf("\n Just before the begin of new interval, shift r_bit of every page.\n");

				//shift r_bit of every page
				while (iterF != NULL)
				{
					
					shiftRight(iterF -> r_bit, 8); //length of r_bit is always 8
					iterF = iterF -> next;
				}
			}

			iterF = frames -> front;

			//check if there is any existing memory that contain the address from the instruction
			for (i=0;i<num_of_pages;i++) 
			{
				if ((iter -> memory <= iterF -> limit) && (iter -> memory >= iterF -> base))	
				{
					printf("\n The memory is already available in the virtual memory. \n");
					if (strcmp(iter -> instruction, "W") == 0)
					{
						printf("The page with the base of %li is modified \n", ((iterF -> base) - (iterF->base)%(iterF->size))/(iterF->size));
						iterF -> mod = 1;
					}

					iterF -> r_bit[0] = 1;

					skip = 1;
					break;
				}

				iterF = iterF -> next; //move to the next frame
			}

			iterF = frames -> front; //reset frame pointer to the front of the queue

			//check if there is any free frame
			if (skip == 0)
			{
				for (i=0;i<num_of_pages;i++) 
				{
					if (iterF -> base == -1 && skip == 0) //check if the frame is free or not. Free frame has base and other variable equal -1
					{
						printf("\n Found a free frame.");

						//since we just read the disk, increase totalR
						totalR++;
						
						//calculate the base and limit of the frame
						my_mem = iter -> memory;
						my_base = (my_mem - my_mem % my_size);
						my_lim = my_base + my_size -1;

						//put the page into the free frame
						iterF -> base = my_base;
						iterF -> limit = my_lim;

						iterF -> arrivalTime = clock;

						if (strcmp(iter -> instruction, "W") == 0)
						{	
							printf("The page with the base of %li is modified \n", ((iterF -> base) - (iterF->base)%(iterF->size))/(iterF->size));
							iterF -> mod = 1;
						}

						iterF -> r_bit[0] = 1;

						skip = 1;
						break;
					}

					iterF = iterF -> next; //move to the next frame
				}
			}

			iterF = frames -> front; //reset frame pointer to the front of the queue

			//After fail 2 test, we will now activate page replacement

			if (skip == 0)
			{
				printf("Page replacement activate. \n");
				//check for page with the smallest dec_bit
				//after new page get in, r_bit reset all equal 0 then r_bit[0] = 1
				//all is done in the function pickLowestDecBit
				my_mem = iter -> memory;
				my_base = (my_mem - my_mem % my_size);
				my_lim = my_base + my_size -1;

				//and since we just read from memory,
				totalR++;
				pickLowestDecBit(frames, my_base, my_lim, &totalW, clock);

				if (strcmp(iter -> instruction, "W") == 0)
				{	
					printf("The page with the base of %li is modified \n", ((iterF -> base) - (iterF->base)%(iterF->size))/(iterF->size));
					iterF -> mod = 1;
				}
			}

			skip = 0;
			iter = iter -> next;

			printf(" \n Current table: \n");
			displayP2(frames);
			printf("\n\n");

		}
	}

	else if (strcmp(argv[4],"EARB") == 0)
	{
		printf("USING ENHANCED ADDITONAL REFERENCE BIT \n\n");
		//Enhanced additional reference bit
	}

	printOutput(event, totalR, totalW);
	return 0;
}