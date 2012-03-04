/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */

 void producer( void ) {
/****************************/
//n i -> Q
    int         Q;
	
    char c[] = "Hello World!\0\n";
   	int cons = 3;
		syssend (cons, &c, 15);
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
		char str[15];
		int prod = 2;
    sysrecv(NULL,&str,15);
    kprintf( "Consumer message: %s \n", str );
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
    int prod = syscreate( &producer, 4096 );
    int cons = syscreate( &consumer, 4096 );
    
    
    kprintf("Produce PID is : %d\n", prod);
    kprintf("Consume PID is : %d\n", cons);
    
    
		
			
		
    for( ;; ) {
        sysyield();
    }
}
