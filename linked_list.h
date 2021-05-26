/************************************************************
 *Name: Kay Men Yap
 *File name: linked_list.h
 *Date last modified: 4/5/2019
 ************************************************************/

/* Modified code obtained from UCP Assignment Semester 2 2018*/

/*Forward declarations only. Comments for each individual function is at the definition of function*/
#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <time.h>
#include "task_info.h"
/*
Purpose: To hold the reference to the next node of the current node and
         the data of the current node
*/
typedef struct LinkedListNode{
    TaskInfo* task_info;
    struct LinkedListNode* next;
} LinkedListNode;

/*
Purpose: To hold reference to the starting node of a linked list and the
         number of nodes in the linked list
*/
typedef struct LinkedList{
    LinkedListNode* head;
    int count;
} LinkedList;

LinkedList* createLinkedList(void);
void insertLast(LinkedList* list, TaskInfo* data);
TaskInfo* removeFirst(LinkedList* list);
#endif

/* End of code obtained from UCP Assignment */
