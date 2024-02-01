/* 
 * File:   CAPTOUCH.c
 * Author: Nadia
 *
 * Created on January 31, 2024, 8:00 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <CAPTOUCH.h>
#include <BOARD.h>
#include <serial.h>
#include <timers.h>

#define TOUCH_THRESHOLD 1000

static volatile unsigned int touchPeriod;
static volatile unsigned int prevTouchPeriod;


char CAPTOUCH_Init(void) {
    // following block inits the timer
    T2CON = 0;
    T2CONbits.TCKPS = 0b011;
    PR2 = 0xFFFF;
    T2CONbits.ON = 1;

    //this block inits input capture
    IC4CON = 0;
    IC4CONbits.ICTMR = 1;
    IC4CONbits.ICM = 0b010;

    IFS0bits.IC4IF = 0;
    IPC4bits.IC4IP = 7;
    IEC0bits.IC4IE = 1;
    IC4CONbits.ON = 1;
    // whatever else you need to do to initialize your module
    return SUCCESS;
}

void __ISR(_INPUT_CAPTURE_4_VECTOR) InputCapture_Handler(void) {
    IFS0bits.IC4IF = 0;
    // IC4BUF contains the timer value when the rising edge occurred.
    unsigned int currentTimerValue = IC4BUF;
    touchPeriod = currentTimerValue - prevTouchPeriod;
    prevTouchPeriod = currentTimerValue;
}


/**
 * @function    CAPTOUCH_Init(void)
 * @brief       This function initializes the module for use. Initialization is 
 *              done by opening and configuring timer 2, opening and configuring the input capture
 *              peripheral, and setting up the interrupt.
 * @return      SUCCESS or ERROR (as defined in BOARD.h)
 */
char CAPTOUCH_Init(void){
    unsigned int averagedTouchPeriod = (touchPeriod + prevTouchPeriod) / 2;
    return (averagedTouchPeriod > TOUCH_THRESHOLD) ? TRUE : FALSE;
}

/**
 * @function    CAPTOUCH_IsTouched(void)
 * @brief       Returns TRUE if finger is detected. Averaging of previous measurements 
 *              may occur within this function, however you are NOT allowed to do any I/O
 *              inside this function.
 * @return      TRUE or FALSE
 */
char CAPTOUCH_IsTouched(void);

/*
 * 
 */
int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}

