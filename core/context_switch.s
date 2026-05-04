.syntax unified
.cpu cortex-m3
.thumb

.extern p_current_tcb

.global PendSV_Handler
.type PendSV_Handler, %function

PendSV_Handler:
    /* Check if scheduler started */
    LDR     R0, =scheduler_started
    LDRB    R0, [R0]
    CBZ     R0, PendSV_Exit    /* if 0, skip everything */

    /* Save current task */
    MRS     R0, PSP
    STMDB   R0!, {R4-R11}

    LDR     R1, =p_current_tcb
    LDR     R1, [R1]
    STR     R0, [R1]

    PUSH    {LR}
    BL      scheduler_next_task
    POP     {LR}

    LDR     R1, =p_current_tcb
    LDR     R1, [R1]
    LDR     R0, [R1]

    LDMIA   R0!, {R4-R11}
    MSR     PSP, R0

PendSV_Exit:
    BX      LR

.size PendSV_Handler, .-PendSV_Handler
