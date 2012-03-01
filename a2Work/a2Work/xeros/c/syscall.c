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
        : "g" (req), "g" (ap), "i" (KERNEL_INT)
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

void	sysputs( char *str ){
/**************************/
	
    syscall( SYS_PUTS, str );
}
