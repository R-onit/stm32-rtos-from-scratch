#include "systick.h"
#include "stm32f103xb.h"
#include <stdint.h>


// step 1: tick counter - you told me: static volatile
static volatile uint32_t tick_count = 0;

void systick_init(uint32_t sysclk_hz) {
    // step 2: program reload value - you told me this formula
    SysTick->LOAD = (sysclk_hz / 1000) - 1;; //(64,000,000 / 1000) - 1 =63999
    
    // step 3: clear current value - you told me CVR = 0
    SysTick->VAL  = 0;
    
    // step 4: program CSR - you told me enable + tickint + clksource=1
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk     |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_CLKSOURCE_Msk;
}

// step 5: ISR - you told me tick_count++
void SysTick_Handler(void) {
    tick_count++;
}

// step 6: getter - returns tick_count
uint32_t systick_get_tick(void) {
    return tick_count;
}