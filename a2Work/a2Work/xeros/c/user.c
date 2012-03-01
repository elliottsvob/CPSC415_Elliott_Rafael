/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */

 void producer( void ) {
/****************************/
//n i -> Q
    int         Q;


    for( Q = 0; Q < 5; Q++ ) {
        kprintf( "Produce %d\n", Q );

        sysyield();
    }

    return;
}
//extra comment done online
 void consumer( void ) {
/****************************/

    int         i;
		//char  str[] = "hello world!\0";
    for( i = 0; i < 5; i++ ) {
    		//sysputs(str);
        kprintf( "Consume %d \n", i );
        sysyield();
    }

    return;
}

 void     root( void ) {
/****************************/

    int i, j;
    kprintf("Root has been called\n");

    sysyield();
    sysyield();
    syscreate( &producer, 4096 );
    syscreate( &consumer, 4096 );

    for( ;; ) {
        sysyield();
    }
}
