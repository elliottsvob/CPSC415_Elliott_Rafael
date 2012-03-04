/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */

 void producer( void ) {
/****************************/
//n i -> Q
    int         Q;
	
    char c[] = "Hello World!\0\n";
<<<<<<< HEAD
   	int cons = 3;
		syssend (cons, &c, 15);
=======
    
		syssend (2, &c, 13);
>>>>>>> 78399f83d6fd5a0a85ca5fe63ff5f6b51a364f18
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
<<<<<<< HEAD
		//char  str[] = "hello world!\0";
		char str[15];
		int prod = 2;
    sysrecv(NULL,&str,15);
=======
		char  str[15];
		int prod = 3;

    sysrecv(&prod,&str,15);
>>>>>>> 78399f83d6fd5a0a85ca5fe63ff5f6b51a364f18
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

<<<<<<< HEAD


=======
    kprintf("foo!\n");  	
		
    sysstop();
}
>>>>>>> 78399f83d6fd5a0a85ca5fe63ff5f6b51a364f18
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
    

    
<<<<<<< HEAD
    kprintf("Produce PID is : %d\n", prod);
    kprintf("Consume PID is : %d\n", cons);
    
    
		
=======
    sysyield();

    
    //sysstop();
>>>>>>> 78399f83d6fd5a0a85ca5fe63ff5f6b51a364f18
			
		
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

