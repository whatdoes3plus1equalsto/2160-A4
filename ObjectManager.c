#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ObjectManager.h"

unsigned char *buffer1;
unsigned char *buffer2;

unsigned char *bufferCurr;

Ref insertObject( ulong size ){
    //Request a block of memory of given size from the object manager
}

void *retrieveObject( Ref ref ){
    //Retrieve the address of an object, identified by the reference id
}

void addReference( Ref ref ){
    //Increment the reference count for the object with reference id
}

void dropReference( Ref ref ){
    //Decrement the reference count for the object with reference id
}

void initPool(){
    //Initialize the object manager upon starting
}

void destroyPool(){
    //Clean up the object manager upon quitting
}

void dumpPool(){
    //Print (to stdout) info about each object that is currently allocated including its id, start address, and size
}

static void compact(){
    //Initiate garbage collection
}