/************************************************************
 *Name: Kay Men Yap
 *File name: scheduler.h
 *Date last modified: 4/5/2019
 ************************************************************/

/* Only contains forward declarations for scheduler.c */
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "linked_list.h"
#include "task_info.h"
void get_task_file_contents(LinkedList* tasks_from_file, FILE* task_file);
void* task(void* info);
void logTaskInQueue(TaskInfo* task_info);
void* cpu(void* number);
void logTaskTakenFromQueue(int cpu_id, TaskInfo* task_info, time_t service_time);
void logCompletedTask(int cpu_id, TaskInfo* task_info, time_t completion_time);
#endif
