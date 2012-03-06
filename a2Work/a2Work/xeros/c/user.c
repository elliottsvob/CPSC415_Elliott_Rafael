/* user.c : User processes
 */

#include <kernel.h>

/* Your code goes here */
int cons;
int prod;
int t3;
int t4a;
int t4b;

int root_pid;

 void send_test_a( void ) {
/****************************/
//n i -> Q
    int     i;
    int q=2;
		char  c[] ="Hello World!\n"; 
		int ret;
		
		syssend(cons,&c,17);
		char  print[15];

    for( i = 0; i < 5; i++  ) {
    		sprintf(&print, "Produce: %d\n", i);
    		sysputs(&print);
    		syssleep(10000);
        sysyield();
    }
    memset(c,0,17);
    ret = sysrecv(&cons, c,17);
    
    sysputs(c);

    return;
  	
}
//extra comment done online
 void send_test_b( void ) {
/****************************/

    int         i;
    int 				k;
		char c[17] ;
		int ret;
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
    
    //Proves Hello World was modified and sent back;
    c[0] = 'W';
    c[6] = 'H';
    
   
    ret = syssend(prod, &c,17);
	
    return;
}
void test_3( void ) {
/****************************/
		//Process tries sending a message to itself
		char  c[] ="Hello World!\n"; 
		int ret;
		
		ret = syssend(t3,&c,17);
    
  		char r[50];
		sprintf(r, "PID: %d Syssend() returned: %d\n", t3 ,ret); 
		sysputs(r);	
    return;
}
void test_4a( void ) {
/****************************/
		//Process tries receiving a message from itself
		char  c[20];
		int ret;
		ret = sysrecv(&t4a,&c,17);
    
  		char r[50];
		sprintf(r, "PID: %d Sysrecv() returned: %d\n", t4a ,ret); 
		sysputs(r);	
		syssleep(1000);
    return;
}
void test_4b( void ) {
/****************************/
		//Process tries receiving with empty buffer
		char  c;
		int ret;
		int send_to = 1;
		ret = sysrecv(&send_to,&c,17);
    
  		char r[50];
		sprintf(r, "PID: %d Sysrecv() returned: %d\n", t4b ,ret); 
		sysputs(r);	

    return;
  	
}
void     foo( void ) {
/****************************/

		syssleep(1000);

		char c[50];

		int i = sysgetpid();
		sprintf(c, "Process %d is alive\n", i);
		sysputs(c);

		syssleep(5000);


		
		int s_t = -1;
		//memset(s_t,0,5);
		int r_call = sysrecv(&root_pid,&s_t,4);
		
		char r[50];
		sprintf(r, "PID: %d Message Received, Sleeping: %d\n", i,s_t); 
		sysputs(r);


		if(!r_call){
			memset(c, 0, 128);
			sprintf(c, "PID: %d Error receiving from Root\n", i);
			sysputs(c);
			return;
		}

			
			syssleep(s_t);
			memset(c, 0, 50);
			sprintf(c, "PID: %d sleep exiting \n", i);

			sysputs(c);
			return;
}

 void     root( void ) {
/****************************/

    int a,b,c,d,i;
    int ret;
  //kprintf("Root has been called\n");
		char msg[128];
		
		root_pid = sysgetpid();


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

		int a_time = 10000;//[5];sprintf(a_time, "%d",10000);
		int b_time = 7000;//[4];sprintf(b_time, "%d",7000);
		int c_time = 20000;//[5];sprintf(c_time, "%d", 27000);
		int d_time = 27000;//[5];sprintf(d_time, "%d",20000);
		
		char r[50];

		ret = syssend(a,&a_time,4);
		//sprintf(r, "PID: %d Syssend returned: %d\n", a ,ret); 
		//sysputs(r);	
		
		ret = -1;


		ret = syssend(b,&b_time,4);
		//sprintf(r, "PID: %d Syssend returned: %d\n", b ,ret); 
		//sysputs(r);	
	
		ret = -1;

		ret = syssend(c,&c_time,4);
		//sprintf(r, "PID: %d Syssend returned: %d\n", c ,ret); 
		//sysputs(r);	
		
		ret = -1;

		ret = syssend(d,&d_time,4);
		//sprintf(r, "PID: %d Syssend returned: %d\n", d ,ret); 
		//sysputs(r);	
	
		ret = -1;
	
		
		char recv_fr_4[50];
		ret = sysrecv(&d,recv_fr_4,20);
		sprintf(r, "\nROOT: sysrecv() From PID:%d Returned Status %d\n\n",d,ret); 
		sysputs(r);	
		
		//clear r
		memset(r,0,50);
		
		char send_to_3[]="balls\n";
		ret = syssend(c, send_to_3, 6);
		sprintf(r, "\nROOT: syssend() To PID: %d Returned Status %d\n\n",c,ret); 
		sysputs(r);	
		
		return;
}

void     idleproc( void ) {
/****************************/		
	 for( ;; ){
	 	
   	}
}

void test_1_and_2(){

	 cons = syscreate( &send_test_b, 4096 );
	 prod = syscreate( &send_test_a, 4096 );
	
	return;
}
 
void test3(){

	 t3 = syscreate( &test_3, 4096 );
	return;
}
void test4a(){

 	 t4a = syscreate( &test_4a, 4096 );	
	return;
}
void test4b(){
 	 t4b = syscreate( &test_4b, 4096 );

	return;
}
 
void test5_h(){
/*Shows that time sharing is working*/
//this process and another which alternate print statments	with below
	char c[] = "I'm doing alright.\n";
	for (;;){
		sysputs(c);
		}
}

void test5(){
/*Shows that time sharing is working*/
//this process and another which alternate print statments with above
	char c[] = "Hi, How are you?\n";
	syscreate(&test5_h, 4096);
	for(;;){
		sysputs(c);
	}

}




