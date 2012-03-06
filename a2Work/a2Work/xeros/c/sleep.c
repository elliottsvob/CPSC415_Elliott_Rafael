/* sleep.c : sleep device (assignment 2)
 */

#include <kernel.h>
static pcb      *head = NULL;
/* Your code goes here */

//------------------------------------------
//							Helper Methods
//------------------------------------------
void push_sleep(pcb *r)
{
	/****	adds the process to the sleeping queue ***/
	int slp_time = r->sleep_time;
	
	r->sleep_next = NULL;

	if (head==NULL){
		head = r;
		return;
	}
	pcb *temp = head;
	if (temp->sleep_time >= slp_time)
	{
		//If the sleeping time of new process is less than that of the head
		r->sleep_next = temp;
		head = r;
		return;
	}
	while ((temp->sleep_next!=NULL) && (temp->sleep_next->sleep_time < slp_time))
	{
		//Advances in the linked list till the proper position to insert is found
		temp = temp-> sleep_next;
	}
	if(temp->sleep_next==NULL)
	{
		//adds the process to the tail of the queue
		temp->sleep_next = r;
		return;
	}
	//adds the process in between two processes
	r->sleep_next = temp->sleep_next;
	temp->sleep_next = r;
}
//------------------------------------------
//							Public Methods
//------------------------------------------
int sleep ( pcb *p, unsigned int milliseconds){
	p->sleep_time = milliseconds;
	p->state = STATE_BLOCKED;
	push_sleep (p);
	return 0;
}
void tick (){
	pcb *p = head;
	while (p)
	{
		//reduces the sleep time by a clock tick
		p->sleep_time -= 10;
		p = p->sleep_next;
	}
	
	while ((head)&&(head->sleep_time<=0))
	{
		p = head;
		head = head->sleep_next;
		p->sleep_next = NULL;
		ready(p);
	}
	return;
}

