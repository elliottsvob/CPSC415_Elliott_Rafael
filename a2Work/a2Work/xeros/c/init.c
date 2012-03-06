/* initialize.c - initproc */

#include <i386.h>
#include <kernel.h>

extern	int	entry();        /* start of kernel image, use &start    */
extern	int	end();          /* end of kernel image, use &end        */
extern  long	freemem; 	/* start of free memory (set in i386.c) */
extern char	*maxaddr;	/* max memory address (set in i386.c)	*/
extern void test_1_and_2();
extern void test3();
extern void test4a();
extern void test4b();
extern void test5();
/************************************************************************/
/***				NOTE:				      ***/
/***								      ***/
/***   This is where the system begins after the C environment has    ***/
/***   been established.  Interrupts are initially DISABLED.  The     ***/
/***   interrupt table has been initialized with a default handler    ***/
/***								      ***/
/***								      ***/
/************************************************************************/

/*------------------------------------------------------------------------
 *  The init process, this is where it all begins...
 *------------------------------------------------------------------------
 */
initproc()				/* The beginning */
{
	kprintf( "\n\nCPSC 415, Jan 2012\n32 Bit Xeros 1.1\nLocated at: %x to %x\n", &entry, &end );

        /* Your code goes here */

	kprintf("Max addr is %d %x\n", maxaddr, maxaddr);

	kmeminit();
	kprintf("memory inited\n");
	
	dispatchinit();
	kprintf("dispatcher inited\n");
  
	contextinit();
	kprintf("context inited inited\n");
	
	create( idleproc, PROC_STACK );
	
	//create( root, PROC_STACK );
	kprintf("create inited\n");

	//Tests
	//Note: execute the tests one at a time to get the expected result
	//create( &test_1_and_2, PROC_STACK);
	//create( &test3, PROC_STACK);
	//create( &test4a, PROC_STACK);
	//create( &test4b, PROC_STACK);
  create (&test5,PROC_STACK);
	dispatch();
  
	kprintf("Returned to init, you should never get here!\n");
        /* This code should never be reached after you are done */
	for(;;); /* loop forever */
}
