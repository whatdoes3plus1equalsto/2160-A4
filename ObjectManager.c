#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ObjectManager.h"

static unsigned char *buffer1;  //double buffer
static unsigned char *buffer2;  //double buffer

static unsigned char *bufferCurr; //the buffer in use

typedef struct NODE{
    //storing object data
    int numBytes;   //size
    int startAddr;  //starting address in the buffer
    int ref;    //id
    int count;  //times being called
    Node *next; //next node
}Node;

static Node *root = NULL; //the top of the linked list

static int freePtr; //point to the insert address

static int numOfBlocks; //number of blocks

static void validate(){
    //invarient
    #ifndef NDEBUG

    #endif
}

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