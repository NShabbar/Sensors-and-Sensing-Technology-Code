/* 
 * File:   helloworld.c
 * Author: Nadia
 *
 * Created on January 9, 2024, 3:57 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "serial.h"
#include "AD.h"
#include "ToneGeneration.h"
#include "Buttons.h"
#define sample_size 300

int main(int argc, char** argv) {
    BOARD_Init();
    AD_Init();
    ToneGeneration_Init();
    AD_AddPins(AD_A0);
    printf("Hello world!\n");
    BOARD_End();
    unsigned int pot_val = 0;
    unsigned int move_average_readings[sample_size] = {0}; // array to store samples of frequency
    unsigned int index = 0; // current index
    unsigned int sum = 0; // sum of all readings


    while (1) {
        unsigned int new_pot_val = AD_ReadADPin(AD_A0);
        if (pot_val != new_pot_val) {
            pot_val = new_pot_val;
            printf("%d\n", pot_val);
        }
        sum = sum - move_average_readings[index] + new_pot_val; // subtract oldest reading and add new one
        move_average_readings[index] = new_pot_val; // store the new reading

        unsigned int average = sum / sample_size; // calculate the moving average

        index = (index + 1) % sample_size; // this will increment and wrap index

        uint8_t bEvent = ButtonsCheckEvents();
        if (bEvent & BUTTON_EVENT_1DOWN) {
            ToneGeneration_SetFrequency(average + TONE_196);
            ToneGeneration_ToneOn();
        } else if (bEvent & BUTTON_EVENT_2DOWN) {
            ToneGeneration_SetFrequency(average + TONE_293);
            ToneGeneration_ToneOn();
        } else if (bEvent & BUTTON_EVENT_3DOWN) {
            ToneGeneration_SetFrequency(average + TONE_440);
            ToneGeneration_ToneOn();
        } else if (bEvent & BUTTON_EVENT_4DOWN) {
            ToneGeneration_SetFrequency(average + TONE_659);
            ToneGeneration_ToneOn();
        } else if (BUTTON_EVENT_1UP | BUTTON_EVENT_2UP | BUTTON_EVENT_3UP | BUTTON_EVENT_4UP) {
            ToneGeneration_ToneOff();

        }

        //        ToneGeneration_SetFrequency(average); // This was used to test the speaker with pot, filter, and moving average
        //        ToneGeneration_ToneOn();

        //        ToneGeneration_SetFrequency(pot_val); // This was used to test the speaker with pot
        //        ToneGeneration_ToneOn();
        //        ToneGeneration_ToneOff();

    }
}

