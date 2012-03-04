/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */

 void producer( void ) {
/****************************/
//n i -> Q
    int         Q;
	
    char c[] = "Hello World!\0\n";
    
		syssend (2, &c, 13);
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
		char  str[15];
		int prod = 3;

    sysrecv(&prod,&str,15);
    kprintf( "Consumer message: %s \n", str );
    for( i = 0; i < 5; i++ ) {
    		//sysputs(str);
        kprintf( "Consume %d \n", i );
        sysyield();
    }
   
    

    return;
}
void     foo( void ) {
/****************************/

    kprintf("foo!\n");  	
		
    sysstop();
}
 void     root( void ) {
/****************************/

    int i, j;
     
   
    
    kprintf("Root has been called\n");
	
	//syscreate( &foo, 4096 );
	//sysyield();
	int cons = syscreate( &consumer, 4096 );
	int prod = syscreate( &producer, 4096 );

	kprintf("/-------------------\n");
	kprintf("Producer ID: %d\n", prod);
	kprintf("Consumer ID: %d\n", cons);
	kprintf("/-------------------\n");
    

    
    sysyield();

    
    //sysstop();
			
		
    for( ;; ) {
        sysyield();
    }
}

void     idleproc( void ) {
/****************************/

    kprintf("Iddle process has been called\n");  	
		
    for( ;; ) {
        //sysyield();
    }
}

