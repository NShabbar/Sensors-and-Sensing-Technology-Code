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
#define CapTouch PORTDbits.RD11 // Pin 35

int touch_period;
int prev_touch_period;
int curr_timer_val;

int move_average_readings[TOUCH_THRESHOLD] = {0};
int i = 0; // current index
int sum = 0; // sum of all readings
int new_average = 0;
int average = 0;
int new_reading = 0;
int is_touched = 0;

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

    TRISDbits.TRISD11 = 1; // input needs to be high for input
    return SUCCESS;
}

void __ISR(_INPUT_CAPTURE_4_VECTOR) InputCapture_Handler(void) {
    IFS0bits.IC4IF = 0;
    // IC4BUF contains the timer value when the rising edge occurred.
    curr_timer_val = IC4BUF;
    touch_period = curr_timer_val - prev_touch_period;
    if (touch_period < 0) {
        touch_period += 65536; // roll over large gaps in captouch sensor.
    }
    prev_touch_period = curr_timer_val;
}

/**
 * @function    CAPTOUCH_IsTouched(void)
 * @brief       Returns TRUE if finger is detected. Averaging of previous measurements 
 *              may occur within this function, however you are NOT allowed to do any I/O
 *              inside this function.
 * @return      TRUE or FALSE
 */
char CAPTOUCH_IsTouched(void) {
    return is_touched;
}

/*
 * 
 */
int main(int argc, char** argv) {
    BOARD_Init();
    SERIAL_Init();
    TIMERS_Init();
    CAPTOUCH_Init();

    while (1) {
        new_reading = touch_period;

        sum = sum - move_average_readings[i] + new_reading; // subtract oldest reading and add new one
        move_average_readings[i] = new_reading; // store the new reading

        average = sum / TOUCH_THRESHOLD; // calculate the moving average

        i = (i + 1) % TOUCH_THRESHOLD; // this will increment and wrap index

        if (is_touched) {
            if (average < 6000) {
                printf("I am not being touched.\n");
                is_touched = 0;
            }

        } else {
            if (average > 10000) {
                printf("I am being touched.\n");
                is_touched = 1;
            }
        }
    }
    return (EXIT_SUCCESS);
}

