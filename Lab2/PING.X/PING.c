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




static volatile unsigned int time_of_flight;
static volatile unsigned int start_time;
static volatile unsigned int echo = 0;
static volatile unsigned int trigger = 0;
int current_state = 0;

typedef enum{
    start = 0,
    waiting = 1   
} ping_states;


char PING_Init(void) {
    // following block inits the timer
    T4CON = 0;
    T4CONbits.TCKPS = 0b110;
    PR4 = 3750; // this is not the timer value wanted (changed o 3750)
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
    return SUCCESS;
}

void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    IFS1bits.CNIF = 0;
    //Anything else that needs to occur goes here
    echo = PORTDbits.RD4;
    printf("Pinging");
    switch(current_state){
        case start:
            if (echo){
                start_time = TIMERS_GetMicroSeconds();
                current_state = waiting;
            }
            break;
        case waiting:
            if (!echo){
                unsigned int end_time = TIMERS_GetMicroSeconds();
                time_of_flight = end_time - start_time;
                current_state = start;
            }
            break;
    }
}

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;
            
    trigger = 1;
    while (TMR4 < 400);
    trigger = 0;
    
    //Anything else that needs to occur goes here
}


/**
 * @function    PING_GetDistance(void)
 * @brief       Returns the calculated distance in mm using the sensor model determined
 *              experimentally. 
 *              No I/O should be done in this function
 * @return      distance in mm
 */
unsigned int PING_GetDistance(void){
    // 34000 comes from the speed of sound. 340 meters/seconds * 1000 mm/ meter
    // 1000000 comes from 1 mil microseconds in a second.
    unsigned int distance = (unsigned int)((0.5 * 34000 * time_of_flight) / 1000000);
    return distance;
}

/**
 * @function    PING_GetTimeofFlight(void)
 * @brief       Returns the raw microsecond duration of the echo from the sensor.
 *              NO I/O should be done in this function.
 * @return      time of flight in uSec
 */
unsigned int PING_GetTimeofFlight(void){
    return time_of_flight;
}


int main(int argc, char** argv) {
    BOARD_Init();
    TIMERS_Init();
    ToneGeneration_Init();
    PWM_Init();
    
    PWM_AddPins(PWM_PORTY04);
    while(1){
        if (trigger){
            printf("trigger\n");
        }
    }
    return (EXIT_SUCCESS);
}

