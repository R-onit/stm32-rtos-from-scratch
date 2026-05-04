/*
    So now i am closer to the cyclic scheduler

    what would be happening in the internal logics??
    1. This is like we are giving 
*/  
#include "scheduler.h"
#include <string.h>


TCB_t task_table[MAX_TASKS];
uint8_t task_count = 0;
volatile uint8_t scheduler_started = 0;
TCB_t *p_current_tcb;   // assembly will use this directly
uint8_t current_task = 0;


// add this global — visible in debugger or map file
volatile uint32_t tcb_size = sizeof(TCB_t);
volatile uint32_t stack_offset = offsetof(TCB_t, stack);
void scheduler_init(void){

    //Zeroes out the task_counter
    task_count = 0;

    // current_task = task_count - 1;  // so first next_task() call goes to 0

    // PendSV must be lowest priority — always
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
}

void scheduler_add_task(const char *name, void(*fn)(void),uint32_t period){
    
    strcpy(task_table[task_count].name,name);
    task_table[task_count].task_fn=fn;
    task_table[task_count].period=period;

    task_table[task_count].last_run=0;
    task_table[task_count].state=TASK_READY;

    task_stack_init(&task_table[task_count]);

    task_count++;
}

void scheduler_run(void){
    // while(1){

    //     uint32_t now=systick_get_tick();
    //     for(int i=0; i<task_count; i++){

    //         if(task_table[i].state == TASK_READY){

    //             if((now - task_table[i].last_run) >= task_table[i].period){

    //                 task_table[i].last_run=now;
    //                 task_table[i].task_fn();
    //             }
    //         }
    //     }

    // }
    current_task  = 0;
    p_current_tcb = &task_table[0];

    scheduler_started = 1;

    // set PSP to first task's saved stack pointer
    __set_PSP((uint32_t)task_table[0].stack_ptr);
    __set_CONTROL(__get_CONTROL() | 0x02);
    __ISB();

    // jump into first task directly
    task_table[0].task_fn();

    // never reaches here
    while(1) {}
    // current_task = task_count - 1;
    // p_current_tcb = &task_table[current_task];

    // // Set PSP to first task's saved stack pointer
    // __set_PSP((uint32_t)task_table[0].stack_ptr);
    
    // // Switch to PSP
    // __set_CONTROL(__get_CONTROL() | 0x02);
    // __ISB();
    
    // // Trigger first context switch via PendSV
    // // This loads task 0 context and jumps into it properly
    // SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    
    // // Should never reach here
    // while(1) {}
}

void task_stack_init(TCB_t *task) {

    uint32_t *sp = &task->stack[STACK_SIZE - 1];

    // hardware saved frame (bottom to top order):
    *sp-- = 0x01000000;              // xPSR - thumb bit must be set
    *sp-- = (uint32_t)task->task_fn; // PC   - where task starts
    *sp-- = 0xFFFFFFFD;              // LR   - return to thread mode, PSP
    *sp-- = 0;                       // R12
    *sp-- = 0;                       // R3
    *sp-- = 0;                       // R2
    *sp-- = 0;                       // R1
    *sp-- = 0;                       // R0

    // manually saved registers (R11 down to R4):
    *sp-- = 0;                       // R11
    *sp-- = 0;                       // R10
    *sp-- = 0;                       // R9
    *sp-- = 0;                       // R8
    *sp-- = 0;                       // R7
    *sp-- = 0;                       // R6
    *sp-- = 0;                       // R5
    *sp-- = 0;                       // R4

    // sp now points to R4 — this is where we save stack_ptr
    task->stack_ptr = sp + 1;
}

void scheduler_next_task(void) {
    current_task = (current_task + 1) % task_count;
    p_current_tcb = &task_table[current_task];
}