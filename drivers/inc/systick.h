#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>


void systick_init(uint32_t sysclk_hz);
uint32_t systick_get_tick(void);

#endif
