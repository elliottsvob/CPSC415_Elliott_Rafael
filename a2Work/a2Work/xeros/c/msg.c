/* msg.c : messaging system (assignment 2)
 */

#include <kernel.h>
#include <stdarg.h>

/* Your code goes here */

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
		if(proctab[dest_pid%10 -1].state == STATE_STOPPED){
				
			return code;
		}
		
		if(proctab[dest_pid%10 - 1].state == STATE_BLOCKED){
				
				r = &proctab[dest_pid%10 -1];
				ap = (va_list)r->args;
				pid = va_arg(ap, unsigned int *);
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
				
		}else{
			s->state = STATE_BLOCKED;
			code = NO_RECV;
			return code;
			}	
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
	
	//TODO	
	/*	if(!from_pid){
			//recv from any 
		}*/
		
		
		if(proctab[*from_pid%10 -1].state == STATE_BLOCKED){
			s = &proctab[*from_pid%10 -1];
			ap = (va_list)s->args;
			pid = va_arg(ap, int);
			s_buf = va_arg(ap, void*);
			kprintf("s_buf: %s\n", s_buf);
			s_len = va_arg(ap, int);
			kprintf("Receive \n");
			if (s_len < buffer_len){
				blkcopy(buffer,s_buf , s_len);
				code = s_len;
			}else {
				blkcopy(buffer,s_buf, buffer_len);
				code = buffer_len;
			}
			ready ( s );
			ready ( r );
		}
		else {
		 r->state = STATE_BLOCKED;
		}		
	
}
