/* 
 * File:   main.c
 * Author: Nadia
 *
 * Created on January 18, 2024, 4:06 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "AD.h"
#include "BOARD.h"
#include "Buttons.h"
#include "serial.h"
#include "ToneGeneration.h"

#define sample_size 150
uint8_t button_avg_flag = FALSE;
unsigned int frequency = DEFAULT_TONE;

int main(int argc, char** argv) {
    BOARD_Init();
    AD_Init();
    ToneGeneration_Init();
    AD_AddPins(AD_A1); // turning on pin for flex sensor

    BOARD_End();
    unsigned int flex_val = 0;
    unsigned int move_average_readings[sample_size] = {0}; // array to store samples of frequency
    unsigned int index = 0; // current index
    unsigned int sum = 0; // sum of all readings
    unsigned int new_average = 0;
    unsigned int average =0;


    while (1) {
//        uint8_t bEvent = ButtonsCheckEvents();
//        switch (bEvent) {
//            case BUTTON_EVENT_1DOWN:
//                button_avg_flag = TRUE;
//                frequency = TONE_196;
//                ToneGeneration_SetFrequency(frequency);
//                ToneGeneration_ToneOn();
//                break;
//            case BUTTON_EVENT_2DOWN:
//                button_avg_flag = TRUE;
//                frequency = TONE_293;
//                ToneGeneration_SetFrequency(frequency);
//                ToneGeneration_ToneOn();
//                break;
//            case BUTTON_EVENT_3DOWN:
//                button_avg_flag = TRUE;
//                frequency = TONE_440;
//                ToneGeneration_SetFrequency(frequency);
//                ToneGeneration_ToneOn();
//                break;
//            case BUTTON_EVENT_4DOWN:
//                button_avg_flag = TRUE;
//                frequency = TONE_500;
//                ToneGeneration_SetFrequency(frequency);
//                ToneGeneration_ToneOn();
//                break;
//            case BUTTON_EVENT_1UP:
//            case BUTTON_EVENT_2UP:
//            case BUTTON_EVENT_3UP:
//            case BUTTON_EVENT_4UP:
//                ToneGeneration_ToneOff();
//                button_avg_flag = FALSE;
//                break;
//        }
//
//        if (button_avg_flag) {
//            unsigned int new_flex_val = AD_ReadADPin(AD_A0);
//            sum = sum - move_average_readings[index] + new_flex_val; // subtract oldest reading and add new one
//            move_average_readings[index] = new_flex_val; // store the new reading
//            average = sum / sample_size;
//          
//            index = (index + 1) % sample_size;
//            if (abs(new_average - average) >= 5) {
//                new_average = average;
//                ToneGeneration_SetFrequency((average/5) + frequency);
//                ToneGeneration_ToneOn();
//            }
//        }
                unsigned int new_flex_val = AD_ReadADPin(AD_A1); // Pin for Flex sensor
                if (flex_val != new_flex_val) {
                    flex_val = new_flex_val;
//                    printf("%d\n", flex_val);
                }
                sum = sum - move_average_readings[index] + new_flex_val; // subtract oldest reading and add new one
                move_average_readings[index] = new_flex_val; // store the new reading
        
                unsigned int average = sum / sample_size; // calculate the moving average
                printf("%d\n", average);
        
                index = (index + 1) % sample_size; // this will increment and wrap index
        //        

        //        ToneGeneration_SetFrequency(average); // This was used to test the speaker with flex sensor, filter, and moving average
        //        ToneGeneration_ToneOn();

        //        ToneGeneration_SetFrequency(flex_val); // This was used to test the speaker with flex sensor
        //        ToneGeneration_ToneOn();
        //        ToneGeneration_ToneOff();

    }
}