/************************************************************
 *Name: Kay Men Yap
 *File name: linked_list.c
 *Date last modified: 4/5/2019
 ************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

/* Modified code obtained from UCP Assignment Semester 2 2018 */

/*
Purpose: To create an empty linked list and return it to the
         function call
*/
LinkedList* createLinkedList(void)
{
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->count = 0;
    return list;
}

/*
Purpose: To add a new node with nodeData at the end of the linked list
How it works: It checks for the next field of a linked list node that is NULL
              Make the next field of the node that has a NULL next field to
              point to the new node. If linked list is empty, make head point
              to new node
*/
void insertLast(LinkedList* list, TaskInfo* nodeData)
{
    LinkedListNode* currNode;
    LinkedListNode* newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
    newNode->task_info = nodeData;
    newNode->next = NULL;
    currNode = list->head;
    if(currNode != NULL)
    {
        while(currNode->next != NULL)
        {
            currNode = currNode->next;
        }
        currNode->next = newNode;
    }
    else
    {
        list->head = newNode;
    }
    list->count = list->count + 1;
}

/*
Purpose: To remove the first node in the linked list and return the data of that node
How it works: Returns the nodeData of the first node or NULL if the linked list is
              empty. If the list is not empty, a node variable points to the first node and
              the nodeData is updated with the data of the node to be removed and the
              head of list is updated to point to the next point and finally the node
              to be removed is freed
*/
TaskInfo* removeFirst(LinkedList* list)
{
    TaskInfo* nodeData;
    LinkedListNode* node;
    if(list->head == NULL)
    {
        nodeData = NULL;
    }
    else
    {
        node = list->head;
        nodeData = node->task_info;
        list->head = list->head->next;
        list->count--;
        free(node);
        node = NULL;
    }
    return nodeData;
}

/* End of modified code obtained from UCP Assignment */
