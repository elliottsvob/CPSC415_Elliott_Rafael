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
    unsigned int* f_pid;
    void * buffer;
    
    int recv_error;
    

    for( p = next(); p; ) {
      //      kprintf("Process %x selected stck %x\n", p, p->esp);
      int i;
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
	kfree(p->stack_top);
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
	
	if(!send(d_pid, buffer, buf_len,p){
		kprintf("Send to process: %d failed\n", d_pid);
		}
	break;
			case( SYS_RECV ):
	
	ap = (va_list)p->args; 
	f_pid = va_arg(ap, unsigned int*);
	buffer = va_arg(ap, void*);
	buf_len = va_arg(ap, int); 
	
	recv_error = recv(f_pid, buffer, buf_len);
	if(recv_error == INVALID_PID){
		kprintf("Process id: %d is invalid\n", f_pid);
	}
	if(recv_error == PARAM_ERROR){
		kprintf("Receive error\n");
	}
	
		
      default:
	kprintf( "Bad Sys request %d, pid = %d\n", r, p->pid );
      }
    }

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

