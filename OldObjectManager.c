#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ObjectManager.h"

static uchar *buffer1;  //double buffer
static uchar *buffer2;  //double buffer

static void *bufferCurr; //point to the buffer in use

typedef struct NODE Node;

struct NODE{ //storing object data

    int numBytes;   //size of object
    int startAddr;  //starting address in the buffer
    Ref ref;    //id
    int count;  //times being called
    Node *next; //next node

};

static Node *head; //the top of the linked list

static int insertPtr; //point to the insert address

static int numOfBlocks; //number of blocks in the list

static Ref nextRef; //next reference id to be given

static ulong bytesReleased;

static ulong bytesInuse;

static void validate(){ 
    //printf("validate start\n"); //delete
    //invarient
    #ifndef NDEBUG
    
    //bytes is in th size
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
    //printf("validate end\n"); //delete
}

static void compact(){
    printf("compact start\n"); //delete
    //Initiate garbage collection
    //precondition
    validate();

    uchar *currBuffer = (uchar *)bufferCurr;
    uchar *nextBuffer;
    int newInsertPtr = 0;
    Node *curr = head;  //iterator
    assert(curr == head);
    if(bufferCurr == buffer1){
        nextBuffer = buffer2;
    }else{
        nextBuffer = buffer1;
    }

    while(curr != NULL){
        //printf("%lu\n",curr->ref); //delete
        assert(curr->count > 0);
        memcpy(nextBuffer + newInsertPtr, currBuffer + curr->startAddr, curr->numBytes);
        //nextBuffer[newInsertPtr] = currBuffer[curr->startAddr];
        //for(int i = 0; i<curr->numBytes; i++){
            //nextBuffer[newInsertPtr + i] = currBuffer[curr->startAddr + i];
        //}
        curr->startAddr = newInsertPtr;
        newInsertPtr += curr->numBytes;
        curr = curr->next;
    }
    memset(bufferCurr, MEMORY_SIZE, '0');
    bufferCurr = nextBuffer;
    insertPtr = newInsertPtr;

    printf("\nGarbage collector statistics:\n");
    printf("objects: %d   bytes in use: %lu   freed: %lu\n\n", numOfBlocks, bytesInuse, bytesReleased);

    bytesReleased = 0;
    //postcondition
    validate();
    printf("compact end\n"); //delete
}

////Interface////

Ref insertObject( ulong size ){
    printf("insertObject\n"); //delete
    //Request a block of memory of given size from the object manager
    //return the reference to the object inserted
    printf("**%lu\n",size);
    //precondition
    validate();

    Node *newNode = (Node *)malloc(sizeof(Node));

    if(size <= 0){
        //negative size
        printf("The size should be a positive integer");
        free(newNode);
        return NULL_REF;
    }
    
    if(newNode != NULL && size <= MEMORY_SIZE){
        if(bytesInuse + size > MEMORY_SIZE){

            //invarient
            assert(insertPtr > 0);

            //run garbage collector
            compact();
        }

        Node *curr = head;   //iterator
        while(curr != NULL && curr->next != NULL){
            assert(curr != NULL);
            //loop to the last object
            curr = curr->next;
        }

        if(numOfBlocks > 0){
            assert(curr != NULL);
        }

        if(bytesInuse + size <= MEMORY_SIZE){
            //making the object Node
            newNode->ref = nextRef++;
            newNode->count = 1;
            newNode->numBytes = size;
            newNode->startAddr = insertPtr;
            newNode->next = NULL; //delete
            numOfBlocks++;
            insertPtr += size;
            bytesInuse += size;
            
            if(curr == NULL){
                //if the list is empty
                head = newNode;
                
                //postcondition
                assert(head != NULL);
                assert(head->next == NULL);
                assert(head->count == 1);
                assert(head->numBytes > 0);
                assert(head->ref == nextRef-1);
                assert(head->startAddr >= 0);
                validate();
                
                return head->ref;
            }else{
                //if the list isnt empty
                assert(curr != NULL);
                assert(curr->next == NULL);
                
                curr->next = newNode;
                
                //postcondition
                assert(curr->next != NULL);
                assert(curr->next->next == NULL);
                assert(curr->next->count == 1);
                assert(curr->next->numBytes > 0);
                assert(curr->next->ref == nextRef-1);
                assert(curr->next->startAddr >= 0);
                validate();
                
                return curr->next->ref;
            }   
            
        }else{
            //no space available even compacted
            printf("Unable to successfully complete memory allocation request.\n");
            free(newNode);
            //postcondition
            validate();

            return NULL_REF;
        }
    }else{
        printf("Invalid malloc request with size %lu\n",size);
        free(newNode);
        //postcondition
        validate();

        return NULL_REF;
    }
}//end of insertObject

void *retrieveObject( Ref ref ){
    printf("retrieveObjecr\n"); //delete
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
            //uchar *currBuffer = (uchar *)bufferCurr; 
            //return &bufferCurr[curr->startAddr];
            return (uchar *)((uchar *)bufferCurr + curr->startAddr);

        }else{
            printf("Invalid reference exception with reference %lu, terminating process.\n", ref);

            //postcondition
            validate();

            return NULL;
        }

    }else{
        //list empty case
        printf("Invalid reference exception with reference %lu in empty list, terminating process.\n", ref);

        //postcondition
        validate();

        return NULL;
    }
}//end of retrieveObject

void addReference( Ref ref ){
    printf("addReference\n"); //delete
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
            printf("The buffer is empty, failed to add reference\n");   //delete
        }
    }else{
        printf("NULL reference spotted, failed to add reference\n");    //delete
    }
    //postcondition
    validate();
}//end of addReference

void dropReference( Ref ref ){
    printf("dropReference\n"); //delete
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
                    //printf("*272\n"); //delete
                    free(curr);
                }
            }else{
                printf("The reference is not found, failed to drop reference\n");
            }

        }else{
            //empty buffer case
            printf("The buffer is empty, failed to drop reference\n"); //delete
        }
    }else{
        printf("NULL reference spotted, failed to drop reference\n"); //delete
    }
    //postcondition
    validate();
}//end of dropReference

void initPool(){
    printf("initPool\n"); //delete
    //Initialize the object manager upon starting
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
}

void destroyPool(){
    printf("destroyPool\n"); //delete
    //Clean up the object manager upon quitting

    //precondition
    validate();

    Node *curr = head; //iterator
    
    while(head != NULL){
        //clean up every object node
        head = head->next;
        //printf("*316\n"); //delete
        //printf("%d, %d\n",curr->startAddr,curr->numBytes); //delete
        free(curr);
        curr = head;
    }

    numOfBlocks = 0;
    nextRef = 1;
    head = NULL;
    insertPtr = 0;
    bytesReleased = 0;
    bytesInuse = 0;
    bufferCurr = NULL;
    //printf("**325\n"); //delete
    free(buffer1);
    //printf("**326\n"); //delete
    free(buffer2);
}

void dumpPool(){
    printf("dumpPool\n"); //delete
    //Print (to stdout) info about each object that is currently allocated including its id, start address, and size
    //precondition
    validate();

    Node *curr = head; //iterator
    printf("Objects info\n");

    while(curr != NULL){
        printf("ID: %lu StartAddress: %d Size: %d Count: %d\n",curr->ref,curr->startAddr,curr->numBytes,curr->count);
        curr = curr->next;
    }

    //postcondition
    validate();
}