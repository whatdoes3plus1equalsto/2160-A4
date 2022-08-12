//---------------------------------
//NAME: Man Chun Ng
//STUDENT NUMBER: 7924340
//COURSE: COMP2160, SECTION: A01
//INSTRUCTOR: Dr. Mehdi Niknam
//ASSIGNMENT: assignment #4, Question #1
//
//REMARKS: An impletation of Garbage Collection
//---------------------------------
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

static int numOfBlocks; //number of blocks in the list

static Ref nextRef; //next reference id to be given

static ulong bytesReleased; //released memory

static ulong bytesInuse;    //number of memory using

static void validate(){ 
    //invarient
    #ifndef NDEBUG
    
    //bytes is in the size
    assert(bytesInuse <= MEMORY_SIZE);
    assert(bytesInuse >= 0);
    //0 is reserved for NULL_REF
    assert(nextRef > 0);
    //check null
    if(numOfBlocks == 0){
        assert(head == NULL);
    }else{
        int counter = 1;
        Node *curr = head;
        
        assert(curr->numBytes <= MEMORY_SIZE);
        
        while(curr->next != NULL){
            counter ++;
            //printf("%d\n",curr->numBytes); //delete
            assert(curr->numBytes <= MEMORY_SIZE);
            curr = curr->next;
        }
        
        assert(curr->next == NULL);
        //check number of blocks
        assert(numOfBlocks == counter);
    }
    #endif
}//end of validate

static void compact(){
    //Initiate garbage collection
    //exchange two buffers
    
    //precondition
    validate();

    uchar *nextBuffer;  //next buffer pointer
    ulong newInsertPtr = 0; //insert in next buffer
    Node *curr = head;  //iterator

    if(bufferCurr == buffer1){  //choosing which buffer to-use
        nextBuffer = buffer2;
    }else{
        nextBuffer = buffer1;
    }

    while(curr != NULL){
        //copying the memory to another buffer
        memcpy(nextBuffer + newInsertPtr, bufferCurr + curr->startAddr, curr->numBytes);
        curr->startAddr = newInsertPtr;
        newInsertPtr += curr->numBytes;
        curr = curr->next;
    }
    bufferCurr = nextBuffer;    //point to a new buffer
    insertPtr = newInsertPtr;   //new insert point

    printf("\nGarbage collector statistics:\n");
    printf("objects: %d   bytes in use: %lu   freed: %lu\n\n", numOfBlocks, bytesInuse, bytesReleased);

    bytesReleased = 0;//reset memory released

    //postcondition
    validate();
    assert(bufferCurr != NULL);
}//end of compact

////Interface////

Ref insertObject( ulong size ){
    //Request a block of memory of given size from the object manager
    //return the reference to the object inserted

    //precondition
    validate();

    if(size <= 0){
        //negative size
        printf("The size should be a positive integer");

        return NULL_REF;
    }
    
    if(size <= MEMORY_SIZE){
        //acceptable size
        if(insertPtr + size > MEMORY_SIZE){
            //rearrange buffer
            //invarient
            assert(insertPtr > 0);

            //run garbage collector
            compact();
        }
        
        if(insertPtr + size <= MEMORY_SIZE){
            //making the object Node
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->ref = nextRef++;
            newNode->count = 1;
            newNode->numBytes = size;
            newNode->startAddr = insertPtr;
            newNode->next = NULL;
            numOfBlocks++;
            insertPtr += size;
            bytesInuse += newNode->numBytes;
            
            if(head == NULL){
                //if the list is empty
                head = newNode;

                //postcondition
                validate();
                assert(head != NULL);
                assert(head->next == NULL);
                assert(head->count == 1);
                assert(head->numBytes > 0);
                assert(head->ref == nextRef-1);
                assert(head->startAddr >= 0);
                
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
                assert(prev->next != NULL);
                assert(prev->next->next == NULL);
                assert(prev->next->count == 1);
                assert(prev->next->numBytes > 0);
                assert(prev->next->ref == nextRef-1);
                assert(prev->next->startAddr >= 0);

                return newNode->ref;
            }   
            
        }else{
            //no space available even compacted
            printf("Unable to successfully complete memory allocation request.\n");
            
            //postcondition
            validate();

            return NULL_REF;
        }
    }else{
        printf("Unable to successfully complete memory allocation request.\n");
        
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
            printf("Invalid reference exception with reference %lu, terminating process.\n", ref);

            //postcondition
            validate();

            return NULL_REF;
        }

    }else{
        //list empty case
        printf("Invalid reference exception with reference %lu, terminating process.\n", ref);

        //postcondition
        validate();

        return NULL_REF;
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

                //postcondition
                assert(curr->count > 0);
            }else{
                printf("Invalid reference exception with reference %lu, failed to add reference.\n", ref);
            }

        }
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

            Node *prev = NULL;  //iterator
            Node *curr = head;  //iterator

            while(curr->ref != ref && curr->next != NULL){
                //finding the object using reference
                prev = curr;
                curr = curr->next;
            }

            if(curr->ref == ref){
                //drop reference
                if(curr->count > 0){
                curr->count--;
                }
                if(curr->count == 0){
                    //drop and free node if its count is dropped to 0
                    if(head == curr){
                        head = curr->next;
                    }else{
                        prev->next = curr->next;
                    }
                    numOfBlocks--;
                    bytesReleased += curr->numBytes;
                    bytesInuse -= curr->numBytes;
                    free(curr);
                }
            }else{
                printf("Invalid reference exception with reference %lu, failed to drop reference.\n", ref);
            }

        }
    }

    //postcondition
    validate();

}//end of dropReference

void initPool(){
    //Initialize the object manager upon starting
    //setting up every parameters
    numOfBlocks = 0;
    nextRef = 1;
    head = NULL;
    insertPtr = 0;
    bytesReleased = 0;
    bytesInuse = 0;
    buffer1 = (uchar *)malloc(MEMORY_SIZE);
    buffer2 = (uchar *)malloc(MEMORY_SIZE);
    bufferCurr = buffer1;

    //postcondition
    validate();
}//end of initPool

void destroyPool(){
    //Clean up the object manager upon quitting

    //precondition
    validate();

    Node *curr = head; //iterator

    while(curr != NULL){
        //clean up every object node
        head = head->next;
        free(curr);
        curr = head;
    }
    //reset every parameters
    numOfBlocks = 0;
    head = NULL;
    insertPtr = 0;
    bytesReleased = 0;
    bytesInuse = 0;
    //postcondition
    validate();

    bufferCurr = NULL;
    free(buffer1);
    free(buffer2);

    //postcondition
    assert(head == NULL);
}//end of destroyPool

void dumpPool(){
    //Print (to stdout) info about each object that is currently allocated including its id, start address, and size
    //precondition
    validate();

    Node *curr = head; //iterator
    printf("Objects info\n");

    while(curr != NULL){
        printf("ID: %lu StartAddress: %lu Size: %lu Count: %d\n",curr->ref,curr->startAddr,curr->numBytes,curr->count);
        curr = curr->next;
    }

    //postcondition
    validate();
}//end of dumpPool