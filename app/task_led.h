#ifndef TASK_LED_H_
#define TASK_LED_H_



void task_led_init(void);   // enable clock, configure PA5 as output
void task_led(void);        // toggle PA5 — this is the task function

#endif



