
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
		unsigned int * c;
		unsigned int *expected_sender;
		
		if(dest_pid == send_pcb->pid)
		{
			return -1;
		}
		dest_pcb = &proctab[(dest_pid%MAX_PROC) -1];
		//kprintf("Dest pcb state is: %d\n", dest_pcb->pid);
		switch (dest_pcb->state){
			case (STATE_BLOCKED):
			
				recvr_args = (va_list)dest_pcb->args;

				expected_sender = va_arg(recvr_args,unsigned int*);
				if (expected_sender==0 || expected_sender==send_pcb->pid )
				{
					recv_buf = va_arg(recvr_args, void*);
					recv_len = va_arg(recvr_args, int);
				
					if (recv_len < buffer_len){
							blkcopy(recv_buf, buffer, recv_len);
							code = recv_len;
							dest_pcb->ret = code;
							//kprintf("msg.send 1.a Size: %d FROM PID: %d\n", code, send_pcb->pid);
					}else {
							blkcopy(recv_buf, buffer, buffer_len);
							code = buffer_len;
							dest_pcb->ret = code;
							//kprintf("msg.send 1.b Size: %d FROM PID: %d\n", code, send_pcb->pid);
					}
				
					ready(dest_pcb);
					ready(send_pcb);
				}
				else
				{
					code = 0;
					//kprintf("msg.send 2. Size: %d FROM PID: %d\n", code, send_pcb->pid);
					send_pcb->state = STATE_BLOCKED;
					push(send_pcb,dest_pcb);

				}
				break;
			case (STATE_STOPPED):
				code = -1;
				break;
				
			default:
				send_pcb->state = STATE_BLOCKED;
				push(send_pcb,dest_pcb);
				code = 0;
				break;
				}
	
	//kprintf("msg.send 3. Size: %d FROM PID: %d\n", code, send_pcb->pid);
	return code;
}
int recv(unsigned int  * from_pid, void * buffer, int buffer_len, pcb * recv_pcb)
{
	pcb * send_pcb;
	va_list send_args;
	void *send_buf;
	int send_len;
	int code=0;
	
	if(!buffer && buffer_len>0)
		return -1;
	
	if(!from_pid ){
		
		if( peek(recv_pcb)){
			send_pcb = poll(recv_pcb);			
		}else{
			if(*from_pid == recv_pcb->pid){
				return -1;
			}
			recv_pcb->state = STATE_BLOCKED;
			return 0;
			}
	}
	else{
		send_pcb = &proctab[((int)from_pid%MAX_PROC) -1];
	}
	switch (send_pcb->state){
			case (STATE_BLOCKED):
			
				send_args = (va_list)send_pcb->args;
				int expected_receiver = va_arg(send_args,int);
				
				if(expected_receiver==recv_pcb->pid) {
					send_buf = va_arg(send_args, void*);
					send_len = va_arg(send_args, int);
				
					if (send_len < buffer_len){
							blkcopy(buffer, send_buf, send_len);
							code = send_len;
							send_pcb->ret = code;
							//kprintf("msg.recv 1:0 %d PIF: %d\n", code, recv_pcb->pid);
					}else {
							blkcopy(buffer, send_buf, buffer_len);
							code = buffer_len;
							send_pcb->ret = code;
							//kprintf("msg.recv 2: %d PIF: %d\n", code, recv_pcb->pid);
					}
					ready(send_pcb);
					ready(recv_pcb);
				}
				else
				{
					recv_pcb->state = STATE_BLOCKED;
					code = 0;
				}
				break;
				
			case (STATE_STOPPED):
			
				code = -1;
				kprintf("msg.recv 3: %d PIF: %d\n", code, recv_pcb->pid);
				break;
				
			default:
				recv_pcb->state = STATE_BLOCKED;
				code = 0;
				break;
				}
	//kprintf("msg.recv 3: %d PIF: %d\n", code, recv_pcb->pid);
	return code;
	
}

void process_table_dump()
{
	pcb*p;
	
	int i; 
	kprintf("-------------------------------------------\n");
	for (i=0;i < 6; i++){
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

void pause(){
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
	return r->sender ? 1:0;
}

