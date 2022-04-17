//Heap Memory management and Garbage Collection

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct nodealloc
{	char data[5];
	int address;
	int memory;
	struct nodealloc *next;
}nodea;

nodea* createnodea(char data[5],int address,int memory)			//allocated list node creation  
{
	nodea* ptr;
	ptr = (nodea*)malloc(sizeof(nodea));
	strcpy(ptr->data,data);
	ptr->address = address;
	ptr->memory = memory;
	ptr->next = NULL;
	return ptr;	
} 

typedef struct nodefree
{	int start_address;
	int end_address;
	int memory;
	struct nodefree *next;
}nodef;

nodef* createnodef(int start_add,int end_add,int memory)		//free list node creation
{	
	nodef* ptr;
	ptr = (nodef*)malloc(sizeof(nodef));
	ptr->start_address = start_add;
	ptr->end_address= end_add;
	ptr->memory= memory;
	ptr->next = NULL;
	return ptr;
}

nodea* insert_at_starta(nodea *lptr,int address,int memory,char* data)		//inserting at start in allocated list
{	
	nodea *ptr;
	ptr = createnodea(data,address,memory);
	ptr->next = lptr;
	lptr = ptr;
	return lptr;
}


void printalloc(nodea *lptr)											//print  allocated list
{
	printf("The allocated list is:\n");
	printf("data\taddress\tmemory\n");
	for(;lptr != NULL;lptr = lptr->next)
	{	printf("%s\t",lptr->data);
		printf("%d\t",lptr->address);
		printf("%d\n",lptr->memory);	
	}
}


nodef* insert_at_startf(nodef *lptr,int start_add,int end_add,int memory)			//inserting at start in allocated list
{
	nodef *ptr;
	ptr = createnodef(start_add,end_add,memory);
	ptr->next = lptr;
	lptr = ptr;
	return lptr;
}

void printfree(nodef *lptr)														//print of free list 
{
	printf("The Free list is:\n");
	printf("start_addr\tEnd_addr\tmemory\n");
	for(;lptr != NULL;lptr = lptr->next)
	{	printf("%d\t\t",lptr->start_address);
		printf("%d\t\t",lptr->end_address);
		printf("%d\n",lptr->memory);
	}
}

nodea* free_fun(nodea *aptr,nodef **fptr,char f[5])									//free the memory 
{	
	nodea *p,*sptr,*prev,*nptr;
	nodef *q,*optr;
	int start_address,end_address,memory,value = 0;
	q = *fptr;
	p = aptr;
	prev = NULL;
	while((p != NULL)&&(value == 0))
	{	if(!strcmp(p->data,f))
		{	start_address = p->address;
			end_address = (p->address) + (p->memory);
			memory= p->memory;
			value = 1;
			if(p == aptr)
			{	nptr = p;
				p = p->next;
				free(nptr);
				aptr = p;
			}	
			else
			{	prev->next = p->next;
				free(p);
			}
		}
		else
		{	prev = p;
			p = p->next;
		}
	}
	value = 0;
	while((q != NULL)&&(value== 0))
	{	if(q->start_address == end_address)
		{	q->start_address = start_address;
			q->memory= (q->memory) + (memory);
			value = 1;
		}
		else if(q->end_address == start_address)
		{	q->end_address = end_address;
			q->memory = (q->memory) + (memory);
			value = 1;
		}
		q = q->next;
	}

	if(value == 0)
	{	*fptr = insert_at_startf(*fptr,start_address,end_address,memory);
	}
	return aptr;
}		

nodea* modifyf(nodef *lptr,nodea *aptr,int memory,char data[5])						//modifing the data in free memory
{
	nodef *ptr,*sptr,*prev;
	int start_address;
	int flag = 1;
	sptr = ptr = lptr;
	while((ptr != NULL)&&(flag == 1))
	{	if(ptr->memory>= memory)
		{	start_address = ptr->start_address;
			ptr->start_address = (ptr->start_address) + memory;
			ptr->memory= (ptr->memory) - (memory);	
			flag = 0;
			aptr = insert_at_starta(aptr,start_address,memory,data);
		}
		else 
		{
			ptr = ptr->next;
		}
	}
	if(ptr == NULL)
	{ 
		printf("Memory allocation not possible \n");		//when full of memory
	}
	while(sptr->next != NULL)
	{	if(((sptr->next)->start_address) == ((sptr->next)->end_address))
		{	prev = sptr;														//removing duplicate nodes with same start and end addresses
			sptr = sptr->next;
			prev->next = sptr->next;
			free(sptr);
		}
		else
		{
			sptr = sptr->next;
		}
	}
	return aptr;
}

nodef* merge_freeblocks(nodef *lptr1,nodef *lptr2)												// merge function
{
	nodef *final,*last;
	nodef *a,*b;
	a = lptr1,b = lptr2;
	
	if(lptr1 == NULL)
	{	final = lptr2;
	}
	else if(lptr2 == NULL)
	{	final = lptr1;
	}
	else
	{	if((lptr1->memory) < (lptr2->memory))
		{	final = lptr1;
			a = a->next;
		}
		else
		{	final = lptr2;
			b = b->next;
		}
		last = final;
		while((a != NULL)&&(b != NULL))
		{	if((a->memory) < (b->memory))
			{	last = last->next = a;
				a = a->next;
			}
			else
			{	last = last->next = b;
				b = b->next;
			}
		}
		if(a != NULL)
		{
			last->next = a;
		}
		else
		{	
			last->next = b;
		}
	}
	return final;
}



//divide function
nodef* divide(nodef *lptr)
{	nodef *slow,*fast,*result;
	slow = lptr;
	fast = lptr->next->next;
	while(fast != NULL)
	{	slow = slow->next;
		fast = fast->next;
		if(fast != NULL)
		{	fast = fast->next;
		}
	}
	result = slow->next;
	slow->next = NULL;
	return result;
}

//sort function
nodef* sort(nodef *lptr)
{	nodef *head,*temp;
	head = lptr;
	if((head != NULL)&&(head->next != NULL))
	{	temp = divide(lptr);
		lptr = sort(lptr);
		temp = sort(temp);
		head = merge_freeblocks(lptr,temp);
	}
	return head;
}



int main()
{
	nodea *aptr;
	nodef *fptr;
	aptr = NULL;
	fptr = NULL;
	aptr = insert_at_starta(aptr,10,10,"node1");
	aptr = insert_at_starta(aptr,50,20,"node2");
	aptr = insert_at_starta(aptr,100,30,"node3");
	fptr = insert_at_startf(fptr,20,50,30);
	fptr = insert_at_startf(fptr,70,100,30);
	fptr = insert_at_startf(fptr,130,200,70);//considered the max size as 200
	int option=0,flag=1;
	while(flag==1)
	{
		printf("\t1)Allocate \n");
		printf("\t2)Delete/free memory \n");
		printf("\t3)Display Memory stats \n");
		printf("\t4)Exit the program\n");
		printf("Select a option from Following :- ");
		scanf("%d",&option);
		switch(option)
        {
            case 1:
            {
            	int memory;
            	char data[5];
                printf("\nEnter the space to be allocated : ");
                scanf("%d",&memory);
				printf("enter the data :");
				scanf("%s",data);
				aptr = modifyf(fptr,aptr,memory,data);
				printalloc(aptr);
				printfree(fptr);
				break;
			}
				
			case 2:
			{
				char free[5];
		    	printalloc(aptr);
				printf("\n Which memory you want to free\n");
				scanf("%s",free);
				aptr = free_fun(aptr,&fptr,free);
				printalloc(aptr);
				printfree(fptr);
				break;
			
			}
			case 3:
			{
				printalloc(aptr);
				printfree(fptr);
				break;
			}
			
			case 4:
				flag=0;
				
		
		}
		printf("for continuing the program 1 should be entered");
		scanf("%d",&flag);
	}
	return 0;
}
