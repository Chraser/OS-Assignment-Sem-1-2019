/************************************************************
 *Name: Kay Men Yap
 *File name: read_info.h
 *Date last modified: 4/5/2019
 ************************************************************/

/* contains typedef declartion of ReadInfo that contains the list of tasks from file and
   size of ready queue*/
#ifndef READ_INFO_H
#define READ_INFO_H

typedef struct{
    LinkedList* tasks_from_file;
    int queue_capacity;
} ReadInfo;
#endif
