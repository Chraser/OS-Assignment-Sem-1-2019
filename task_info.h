/************************************************************
 *Name: Kay Men Yap
 *File name: task_info.h
 *Date last modified: 4/5/2019
 ************************************************************/

/* contains typedef declartion of TaskInfo that contains task information */
#ifndef TASK_INFO_H
#define TASK_INFO_H

typedef struct{
    int task_num;
    int burst_size;
    time_t arrival_time;
} TaskInfo;
#endif
