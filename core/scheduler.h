#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "tcb.h"
#include "systick.h"

#define MAX_TASKS 8

// declarations
// function prototypes
// structs, enums, macros
void scheduler_init(void);
void scheduler_add_task(const char *name, void(*fn)(void), uint32_t period);
void scheduler_run(void);

#endif // MY_HEADER_H