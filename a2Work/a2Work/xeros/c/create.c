/* create.c : create a process
 */

#include <kernel.h>


#include <i386.h>
#include <kernel.h>

pcb     proctab[MAX_PROC];

/* make sure interrupts are armed later on in the kernel development
 */
#define STARTING_EFLAGS         0x00003200

static int      nextpid = 1;



int      create( funcptr fp, int stack ) {
/***********************************************/

    context_frame       *cf;
    pcb                 *p = NULL;
    int                 i;

    if( stack < PROC_STACK ) {
        stack = PROC_STACK;
    }

    for( i = 0; i < MAX_PROC; i++ ) {
        if( proctab[i].state == STATE_STOPPED ) {
            p = &proctab[i];
            break;
        }
    }

    if( !p ) {
        return( -1 );
    }


    cf = kmalloc( stack*2);
    p->stack_top = (long) cf;
    if( !cf ) {
        return( -1 );
    }

    cf = (context_frame *)((int)cf + stack - 4);
    cf--;

    memset(cf, 0x81, sizeof( context_frame ));

    cf->iret_cs = getCS();
    cf->iret_eip = (unsigned int)fp;
    cf->eflags = STARTING_EFLAGS;

    cf->esp = (int)(cf + 1);
    cf->ebp = cf->esp;
    //set what ebp points to to the address of systop
    //allows the return key word to run sysstop();
    int *ptr = (int*)cf->esp;
    *ptr = (int)&sysstop;
    
    p->esp = (int)cf;
    p->state = STATE_READY;
	//message queues initialized to null

    
    if(p->pid == 0){
    	p->pid = i+1;
    	}
    else{
    	p->pid *= MAX_PROC;
    	}
    ready( p );
    return( p->pid );
}
