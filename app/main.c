#include <stdint.h>
#include "stm32f103xb.h"
#include "systick.h"

#define SYSCLK_HZ   64000000UL


void clock_init(void) {
    // 1. Enable HSI (already on by default, but be explicit)
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    // 2. Configure PLL: HSI/2 * 16 = 64MHz
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;        // HSI/2 as PLL source
    RCC->CFGR |=  RCC_CFGR_PLLMULL16;     // x16

    // 3. Enable PLL and wait
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // 4. Set flash latency for 64MHz (2 wait states)
    FLASH->ACR = FLASH_ACR_LATENCY_2 | FLASH_ACR_PRFTBE;

    // 5. Switch SYSCLK to PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void gpio_init(void) {
    // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PA5 as output push-pull 2MHz
    // CRL controls pins 0-7
    // each pin = 4 bits: MODE[1:0] CNF[1:0]
    // MODE=10 (output 2MHz), CNF=00 (push-pull)
    GPIOA->CRL &= ~(0xF << 20);   // clear bits for PA5
    GPIOA->CRL |=  (0x2 << 20);   // MODE=10, CNF=00
}


int main(void) {
    clock_init();
    // systick_init(64000000) will go here next
        gpio_init();

    systick_init(SYSCLK_HZ);

    uint32_t last = 0;


    while (1) {
        if ((systick_get_tick() - last) >= 1000) {
            last = systick_get_tick();
            GPIOA->ODR ^= (1 << 5);   // toggle PA5
        }
    }
    return 0;
}