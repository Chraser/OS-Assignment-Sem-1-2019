/************************************************************
 *Name: Kay Men Yap
 *File name: scheduler.c
 *Date last modified: 4/5/2019
 ************************************************************/

/*  This file contains code derived and heavily modified from
    https://youtu.be/lu6gGS9BJSY?t=112 accessed on 27/4/2019 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "scheduler.h"
#include "linked_list.h"
#include "read_info.h"
#include "task_info.h"
#define MAX_STRING_SIZE 11

FILE* simulation_log;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cpu_shared_vars_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty= PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full= PTHREAD_COND_INITIALIZER;
LinkedList* ready_queue;
int num_tasks = 0;
double total_waiting_time = 0.0;
double total_turnaround_time = 0.0;
int more_task_to_do = 1;

int main(int argc, char** argv)
{
    ReadInfo* read_info;
    FILE* task_file;
    LinkedList* tasks_from_file;
    pthread_t task_thread, cpu1, cpu2, cpu3;
    double average_waiting_time, average_turnaround_time;
    int one, two, three, queue_capacity;
    one = 1;
    two =  2;
    three = 3;

    if(argc != 3)
    {
        printf("Run program with ./scheduler task_file m\n");
    }
    else
    {
        task_file = fopen(argv[1], "r");
        if(task_file == NULL)
        {
            perror("Error reading from task file");
        }
        else
        {
            if(ferror(task_file))
            {
                perror("Error reading from taks file");
            }
            else
            {
                /* checks if third argument is a number */
                if(sscanf(argv[2],"%d", &queue_capacity) == 1)
                {
                    if((queue_capacity< 1 ) || (queue_capacity > 10))
                    {
                        printf("Invalid queue_capacity for Ready Queue. Set m to be between 1 and 10\n");
                    }
                    else
                    {
                        simulation_log = fopen("simulation_log", "w");
                        if(simulation_log == NULL)
                        {
                            perror("Error opening simulation_log file");
                        }
                        else
                        {
                            if(ferror(simulation_log))
                            {
                                perror("Error reading from simulation_log file");
                            }
                            else
                            {
                                tasks_from_file =  createLinkedList();
                                get_task_file_contents(tasks_from_file, task_file);
                                ready_queue = createLinkedList();
                                read_info = (ReadInfo*)malloc(sizeof(ReadInfo));
                                read_info->tasks_from_file = tasks_from_file;
                                read_info->queue_capacity = queue_capacity;

                                pthread_create(&task_thread, NULL, task, (void*)read_info);
                                pthread_create(&cpu1, NULL, cpu, (void*)(&one));
                                pthread_create(&cpu2, NULL, cpu, (void*)(&two));
                                pthread_create(&cpu3, NULL, cpu, (void*)(&three));

                                /* wait until all other threads are finish before continuing */
                                pthread_join(task_thread, NULL);
                                pthread_join(cpu1, NULL);
                                pthread_join(cpu2, NULL);
                                pthread_join(cpu3, NULL);

                                free(tasks_from_file);
                                free(ready_queue);
                                free(read_info);

                                average_waiting_time = total_waiting_time / (double)num_tasks;
                                average_turnaround_time = total_turnaround_time / (double)num_tasks;
                                fprintf(simulation_log,"Number of tasks: %d tasks\n", num_tasks);
                                fprintf(simulation_log,"Average waiting time: %d seconds\n", (int)average_waiting_time);
                                fprintf(simulation_log,"Average turn around time: %d seconds\n", (int)average_turnaround_time);
                            }
                        }
                        fclose(simulation_log);
                    }
                }
                else
                {
                    printf("Error: Third argument must be a number\n");
                }
            }
            fclose(task_file);
        }
    }
    return 0;
}

/* takes the content of the file and add the task info of each line to the linked list called
   tasks_from_file */
void get_task_file_contents(LinkedList* tasks_from_file, FILE* task_file)
{
    char line[MAX_STRING_SIZE];
    int task_num = 0;
    int burst_size = 0;
    TaskInfo* task_info;
    while(fgets(line, sizeof(line), task_file) != NULL)
    {
        /* check if task num and burst size are integers */
        if(sscanf(line, "task%d %d", &task_num, &burst_size) == 2)
        {
            /* check if burst size is in range of 1 to 50 inclusive */
            if(burst_size >= 1 && burst_size <= 50)
            {
                /* create new TaskInfo struct */
                task_info = (TaskInfo*)malloc(sizeof(TaskInfo));
                task_info->task_num = task_num;
                task_info->burst_size = burst_size;
                task_info->arrival_time = time(NULL);
                /* add task_info to tasks_from_file list */
                insertLast(tasks_from_file, task_info);
            }
        }
    }
}


void* task(void* info)
{
    ReadInfo* read_info = (ReadInfo*)info;
    LinkedList* tasks_from_file = read_info->tasks_from_file;
    int total_num_tasks = 0;
    TaskInfo* task_info;

    /* Code below derived and heavily modified from https://youtu.be/lu6gGS9BJSY?t=112 */

    /* continue looping while there's still tasks in the tasks_from_file list */
    while(tasks_from_file->head != NULL)
    {
        pthread_mutex_lock(&queue_mutex);

        /* check if queue is full before adding and block the thread while queue is full*/
        while(ready_queue->count == read_info->queue_capacity)
        {
            pthread_cond_wait(&not_full, &queue_mutex);
        }

        /* adds first task that was read to queue */
        task_info = removeFirst(tasks_from_file);
        task_info->arrival_time = time(NULL);
        insertLast(ready_queue, task_info);
        logTaskInQueue(task_info);
        pthread_cond_signal(&not_empty);
        total_num_tasks++;

        /* adds second task that was read to queue if possible else don't add a second task*/
        if(tasks_from_file != NULL && ready_queue->count < read_info->queue_capacity)
        {
            task_info = removeFirst(tasks_from_file);
            task_info->arrival_time = time(NULL);
            insertLast(ready_queue, task_info);
            logTaskInQueue(task_info);
            pthread_cond_signal(&not_empty);
            total_num_tasks++;
        }

        pthread_mutex_unlock(&queue_mutex);
    }
    /* End of code derived and heavily modified from https://youtu.be/lu6gGS9BJSY?t=112 */

    /* signal to cpu threads that there is no more task to put into queue */
    pthread_mutex_lock(&queue_mutex);
    more_task_to_do = 0;
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&queue_mutex);

    /* get current time for termination and change it to hours, minutes and seconds */
    int hour, minute, second;
    time_t current_time;
    current_time = time(NULL);
    struct tm* local = localtime(&current_time);
    hour = local->tm_hour;
    minute = local->tm_min;
    second = local->tm_sec;

    pthread_mutex_lock(&log_mutex);
    fprintf(simulation_log, "Number of tasks put into Ready-Queue: %d\n", total_num_tasks);
    fprintf(simulation_log, "Terminate at time: %d:%d:%d\n\n", hour, minute, second);
    pthread_mutex_unlock(&log_mutex);
    return NULL;
}

/* logs the task that is put into the queue to simulation_log */
void logTaskInQueue(TaskInfo* task_info)
{
    /* extracts the arrival time to hours, minutes, seconds */
    int hour, minute, second;
    struct tm* local = localtime(&(task_info->arrival_time));
    hour = local->tm_hour;
    minute = local->tm_min;
    second = local->tm_sec;

    /* acquire mutex lock for simuation_log then write to simuation_log */
    pthread_mutex_lock(&log_mutex);
    fprintf(simulation_log, "task%d: %d\n", task_info->task_num, task_info->burst_size);
    fprintf(simulation_log, "Arrival time: %d:%d:%d\n\n", hour, minute, second);
    pthread_mutex_unlock(&log_mutex);
}

void* cpu(void* number)
{
    int completed_tasks = 0;
    int cpu_id = *(int*)number;
    time_t service_time, completion_time;
    TaskInfo* task_info;
    /* Code derived and heavily modified from https://youtu.be/lu6gGS9BJSY?t=112 */
    pthread_mutex_lock(&queue_mutex);
    while(ready_queue->head != NULL || more_task_to_do == 1)
    {
        while(ready_queue->head == NULL && more_task_to_do == 1)
        {
            /* block if there is still more task to do and queue is empty else
               signal other cpu thread to chain signal the last cpu thread then
               proceed to terminating phase */
            if(ready_queue->head == NULL && more_task_to_do != 1)
            {
                pthread_cond_signal(&not_empty);
            }
            else
            {
                pthread_cond_wait(&not_empty, &queue_mutex);
            }
        }
        if(ready_queue->head != NULL || more_task_to_do == 1)
        {
            task_info = removeFirst(ready_queue);
            /* signal to task thread that queue is no longer full */
            pthread_cond_signal(&not_full);
            pthread_mutex_unlock(&queue_mutex);
            service_time = time(NULL);
            logTaskTakenFromQueue(cpu_id, task_info, service_time);

            /* acquire mutex lock for cpu shared variables and update num_tasks and total_waiting_time*/
            pthread_mutex_lock(&cpu_shared_vars_mutex);
            num_tasks++;
            total_waiting_time += difftime(service_time, task_info->arrival_time);
            pthread_mutex_unlock(&cpu_shared_vars_mutex);

            sleep(task_info->burst_size);

            completion_time = service_time + task_info->burst_size;
            logCompletedTask(cpu_id, task_info, completion_time);
            completed_tasks++;

            /* acquire mutex lock for cpu shared variables and update total_turnaround_time */
            pthread_mutex_lock(&cpu_shared_vars_mutex);
            total_turnaround_time += difftime(completion_time, task_info->arrival_time);
            pthread_mutex_unlock(&cpu_shared_vars_mutex);
            free(task_info);
            /* aquire mutex lock since loop using global vars */
            pthread_mutex_lock(&queue_mutex);
        }
    }
    pthread_mutex_unlock(&queue_mutex);
    /* End of code derived and heavily modified from https://youtu.be/lu6gGS9BJSY?t=112 */
    /* log the termination of CPU to simuation_log */
    pthread_mutex_lock(&log_mutex);
    fprintf(simulation_log, "CPU-%d terminates after servicing %d tasks\n\n", cpu_id, completed_tasks);
    pthread_mutex_unlock(&log_mutex);
    return NULL;
}

/* log the task taken from queue to simulation_log */
void logTaskTakenFromQueue(int cpu_id, TaskInfo* task_info, time_t service_time)
{
    /* extracts the arrival time and service time to hours, minutes, seconds */
    int arrival_hour, arrival_minute, arrival_second, service_hour, service_minute, service_second;
    struct tm* arrival_local = localtime(&(task_info->arrival_time));
    struct tm* service_local = localtime(&service_time);

    arrival_hour = arrival_local->tm_hour;
    arrival_minute = arrival_local->tm_min;
    arrival_second = arrival_local->tm_sec;

    service_hour = service_local->tm_hour;
    service_minute = service_local->tm_min;
    service_second = service_local->tm_sec;

    pthread_mutex_lock(&log_mutex);
    fprintf(simulation_log, "Statistics for CPU-%d:\n", cpu_id);
    fprintf(simulation_log, "Task %d\n", task_info->task_num);
    fprintf(simulation_log, "Arrival time: %d:%d:%d\n", arrival_hour, arrival_minute, arrival_second);
    fprintf(simulation_log, "Service time: %d:%d:%d\n\n", service_hour, service_minute, service_second);

    pthread_mutex_unlock(&log_mutex);
}

/* log the task completed by cpu thread to simuation_log */
void logCompletedTask(int cpu_id, TaskInfo* task_info, time_t completion_time)
{
    /* extract arrival_time and completion_time to hours, minutes, seconds */
    int arrival_hour, arrival_minute, arrival_second, completion_hour, completion_minute, completion_second;
    struct tm* arrival_local = localtime(&(task_info->arrival_time));
    struct tm* completion_local = localtime(&completion_time);
    arrival_hour = arrival_local->tm_hour;
    arrival_minute = arrival_local->tm_min;
    arrival_second = arrival_local->tm_sec;

    completion_hour = completion_local->tm_hour;
    completion_minute = completion_local->tm_min;
    completion_second = completion_local->tm_sec;

    pthread_mutex_lock(&log_mutex);
    fprintf(simulation_log, "Statistics for CPU-%d:\n", cpu_id);
    fprintf(simulation_log, "Task %d\n", task_info->task_num);
    fprintf(simulation_log, "Arrival time: %d:%d:%d\n", arrival_hour, arrival_minute, arrival_second);
    fprintf(simulation_log, "Completion time: %d:%d:%d\n\n", completion_hour, completion_minute, completion_second);

    pthread_mutex_unlock(&log_mutex);
}
