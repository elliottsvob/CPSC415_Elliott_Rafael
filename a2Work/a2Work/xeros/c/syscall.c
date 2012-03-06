
/* syscall.c : syscalls
 */

#include <stdarg.h>
#include <kernel.h>

 int syscall( int req, ... ) {
/**********************************/

    va_list	ap;
    int		rc;
 
    va_start( ap, req );

    __asm __volatile( " \
        movl %1, %%eax \n\
        movl %2, %%edx \n\
        int  %3 \n\
        movl %%eax, %0 \n\
        "
        : "=g" (rc)
        : "g" (req), "g" (ap), "i" (SYSCALL_INT)
        : "%eax" 
    );
 
    va_end( ap );

    return( rc );
}


 int syscreate( funcptr fp, int stack ) {
/*********************************************/

    return( syscall( SYS_CREATE, fp, stack ) );
}

 int sysyield( void ) {
/***************************/

    return( syscall( SYS_YIELD ) );
}

 int sysstop( void ) {
/**************************/

    return( syscall( SYS_STOP ) );
}

//---------------------------------------------------
// Done by RT
//---------------------------------------------------
int	sysgetpid( void ){
/**************************/

    return( syscall( SYS_PID ) );
}

int	sysputs( char *str ){
/**************************/
	
    return syscall( SYS_PUTS, str );
}
//---------------------------------------------------
// Done by ES
//---------------------------------------------------
int syssend( int dest_pid, void * buffer, int buffer_len){
/**************************/
		return( syscall(SYS_SEND, dest_pid, buffer, buffer_len));
} 
//---------------------------------------------------
// Done by ES
//---------------------------------------------------
int sysrecv( unsigned int *from_pid, void *buffer, int buffer_len ){
		
		return( syscall(SYS_RECV, *from_pid, buffer, buffer_len));
}
int syssleep(unsigned int milliseconds)
{
	return syscall(SYS_SLEEP, milliseconds);
}


