/* msg.c : messaging system (assignment 2)
 */

#include <kernel.h>
#include <stdarg.h>

/* Your code goes here */
//----------------------------------------------------
//							Helper MEthods
//----------------------------------------------------
//adds the sender (s) process to the receiver's queue
void add_recvQ(pcb *s, pcb *r)
{
	if (r->receive_queue == NULL) {
		r->receive_queue = s;
		r->receive_qtail = s;
		//s->next = NULL; // cannot be done cuz dispatcher uses next to call next process.
	}else {
		r->receive_qtail->next = s;
		r->receive_qtail = s;
	}
	s->state = STATE_BLOCKED;
}

//Returns 1 if found, 0 otherwise
//NOTE:  This method also removes the process from the queue
int in_recvQ(pcb *s, pcb *r)
{
	pcb *p;

	if (r->receive_queue == NULL) {
		r->state = STATE_BLOCKED;
		return 0;
	}
	if (r->receive_queue == s){
		//checks if there was only one element in the queue
		if (r->receive_qtail == s) 	{
			r->receive_queue = NULL;
			r->receive_qtail = NULL;
			return 1;
		}
		r->receive_queue = r->receive_queue->next;
		return 1;
	}
	//
	p = r->receive_queue;
	while (p != r->receive_qtail){
		//if it finds the sender process in the queue,
		//it is remove from the queue and returns 1
		//else, we iterate till the end
		if(p->next == s){
			if (r->receive_qtail = s){
				p->next = NULL;
				r->receive_qtail = p;
			}
			else {
				p->next = p->next->next;
			}
			return 1;
		}
		p = p->next;
	}

	return 0;
}
//----------------------------------------------------
//							External Methods
//----------------------------------------------------
int send(int dest_pid, void *buffer, int buffer_len, pcb * s)
{
	
	va_list ap;
	pcb * r; 
	int i ;
	
	void * r_buf;
	int 	 r_len;
	unsigned int * pid;
	int code = SEND_ERROR;
		
		kprintf("Send got : %s \n", buffer);
		r = &proctab[dest_pid%MAX_PROC -1];

		if(r->state == STATE_STOPPED){
			return code;
		}
		//finds the reciver based on the destination process id
		if(r->state == STATE_BLOCKED){
						
			ap = (va_list)r->args;
			pid = va_arg(ap, unsigned int *);
			
			if (*pid == s->pid || *pid ==0){
				r_buf = va_arg(ap, void*);
				r_len = va_arg(ap, int);
				
				if (r_len < buffer_len){
						blkcopy(r_buf, buffer, r_len);
						code = r_len;
				}else {
						blkcopy(r_buf, buffer, buffer_len);
						code = buffer_len;
				}
				kprintf("%s buffer\n", r_buf);
				ready ( s );
				ready ( r );
				return code;
			}
				
		}
		//s->state = STATE_BLOCKED;
		add_recvQ (s, r);
		code = NO_RECV;
		return code;	
}
int recv(unsigned int  * from_pid, void * buffer, int buffer_len, pcb * r)
{
	va_list ap;
	pcb * s; 
	int i ;
	
	void * s_buf;
	int 	 s_len;
	unsigned int * pid;
	int code = -1;
	
	if(*from_pid==0){
	// This code is run when receive from any
		if(r->receive_queue = NULL){
			r->state = STATE_BLOCKED;
			return;
		} else {
			s = r->receive_queue;
		}
	}else{
		s = &proctab[*from_pid%MAX_PROC -1];	
	}	
	
	if (in_recvQ(s,r)) {		
		//if(proctab[*from_pid%10 -1].state == STATE_BLOCKED){
		//s = &proctab[*from_pid%10 -1];
			ap = (va_list)s->args;
			pid = va_arg(ap, int);
			s_buf = va_arg(ap, void*);
			kprintf("s_buf: %s\n", s_buf);
			s_len = va_arg(ap, int);
			//kprintf("Receive \n");
			if (s_len < buffer_len){
				blkcopy(buffer,s_buf , s_len);
				code = s_len;
			}else {
				blkcopy(buffer,s_buf, buffer_len);
				code = buffer_len;
			}
			ready ( s );
			ready ( r );
	} else {
		 r->state = STATE_BLOCKED;
	}		
}

