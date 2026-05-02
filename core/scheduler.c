/*
    So now i am closer to the cyclic scheduler

    what would be happening in the internal logics??
    1. This is like we are giving 
*/  
#include "scheduler.h"
#include <string.h>


TCB_t task_table[MAX_TASKS];
static uint8_t task_count = 0;


void scheduler_init(void){

    //Zeroes out the task_counter
    task_count = 0;
}

void scheduler_add_task(const char *name, void(*fn)(void),uint32_t period){
    
    strcpy(task_table[task_count].name,name);
    task_table[task_count].task_fn=fn;
    task_table[task_count].period=period;

    task_table[task_count].last_run=0;
    task_table[task_count].state=TASK_READY;
    task_count++;
}

void scheduler_run(void){
    while(1){

        uint32_t now=systick_get_tick();
        for(int i=0; i<task_count; i++){

            if(task_table[i].state == TASK_READY){

                if((now - task_table[i].last_run) >= task_table[i].period){

                    task_table[i].last_run=now;
                    task_table[i].task_fn();
                }
            }
        }

    }
}
