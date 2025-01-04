#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

/*method to create a new node*/
ListNode* createNode(void) {

    /*allcoate a memory space for each new node*/
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));

    /*is the memory could not be allocated*/
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
    }

    /*assign all position values to 0*/
    newNode->data.posX = 0;
    newNode->data.posY = 0;
    newNode->data.sosX = 0;
    newNode->data.sosY = 0;

    /*assign the previous node and next node to null*/
    newNode->Prev = NULL;
    newNode->Next = NULL;

    return newNode;
}

/*method to free all allocated memory*/
void freeMallocs(LinkedList* list) {

    ListNode* currentNode = list->head; /*assign current node to head node*/
    ListNode* nextNode;

    /*runs until it reaches the end of the list*/
    while (currentNode != NULL) {
        nextNode = currentNode->Next; /*get the next node of the current node and assign it to nextNode*/
        free(currentNode); /*free the current node*/        
        currentNode = nextNode; /*make current node the next node*/      
    }
}

/*method to insert new nodes*/
void insertNode(LinkedList* list,int NposX, int NposY, int NsosX, int NsosY) {

    ListNode* newNode = createNode(); /*create a new node using createNode()*/

    /*assign the passed in position values to the nodes position values*/
    newNode->data.posX = NposX;
    newNode->data.posY = NposY;

    newNode->data.sosX = NsosX;
    newNode->data.sosY = NsosY;

    /*if the list is empty make the head and the tail the new node*/
    if(list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }

    /*else go to the end of the list*/
    else {
        ListNode* tempNode = list->head;

        while(tempNode->Next != NULL) {
            tempNode = tempNode->Next;
        }

        
        tempNode->Next = newNode;/*make the current tail node the previous of the new node*/
        newNode->Prev = tempNode;/*make the next in the current tail node the new node*/
        list->tail = newNode; /*make the tail the new node*/
    }
} 

/*get the previous nodes data(postions)*/
positions getPrev(LinkedList* list) {

    if(list->tail->Prev != NULL) {
        list->tail = list->tail->Prev;
        list->tail->Next = NULL;

        return list->tail->data;
    }
}
