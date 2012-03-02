/* msg.c : messaging system (assignment 2)
 */

#include <kernel.h>

/* Your code goes here */

int send(int dest_pid, void *buffer, int buffer_len, pcb * s)
{
	
	va_list ap;
	pcb * r; 
	int i ;
	
	void * r_buf;
	int 	 r_len;
	unsigned int * pid;
	int code = -1;
		
	
		if(proctab[dest_pid%10].state == STATE_STOPPED){
			return code;
		}
		else if(proctab[dest_pid%10].state == STATE_BLOCKED){
				
				r = &proctab[dest_pid%10];
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
				ready ( p );
				ready ( s );
				return code;
				
		}else{
			s->state = STATE_BLOCKED 
			}	
}
int recv(unsigned int * from_pid, void * buffer, int buffer_len, pcb * r)
{
	va_list ap;
	pcb * s; 
	int i ;
	
	void * s_buf;
	int 	 s_len;
	unsigned int * pid;
	int code = -1;

		if(proctab[from_pid%10].state == STATE_BLOCKED){
			s = &proctab[from_pid%10];
			ap = (va_list)s->args;
			pid = va_arg(ap, int);
			s_buf = va_arg(ap, void*);
			s_len = va_arg(ap, int);
			
			}
		if(*from_pid == 0){
			for (i = 0; i < MAX_PROC; i++)
	
}
