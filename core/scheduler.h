#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "tcb.h"
#include "systick.h"
#include "stm32f103xb.h"

#define MAX_TASKS 8
extern volatile uint8_t scheduler_started;
extern uint8_t current_task;
extern TCB_t   task_table[MAX_TASKS];
extern TCB_t *p_current_tcb;
// declarations
// function prototypes
// structs, enums, macros
void scheduler_init(void);
void scheduler_add_task(const char *name, void(*fn)(void), uint32_t period);
void scheduler_run(void);

void task_stack_init(TCB_t *task);
void scheduler_next_task(void);
#endif // MY_HEADER_H