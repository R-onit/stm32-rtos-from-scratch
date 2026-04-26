#ifndef TCB_H
#define TCB_H

#include <stdint.h>

//Task state enum
typedef enum{
    TASK_READY,
    TASK_SUSPEND
}TaskState_t ;


/*
TCB:
Every task in an RTOS is represented by a struct in memory. This struct is called a TCB — Task Control Block. The scheduler keeps an array of these. When it wants to run a task, it looks at the TCB. When a task blocks, it updates the TCB. Everything flows through this struct.

A function pointer — the actual task function
A name — for debugging
A period — how often it should run in milliseconds
A last_run tick — when it last executed
A state — READY or SUSPENDED for now

*/
typedef struct{
    void (*task_fn)(void);
    char name[16];
    uint32_t period;
    uint32_t last_run;
    TaskState_t state;

}TCB_t;

#endif