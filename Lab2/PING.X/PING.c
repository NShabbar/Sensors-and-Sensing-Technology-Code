/* 
 * File:   PING.c
 * Author: Nadia
 *
 * Created on January 30, 2024, 3:44 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <PING.h>
#include <BOARD.h>
#include <serial.h>
#include <timers.h>
#include <pwm.h>
#include <ToneGeneration.h>


#define trigger LATDbits.LATD3 // Pin 9
#define echo PORTDbits.RD5 // Pin 34
#define sample_size 15

static volatile unsigned int time_of_flight;
static volatile unsigned int start_time;
int trigger_count = 0;

unsigned int frequency = DEFAULT_TONE;
unsigned int move_average_readings[sample_size] = {0}; // array to store samples of frequency
unsigned int index = 0; // current index
unsigned int sum = 0; // sum of all readings
unsigned int new_average = 0;
unsigned int average = 0;
unsigned int new_reading = 0;

int map(int moving_avg, int ping_min, int ping_max, int out_min, int out_max) {
    int scaled_value = (moving_avg - ping_min) * (out_max - out_min) / (ping_max - ping_min) + out_min;

    // makes sure the result does not exceed the specified range
    if (scaled_value > out_max) {
        scaled_value = out_max;
    }// makes sure the result does not go below the specified range
    else if (scaled_value < out_min) {
        scaled_value = out_min;
    }
    return scaled_value;
}

char PING_Init(void) {
    // following block inits the timer
    T4CON = 0;
    T4CONbits.TCKPS = 0b110; // 6 in binary
    PR4 = 6; // this is not the timer value wanted (changed to 3750)
    T4CONbits.ON = 1;
    IFS0bits.T4IF = 0;
    IPC4bits.T4IP = 3;
    IEC0bits.T4IE = 1;

    // following block inits change notify
    CNCONbits.ON = 1; // Change Notify On
    CNENbits.CNEN14 = 1;
    int temp = PORTD; // this is intentional to ensure a interrupt occur immediately upon enabling
    IFS1bits.CNIF = 0; // clear interrupt flag
    IPC6bits.CNIP = 1; //set priority
    IPC6bits.CNIS = 3; // and sub priority
    IEC1bits.CNIE = 1; // enable change notify
    //Anything else that needs to occur goes here

    //TRIS sets data direction flow.
    TRISDbits.TRISD5 = 1; // echo, input needs to be high for input
    TRISDbits.TRISD3 = 0; // trigger, output needs to be low for output
    //LAT is effectively the same as writing to the register.
    LATDbits.LATD3 = 1; // for data corresponding to data

    return SUCCESS;
}

void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    IFS1bits.CNIF = 0;
    //Anything else that needs to occur goes here

    if (echo) {
        start_time = TIMERS_GetMicroSeconds();
    }

    if (!echo) {
        unsigned int end_time = TIMERS_GetMicroSeconds();
        time_of_flight = end_time - start_time;
        
        printf("Output of Moving Average: %d\n", average);
        printf("Time of Flight in cm: %d\n", time_of_flight / 58);
        printf("Distance: %d\n\n", PING_GetDistance());
    }
}

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;
    //Anything else that needs to occur goes here
    trigger_count++;
    if (trigger_count > 6000) { // 6000 for 60ms, reset after.
        trigger_count = 0;
        trigger = 1; // set trigger pin high
    } else {
        trigger = 0;
    }
}

/**
 * @function    PING_GetDistance(void)
 * @brief       Returns the calculated distance in mm using the sensor model determined
 *              experimentally. 
 *              No I/O should be done in this function
 * @return      distance in mm
 */
unsigned int PING_GetDistance(void) {
    // 34000 comes from the speed of sound. 340 meters/seconds * 1000 mm/ meter
    // 1000000 comes from 1 mil microseconds in a second.
    unsigned int distance = (unsigned int) ((0.5 * 34000 * time_of_flight) / 1000000);
    return distance;
}

/**
 * @function    PING_GetTimeofFlight(void)
 * @brief       Returns the raw microsecond duration of the echo from the sensor.
 *              NO I/O should be done in this function.
 * @return      time of flight in uSec
 */
unsigned int PING_GetTimeofFlight(void) {
    return time_of_flight;
}

int main(int argc, char** argv) {
    BOARD_Init();
    TIMERS_Init();
    ToneGeneration_Init();
    PING_Init();

    while (1) {
        new_reading = PING_GetDistance();

        sum = sum - move_average_readings[index] + new_reading; // subtract oldest reading and add new one
        move_average_readings[index] = new_reading; // store the new reading

        average = sum / sample_size; // calculate the moving average

        index = (index + 1) % sample_size; // this will increment and wrap index

        int hertz = map(average, 5, 350, 100, 4000);
        ToneGeneration_SetFrequency(hertz);
    }
    return (EXIT_SUCCESS);
}

