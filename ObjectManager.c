#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ObjectManager.h"

static uchar *buffer1;  //double buffer
static uchar *buffer2;  //double buffer

static uchar *bufferCurr = NULL; //point to the buffer in use

typedef struct NODE{ //storing object data

    int numBytes;   //size of object
    int startAddr;  //starting address in the buffer
    int ref;    //id
    int count;  //times being called
    Node *next; //next node

}Node;

static Node *root = NULL; //the top of the linked list

static int insertPtr = 0; //point to the insert address

static int numOfBlocks = 0; //number of blocks

static void validate(){
    //invarient
    #ifndef NDEBUG

    #endif
}

static void compact(){
    //Initiate garbage collection

}

////Interface////

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
    numOfBlocks = 1;
    root = NULL;
    insertPtr = 0;
    buffer1 = (uchar *)malloc(sizeof(MEMORY_SIZE));
    buffer2 = (uchar *)malloc(sizeof(MEMORY_SIZE));

    bufferCurr = buffer1;
    printf("\nSize of the buffer:  %d\n", MEMORY_SIZE);

    //postcondition
    validate();
}

void destroyPool(){
    //Clean up the object manager upon quitting
}

void dumpPool(){
    //Print (to stdout) info about each object that is currently allocated including its id, start address, and size
}