Kay Men Yap OS Assignment Readme

Date created: 4/5/2019

Date last modified: 5/5/2019

Purpose: To make a multithreaded program that has mutual exclusion and uses pthread_mutex and pthread_cond function to achieve mutual exclusion
         and thread blocking and unblocking when needed

Files in project:
scheduler.c
scheduler.h
linked_list.h
linked_list.c
task_info.h
read_info.h
makefile

Test Files:
task_file
smaller_file

Functionality: It reads the task from task file and sleep for the burst size stated in the file per task

How to run: Type the command "make" to compile and then "./scheduler task_file m" where m is the size of the ready queue to run the program

Note: task_file is a file that contains 20 tasks and smaller_file is a file that contains 4 tasks
