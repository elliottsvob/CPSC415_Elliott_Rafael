/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */
int cons;
int prod;
static int root_pid;
 void producer( void ) {
/****************************/
//n i -> Q
    int     i;
    int q=2;
		char  c[] ="Hello World!\n"; 
		
		
		syssend(cons,&c,17);
		char  print[15];

    for( i = 0; i < 5; i++  ) {
    		sprintf(&print, "Produce: %d\n", i);
    		sysputs(&print);
    		syssleep(10000);
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
		
		unsigned int a = prod;
		sysrecv (&a, &c, 17);
		char print[15];
		
    
    sysputs(&c);
    for( i = 0; i < 5; i++ ) {
    		sprintf(&print, "Consumer: %d\n", i);
    		sysputs(&print);
    		syssleep(10000);
    		sysyield();
    }
   
    

    return;
}
void     foo( void ) {
/****************************/
		syssleep(1000);
		char c[50];
		int i = sysgetpid();
		sprintf(c, "foo Process %d is alive\n", i);
		sysputs(c);
		syssleep(5000);
		int sleep_time;
		char s_t[5];
		memset(s_t,0,5);
		int r_call = sysrecv(&root_pid,s_t,5);
		char r[5];
		sprintf(r, "PID: %d Sysrecv returned: %d\n", i,r_call); 
		sysputs(r);
		if(!r_call){
			memset(c, 0, 50);
			sprintf(c, "PID: %d Error receiving from Root\n", i);
			sysputs(c);
			return;
		}
			sleep_time =  atoi(s_t);
			syssleep(sleep_time);
			memset(c, 0, 50);
			sprintf(c, "PID: %d sleep finished, exiting slept: %d\n", i,sleep_time);
			sysputs(c);
			
}

 void     root( void ) {
/****************************/

    int a,b,c,d,i;
  //kprintf("Root has been called\n");
		char msg[128];
		
		root_pid = sysgetpid();
/*   cons = syscreate( &consumer, 4096 );*/
/*	 prod = syscreate( &producer, 4096 );*/

	//kprintf("/-------------------\n");
	//kprintf("Producer ID: %d\n", prod);
	//kprintf("Consumer ID: %d\n", cons);
	//kprintf("/-------------------\n");
	
	  a = syscreate(&foo, 4096);
		sprintf(msg, "Root created 1 Process %d is alive\n", a);	
		sysputs(msg);
		b = syscreate(&foo, 4096);
		memset(msg,0,128);
		sprintf(msg, "Root created 2 Process %d is alive\n", b);	
		sysputs(msg);
		memset(msg,0,128);
		c= syscreate(&foo, 4096);
		sprintf(msg, "Root created 3 Process %d is alive\n", c);	
		sysputs(msg);
		memset(msg,0,128);
		d = syscreate(&foo, 4096);
		sprintf(msg, "Root created 4 Process %d is alive\n", d);	
		sysputs(msg);
		memset(msg,0,128);
		
		syssleep(4000);
		
		
		
		
		char a_time[5];sprintf(a_time, "%d",10000);
		char b_time[4];sprintf(b_time, "%d",7000);
		char c_time[5];sprintf(c_time, "%d", 27000);
		char d_time[5];sprintf(d_time, "%d",20000);
		
		syssend(a,&a_time,5);
		syssend(b,&b_time,4);
		syssend(c,&c_time,5);
		syssend(d,&d_time,5);
		
		
		
	
     for( ;; ) 
    {
        sysyield();
    };
}

void     idleproc( void ) {
/****************************/		
	 for( ;; ){
   	}
}

