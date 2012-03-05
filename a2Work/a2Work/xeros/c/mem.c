/* mem.c : memory manager
 */

#include <kernel.h>
#include <i386.h>

/* Your code goes here */
/*Why do you need a paragraph mask?*/
#define PARAGRAPH_MASK  (~(0xf))

extern long     freemem;

typedef struct struct_mem mem;
struct struct_mem {
    mem         *next;
    mem         *prev;
    int          size;
    mem  *sanityCheck;
};

static mem      *head;

 void kmeminit( void ) {
/****************************/

     long       s;

     s = ( freemem + 0x10 ) & PARAGRAPH_MASK;

     head = (mem *)s;
     head->size = HOLESTART - s;
     head->prev = NULL;
     head->sanityCheck = head;

     s = HOLEEND;

     head->next = (mem *)s;
     head->next->next = NULL;
     head->next->prev = head;
     head->next->size = (1024 * 1024 * 4) - HOLEEND;
     head->sanityCheck = head;
}



void kfree(void * memp) {
     
     mem *toFree = (mem *) memp;
	if(toFree->sanityCheck == toFree) {

		toFree->prev = NULL;
		toFree->next = head;
		head->prev = toFree;
		head = toFree;

    	} 
        else {
		kprintf("//----------------------------\n");
		kprintf("//Invalid sanity check in kfree\n");
		kprintf("//----------------------------\n");
    	}

}


 void *kmalloc( int size ) {
/********************************/

    mem         *p;
    mem         *r;

    if( size & 0xf ) {
        size = ( size + 0x10 ) & PARAGRAPH_MASK;
    }

    for( p = head; p && ( p->size < size ); p = p->next );

    if( !p ) {
        return( 0 );
    }

    if( ( p->size - size ) < sizeof( mem ) ) {
       if( p->next ) {
            p->next->prev = p->prev;
        }

        if( p->prev ) {
            p->prev->next = p->next;
        } else {
            head = p->next;
        }
    } else {
        r = (mem *) ( (int)p + size );

        *r = *p;

	//This line is key for the sanity check or mem free
	r->sanityCheck = r;

        if( p->next ) {
            p->next->prev = r;
        }

        if( p->prev ) {
            p->prev->next = r;
        } else {
            head = r;
        }
    }

    return( p );
}









