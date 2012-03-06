
/* disp.c : dispatcher
 */

#include <kernel.h>
#include <stdarg.h>

static pcb      *head = NULL;
static pcb      *tail = NULL;

static pcb 			*b_head = NULL;
static pcb			*b_tail = NULL;
void clear();
void     dispatch( void ) {
/********************************/

    pcb         *p;
    int         r;
    funcptr     fp;
    int         stack;
    va_list     ap;
    char	*str;
    int buf_len;
    int d_pid;
    unsigned int *f_pid;
    void * buffer;
    unsigned int milliseconds;
    int recv_error;
    int send_error;
    
	/*proctab[IDLE_PROC].state == STATE_BLOCKED;
	head ->next = NULL;
	tail = head;*/

	//while(1){
	
    for( p = next(); p; ) {
      //      kprintf("Process %x selected stck %x\n", p, p->esp);

      int i, k;
      r = contextswitch( p );
      
      switch( r ) {

      case( SYS_CREATE ):
	ap = (va_list)p->args;
	fp = (funcptr)(va_arg( ap, int ) );
	stack = va_arg( ap, int );
	p->ret = create( fp, stack );
	break;
      case( SYS_YIELD ):
	ready( p );
	p = next();
	break;
      case( SYS_STOP ):
	p->state = STATE_STOPPED;
	kfree((void*)p->stack_top);
	clear(p);
	p = next();
	break;
      case( SYS_PID ):
	p->ret = p->pid;
	
	break;
      case( SYS_PUTS ):
  ap = (va_list)p->args;
	str = va_arg(ap,char*);
	kprintf( "%s", str );
	
	break;
			case( SYS_SEND ):
	ap =(va_list)p->args;
	d_pid = va_arg(ap, int);
	buffer = va_arg(ap, void*);
	buf_len = va_arg(ap, int); 
	send_error = send(d_pid, buffer, buf_len,p);
	if ( send_error == SEND_ERROR){	
		kprintf("Send to process: %d failed\n", d_pid);
		ready(p);
		}
	else if( send_error == NO_RECV){
		kprintf("Process: %d not blocking\n", d_pid);
				ready(p);
		}
	//KEY: The syssend return value wasn-t return to the process
	p->ret = send_error;
	p = next();		
	break;
	
			case( SYS_RECV ):	
			//kprintf("RECV\n");
	ap = (va_list)p->args; 
	f_pid = va_arg(ap, unsigned int*);
	
	//	kprintf("PID is: %d\n", f_pid);
	 
	buffer = va_arg(ap, void*);
	buf_len = va_arg(ap, int); 	
	recv_error = recv(f_pid, buffer, buf_len,p);

	if(recv_error == INVALID_PID){
		kprintf("Process id: %d is invalid\n", *f_pid);
				ready(p);
	}
	if(recv_error == PARAM_ERROR){
		kprintf("Receive error\n");
				ready(p);
	}
	//KEY: The sysrecv return value wasn't return to the process
	p->ret = recv_error;
	p = next();		
	break;

	
			case ( TIMER_INT ) :
				//kprintf("timer went off\n");
				ready( p );
				p = next();
				tick();
				end_of_intr();     
				break;
			case( SYS_SLEEP ):
				ap = (va_list)p->args;
				milliseconds = va_arg( ap, unsigned int );
				sleep( p, milliseconds );
				p = next();
      	break;
			default:
       	process_table_dump();
				kprintf( "Bad Sys request %d, pid = %d\n", r, p->pid ); 				 
				break;
      }
    }
    kprintf( "Out of processes: idle process\n" );
		kprintf( "Out of processes: dying\n" );
    for( ;; );
}

extern void dispatchinit( void ) {
/********************************/
	int i;
  //bzero( proctab, sizeof( pcb ) * MAX_PROC );
  memset(proctab, 0, sizeof( pcb ) * MAX_PROC);  
 
}

extern void     ready( pcb *p ) {
/*******************************/

    p->next = NULL;
    p->state = STATE_READY;

    if( tail ) {
        tail->next = p;
    } else {
        head = p;
    }

    tail = p;
}

/*extern void blocked( pcb *p){*/

/*		p->next = NULL;*/
/*		p->state = STATE_BLOCKED;*/
/*		*/
/*		if(b_tail){*/
/*				tail->next = p;*/
/*		} else {*/
/*			b_head = p;*/
/*		}*/
/*		tail = p;*/
/*}*/

extern pcb      *next( void ) {
/*****************************/

    pcb *p;
 
    p = head;

    if( p ) {
        head = p->next;
        if( !head ) {
            tail = NULL;
        }
    }
		if( !p ){
			process_table_dump();
			for(;;);
			return &proctab[0];
		}
    return( p );
}
//Clears a pcb's waiting sender queue to avoid deadlock
void clear(pcb* p){	
	while(p->sender){
		p->sender->ret = -1;
		ready(p->sender);
		p = p->sender;
		}
	}

