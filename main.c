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
Ref testInsertObject(ulong size);
Ref testFailInsertObject(ulong size);
void *testRetrieveObject(Ref ref);
void *testFailRetrieveObject(Ref ref);
void testAddReference(Ref ref);
void testDropReference(Ref ref);
void testDestroyPool();
void testDumpPool();

//counter
int testsExecuted = 0;
int testsFailed = 0;
int objectCounter = 0;


int main (int argc, char *argv[]){
    //test cases
    char *ptr;
    Ref id1, id2, id3, id4, id5;
    testInitPool();
    printf("\n  Testing typical cases:\n");
    printf("Objects exists: %d\n",objectCounter);
    id1 = testInsertObject(100);
    id2 = testInsertObject(300);
    ptr = (char*)retrieveObject(id1);
    for (int i = 0; i < 100; i++){
        ptr[i] = (char)(i%26 + 'A');
    }
    for (int i = 0; i < 100; i++){
        fprintf(stdout,"%c",ptr[i]);
    }
    printf("\n");
    id3 = testInsertObject(100000);
    ptr = (char*)retrieveObject(id2);
    for (int i = 0; i < 200; i++){
        ptr[i] = (char)(i%26 + 'Z');
    }
    for (int i = 0; i < 200; i++){
        fprintf(stdout,"%c",ptr[i]);
    }
    printf("\n");
    id4 = testInsertObject(400000);
    id5 = testInsertObject(23888);
    testDropReference(id4);
    objectCounter--;
    testDropReference(id5);
    objectCounter--;
    testDropReference(id3);
    objectCounter--;
    testDropReference(id2);
    objectCounter--;
    id2 = testInsertObject(100);
    testAddReference(id2);
    testDropReference(id2);
    testDropReference(id2);
    objectCounter--;
    testDropReference(id1);
    objectCounter--;
    printf("Objects exists: %d\n",objectCounter);
    
    printf("\n  Testing failed cases:\n");
    id1 = testFailInsertObject(MEMORY_SIZE+1);
    

    printf("\n  Testing edge cases:\n");


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
    initPool();
    testsExecuted++;
}//end of testInitPool

Ref testInsertObject(ulong size){
    //test method for InsertObject
    printf("EXPECT SUCCESS: Inserting object size of %lu\n",size);
    testsExecuted++;
    Ref ptr;
    ptr = insertObject(size);
    if(ptr == NULL_REF){
        printf("FAILED: expected to insert size of %lu, and cannot insert\n", size);
        testsFailed++;
        return ptr;
    }else{
        printf("SUCCESS: expected to insert size of %lu, and inserted\n", size);
        objectCounter++;
        return ptr;
    }
}//end of testInsertObject

Ref testFailInsertObject(ulong size){
    //test method for fail InsertObject case
    printf("EXPECT FAIL: Inserting object size of %lu\n",size);
    testsExecuted++;
    Ref ptr;
    ptr = insertObject(size);
    if(ptr == NULL_REF){
        printf("SUCCESS: expected cannot insert size of %lu, and cannot insert\n", size);
        return ptr;
    }else{
        printf("FAILED: expected cannot insert size of %lu, and inserted\n", size);
        testsFailed++;
        objectCounter++;
        return ptr;
    }
}//end of testFailInsertObject

void *testRetrieveObject(Ref ref){
    //test method for RetrieveObject
    printf("EXPECT SUCCESS: retrieving object from reference of %lu\n",ref);
    testsExecuted++;
    char *ptr;
    ptr = (char *)retrieveObject(ref);
    if(ptr == (char *)NULL){
        printf("FAILED: expected to retrieve the object address of %lu, and cannot retrieve\n", ref);
        testsFailed++;
        return NULL;
    }else{
        printf("SUCCESS: expected to retrieve the object address of %lu, and retrieved\n", ref);
        return retrieveObject(ref);
    }
}//end of testRetrieveObject

void *testFailRetrieveObject(Ref ref){
    //test method for fail RetrieveObject case
    printf("EXPECT FAIL: retrieving object from reference of %lu\n",ref);
    testsExecuted++;
    char *ptr;
    ptr = (char *)retrieveObject(ref);
    if(ptr == (char *)NULL){
        printf("SUCCESS: expected cannot retrieve the object address of %lu, and cannot retrieve\n", ref);
        return NULL;
    }else{
        printf("FAILED: expected cannot retrieve the object address of %lu, and retrieved\n", ref);
        testsFailed++;
        return retrieveObject(ref);
    }
}//end of testFailRetrieveObject

void testAddReference(Ref ref){
    //test method for AddReference
    printf("TESTING: Adding reference of %lu\n",ref);
    addReference(ref);
    testsExecuted++;
}//end of testAddReference

void testDropReference(Ref ref){
    //test method for DropReference
    printf("TESTING: Dropping reference of %lu\n",ref);
    dropReference(ref);
    testsExecuted++;
}//end of testDropReference

void testDestroyPool(){
    //test method for DestroyPool
    printf("TESTING: Destroying ObjectManager.\n");
    destroyPool();
    testsExecuted++;
}//end of testDestroyPool

void testDumpPool(){
    //test method for DumpPool
    printf("TESTING: Printing out object information.\n");
    dumpPool();
    testsExecuted++;
}//end of testDumpPool
