//---------------------------------
//NAME: Man Chun Ng
//STUDENT NUMBER: 7924340
//COURSE: COMP2160, SECTION: A01
//INSTRUCTOR: Dr. Mehdi Niknam
//ASSIGNMENT: assignment #4, Question #1
//
//REMARKS: test cases for ObjectManager.c
//---------------------------------
#include <stdio.h>
#include <string.h>

#include "ObjectManager.h"

void testInitPool();
void testInsertObject(ulong size, Ref *ptr);
void testFailInsertObject(ulong size, Ref *ptr);
void testRetrieveObject(Ref ref, char *ptr);
void testFailRetrieveObject(Ref ref, char *ptr);
void testAddReference(Ref ref);
void testDropReference(Ref ref);
void testDestroyPool();
void testDumpPool();

//counter
static int testsExecuted = 0;
static int testsFailed = 0;

int main (void){
    //test cases
    char *ptr;
    Ref id1, id2, id3, id4, id5;
    testInitPool();

    printf("\nTesting typical cases:\n");

    printf("\nTesting failed cases:\n");

    printf("\nTesting edge cases:\n");


    testDestroyPool();
    //print the number
    printf("\n\nTotal number of tests executed: %d\n", testsExecuted);
    printf("Number of tests passed:         %d\n",(testsExecuted - testsFailed));
    printf("Number of tests failed:         %d\n", testsFailed);
    printf("Program completed normally.\n");
    return 0;
}

void testInitPool(){
    //test method for initPool
    printf("Initializing ObjectManager.\n");
    testsExecuted++;
}//end of testInitPool

void testInsertObject(ulong size, Ref *ptr){
    //test method for InsertObject
    printf("Expect SUCCESS: Inserting object size of %lu\n",size);
    testsExecuted++;
    ptr = insertObject(size);
    if(ptr == NULL_REF){
        printf("FAILED: expected to insert size of %lu, and cannot insert\n", size);
        testsFailed++;
    }else{
        printf("SUCCESS: expected to insert size of %lu, and inserted\n", size);
    }
}//end of testInsertObject

void testFailInsertObject(ulong size, Ref *ptr){
    //test method for fail InsertObject case
    printf("Expect FAIL: Inserting object size of %lu\n",size);
    testsExecuted++;
    ptr = insertObject(size);
    if(ptr == NULL_REF){
        printf("SUCCESS: expected cannot insert size of %lu, and cannot insert\n", size);
    }else{
        printf("FAILED: expected cannot insert size of %lu, and inserted\n", size);
        testsFailed++;
    }
}//end of testFailInsertObject

void testRetrieveObject(Ref ref, char *ptr){
    //test method for RetrieveObject
    testsExecuted++;
}//end of testRetrieveObject

void testFailRetrieveObject(Ref ref, char *ptr){
    //test method for fail RetrieveObject case
    testsExecuted++;
}//end of testFailRetrieveObject

void testAddReference(Ref ref){
    //test method for AddReference
    testsExecuted++;
}//end of testAddReference

void testDropReference(Ref ref){
    //test method for DropReference
    testsExecuted++;
}//end of testDropReference

void testDestroyPool(){
    //test method for DestroyPool
    printf("Destroying ObjectManager.\n");
    testsExecuted++;
}//end of testDestroyPool

void testDumpPool(){
    //test method for DumpPool
    printf("Printing out object information.\n");
    testsExecuted++;
}//end of testDumpPool