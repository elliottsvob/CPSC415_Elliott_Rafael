/* msg.c : messaging system (assignment 2)
 */

#include <kernel.h>
#include <stdarg.h>

/* Your code goes here */
int ipc_valid(int* d, void * b, int l);
void process_table_dump();
void push(pcb *s, pcb*r);
pcb*poll(pcb*r);
int peek(pcb*r);
int send(int dest_pid, void *buffer, int buffer_len, pcb * send_pcb)
{
		pcb * dest_pcb;
		va_list recvr_args;
		void *recv_buf;
		int recv_len;
		int code;
		
		dest_pcb = &proctab[(dest_pid%MAX_PROC) -1];
		
		switch (dest_pcb->state){
			case (STATE_BLOCKED):
				recvr_args = (va_list)dest_pcb->args;
				va_arg(recvr_args,unsigned int*);
				recv_buf = va_arg(recvr_args, void*);
				recv_len = va_arg(recvr_args, int);
				
				if (recv_len < buffer_len){
						blkcopy(recv_buf, buffer, recv_len);
						code = recv_len;
				}else {
						blkcopy(recv_buf, buffer, buffer_len);
						code = buffer_len;
				}
				
				ready(dest_pcb);
				ready(send_pcb);
				break;
				
			case (STATE_STOPPED):
				code = -1;
				break;
				
			default:
				send_pcb->state = STATE_BLOCKED;
				dest_pcb->sender = send_pcb;
				code = 0;
				break;
				}
	//process_table_dump();		
	//wait();

	return code;
}
int recv(unsigned int  * from_pid, void * buffer, int buffer_len, pcb * recv_pcb)
{
	pcb * send_pcb;
	va_list send_args;
	void *send_buf;
	int send_len;
	int code;
	
	
	if(!from_pid && peek(recv_pcb)){
	
		send_pcb = poll(recv_pcb);
	}	
	else{
		send_pcb = &proctab[(from_pid%MAX_PROC) -1];
	}
	switch (send_pcb->state){
			case (STATE_BLOCKED):
			
				send_args = (va_list)send_pcb->args;
				va_arg(send_args,unsigned int*);
				send_buf = va_arg(send_args, void*);
				send_len = va_arg(send_args, int);
				
				if (send_len < buffer_len){
						blkcopy(buffer, send_buf, send_len);
						code = send_len;
				}else {
						blkcopy(buffer, send_buf, buffer_len);
						code = buffer_len;
				}
				ready(send_pcb);
				ready(recv_pcb);
				break;
				
			case (STATE_STOPPED):
			
				code = -1;
				break;
				
			default:
				recv_pcb->state = STATE_BLOCKED;
				code = 0;
				break;
				}
	//process_table_dump();
	//wait();
	return code;
	
}

void process_table_dump()
{
	pcb*p;
	
	int i; 
	kprintf("-------------------------------------------\n");
	for (i=0;i < 4; i++){
		p = &proctab[i];
		kprintf("Process: %d at index: %d \nHas State:",     p->pid,i);
		
		switch (p->state){
			case (STATE_STOPPED):
				kprintf("STATE_STOPPED\n");break;
			
			case (STATE_READY):
				kprintf("STATE_READY\n");break;
			case (STATE_BLOCKED): 
				kprintf("STATE_BLOCKED\n");break;
			default:break;
		
		}
		kprintf("-------------------------------------------\n");
		}
	}

void wait(){
	int i = 0;
	for( i ; i < 9999999;i++){}
}

void push(pcb *s, pcb*r){
		pcb *temp = r;
		while(temp->sender){
			temp = temp->sender;
		}
		temp->sender = s;
}
pcb * poll(pcb*r)
{
	pcb * s = r->sender;
	r->sender = r->sender->sender;
	return s;
	
}
int peek(pcb*r)
{
	if (r->sender){
		return 1;
	}else{
		return 0;
	}	
}

