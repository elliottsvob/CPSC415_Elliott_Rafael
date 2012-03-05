/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */

 void producer( void ) {
/****************************/
//n i -> Q
    int     i;
    int q=2;
		char  c[] ="Hello World!\n"; 
		
		int cons = 2; 
		syssend(cons,&c,17);
		char  print[15];

    for( i = 0; i < 5; i++  ) {
    		sprintf(&print, "Produce: %d\n", i);
    		sysputs(&print);
        sysyield();
    }

    return;
  	
}
//extra comment done online
 void consumer( void ) {
/****************************/

    int         i;
    int 				k;
		 char c[17] ;
		
		unsigned int a = 0;
		sysrecv (&a, &c, 17);
		char print[15];
		
    
    sysputs(&c);
    for( i = 0; i < 5; i++ ) {
    		sprintf(&print, "Consumer: %d\n", i);
    		sysputs(&print);
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
  //kprintf("Root has been called\n");
	
	
  
  int cons = syscreate( &consumer, 4096 );
	int prod = syscreate( &producer, 4096 );

	//kprintf("/-------------------\n");
	//kprintf("Producer ID: %d\n", prod);
	//kprintf("Consumer ID: %d\n", cons);
	//kprintf("/-------------------\n");
	
     for( ;; ) 
    {
        sysyield();
    };
}

void     idleproc( void ) {
/****************************/		
    for( ;; ){
    sysyield();
    }
}

