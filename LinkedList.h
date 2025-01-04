#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct positions {
    int posX;
    int posY;
    int sosX;
    int sosY;
} positions;


typedef struct ListNode {
    positions data;
    struct ListNode* Prev;
    struct ListNode* Next;
} ListNode;

typedef struct {
    ListNode* head;
    ListNode* tail;
} LinkedList;   

ListNode* createNode(void);
void insertNode(LinkedList* list,int NposX, int NposY, int NsosX, int NsosY);
positions getPrev(LinkedList* list);
void freeMallocs(LinkedList* list);

#endif