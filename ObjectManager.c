#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ObjectManager.h"

static uchar *buffer1;  //double buffer
static uchar *buffer2;  //double buffer

static uchar *bufferCurr; //point to the buffer in use

typedef struct NODE Node;

struct NODE{ //storing object data

    ulong numBytes;   //size of object
    ulong startAddr;  //starting address in the buffer
    Ref ref;    //id
    int count;  //times being called
    Node *next; //next node

};

static Node *head; //the top of the linked list

static ulong insertPtr; //point to the insert address

static Ref numOfBlocks; //number of blocks

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
    //return the reference to the object inserted

    //precondition
    validate();

    if(size < 0){
        //negative size
        printf("The size should be a positive integer");

        return NULL_REF;
    }
    
    if(size <= MEMORY_SIZE){
        if(insertPtr + size > MEMORY_SIZE){

            //invarient
            assert(insertPtr > 0);

            //run garbage collector
            compact();
        }
        
        if(insertPtr + size <= MEMORY_SIZE){
            //making the object Node
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->ref = numOfBlocks++;
            newNode->count = 1;
            newNode->numBytes = size;
            newNode->startAddr = insertPtr;
            newNode->next = NULL;
            insertPtr += size;
            
            if(head == NULL){
                //if the list is empty
                head = newNode;

                //postcondition
                validate();
                
                return newNode->ref;
            }else{
                //if the list isnt empty
                Node *curr = head;   //iterator
                Node *prev = NULL;   //iterator

                while(curr != NULL){
                    //loop to the last object
                    prev = curr;
                    curr = curr->next;
                }

                prev->next = newNode;

                //postcondition
                validate();
                
                return newNode->ref;
            }   
            
        }else{
            //no space available even compacted
            printf("No space available for inserting this object\n");
            printf("The space required: %lu\nThe space available: %lu\n", size, MEMORY_SIZE-insertPtr);
            
            //postcondition
            validate();

            return NULL_REF;
        }
    }else{
        printf("The size requested is way bigger than the size of the buffer\n");
        
        //postcondition
        validate();

        return NULL_REF;
    }
}//end of insertObject

void *retrieveObject( Ref ref ){
    //Retrieve the address of an object, identified by the reference id
    
    //precondition
    validate();

    if(head != NULL){
        //search for object using ref
        assert(head != NULL);
        
        Node *curr = head;  //iterator
        
        while(curr->ref != ref && curr->next != NULL){
            //finding the object
            curr = curr->next;
        }

        if(curr->ref == ref){
            //postcondition
            validate();

            return &bufferCurr[curr->startAddr];
        }else{
            printf("The object does not exist or been deleted\n");

            //postcondition
            validate();

            return NULL;
        }

    }else{
        //list empty case
        printf("The buffer is empty, nothing to retrieve.\n");

        //postcondition
        validate();

        return NULL;
    }
}//end of retrieveObject

void addReference( Ref ref ){
    //Increment the reference count for the object with reference id
    
    //precondition
    validate();
    if(ref > 0){
        if(head != NULL){

            assert(head != NULL);

            Node *curr = head;  //iterator

            while(curr->ref != ref && curr->next != NULL){
                //finding the object using reference
                curr = curr->next;
            }

            if(curr->ref == ref){
                //add reference
                curr->count++;
            }else{
                printf("The reference is not found, failed to add reference\n");
            }

        }else{
            //empty buffer case
            printf("The buffer is empty, failed to add reference\n");
        }
    }else{
        printf("NULL reference spotted, failed to add reference\n");
    }
    //postcondition
    validate();
}//end of addReference

void dropReference( Ref ref ){
    //Decrement the reference count for the object with reference id

    //precondition
    validate();
    if(ref > 0){
        if(head != NULL){

            assert(head != NULL);

            Node *curr = head;  //iterator

            while(curr->ref != ref && curr->next != NULL){
                //finding the object using reference
                curr = curr->next;
            }

            if(curr->ref == ref){
                //drop reference
                if(curr->count > 0){
                curr->count--;
                }
            }else{
                printf("The reference is not found, failed to drop reference\n");
            }

        }else{
            //empty buffer case
            printf("The buffer is empty, failed to drop reference\n");
        }
    }else{
        printf("NULL reference spotted, failed to drop reference\n");
    }
    //postcondition
    validate();
}//end of dropReference

void initPool(){
    //Initialize the object manager upon starting
    numOfBlocks = 1;
    head = NULL;
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