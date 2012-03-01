/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */

 void producer( void ) {
/****************************/

    int        K;

    for( K = 0; K < 5; K++ ) {
        kprintf( "Produce %d\n", K );
        sysyield();
    }

    sysstop();
}
//extra comment done online
 void consumer( void ) {
/****************************/

    int         i;

    for( i = 0; i < 5; i++ ) {
        kprintf( "Consume %d \n", i );
        sysyield();
    }

    sysstop();
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
