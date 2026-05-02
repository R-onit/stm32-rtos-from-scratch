#include "task_led.h"
#include "stm32f103xb.h"



void task_led_init(void){

    // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PA5 as output push-pull 2MHz
    // CRL controls pins 0-7
    // each pin = 4 bits: MODE[1:0] CNF[1:0]
    // MODE=10 (output 2MHz), CNF=00 (push-pull)
    GPIOA->CRL &= ~(0xF << 20);   // clear bits for PA5
    GPIOA->CRL |=  (0x2 << 20);   // MODE=10, CNF=00

}   // enable clock, configure PA5 as output

void task_led(void){
    GPIOA->ODR ^= (1 << 5);   // toggle PA5
}        // toggle PA5 — this is the task function