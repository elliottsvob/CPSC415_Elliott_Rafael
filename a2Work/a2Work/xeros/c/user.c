/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */

 void producer( void ) {
/****************************/
//n i -> Q
    int         Q;

<<<<<<< HEAD
    for( Q = 0; Q < 5; Q++ ) {
        kprintf( "Produce %d\n", Q );
=======
    int        K;

    for( K = 0; K < 5; K++ ) {
        kprintf( "Produce %d\n", K );
>>>>>>> d074e74d3f80e67bad639c2b18e8e8aa80f4f6e1
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
