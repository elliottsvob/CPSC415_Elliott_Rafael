/* disp.c : dispatcher
 */

#include <kernel.h>
#include <stdarg.h>

static pcb      *head = NULL;
static pcb      *tail = NULL;

static pcb 			*b_head = NULL;
static pcb			*b_tail = NULL;

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
      int i;
      r = contextswitch( p );
      	
	  kprintf("disp.c Case #: %d\n", r);
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
		p = next();
      break;
      case( SYS_PID ):
		p->ret = p->pid;
		p = next();
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
			kprintf("Message Sent: %s in disp\n", buffer);
		if ( send_error == SEND_ERROR){	
			kprintf("Send to process: %d failed\n", d_pid);
			
			}
		else if( send_error == NO_RECV){
			kprintf("Process: %d not blocking\n", d_pid);
			}
		p = next();		
      break;
      case( SYS_RECV ):	
	    
		ap = (va_list)p->args; 
		f_pid = va_arg(ap, unsigned int*);
		kprintf("disp.sys_recv: PID is: %d\n", f_pid);
		buffer = va_arg(ap, void*);
		buf_len = va_arg(ap, int); 	
		recv_error = recv(f_pid, buffer, buf_len,p);
		if(recv_error == INVALID_PID){
			kprintf("Process id: %d is invalid\n", *f_pid);
		}
		if(recv_error == PARAM_ERROR){
			kprintf("Receive error\n");
		}
		p = next();		
      break;
      case ( TIMER_INT ) :
		kprintf("disp.Timer interupt\n");
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
		kprintf( "Bad Sys request %d, pid = %d\n", r, p->pid );
      }
    }
	kprintf( "Out of processes: iddle process\n" );
/*	//TODO */
/*	r = contextswitch( &proctab[IDLE_PROC]);*/
	
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

    return( p );
}


