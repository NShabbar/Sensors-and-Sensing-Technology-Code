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
#include <math.h>

#define sample_size 150

unsigned int frequency = DEFAULT_TONE;

unsigned int flex_frequency(unsigned int AD_val){
    unsigned int freq_output = 0.843*AD_val + 528;
    return freq_output;
}

int main(int argc, char** argv) {
    BOARD_Init();
    AD_Init();
    ToneGeneration_Init();
    AD_AddPins(AD_A1); // turning on pin for flex sensor

    BOARD_End();
    
    unsigned int move_average_readings[sample_size] = {0}; // array to store samples of frequency
    unsigned int index = 0; // current index
    unsigned int sum = 0; // sum of all readings
    unsigned int new_average = 0;
    unsigned int average = 0;


    while (1) {
                unsigned int flex_val = AD_ReadADPin(AD_A1);
                unsigned int new_flex_val = flex_frequency(flex_val); // Pin for Flex sensor

                
                sum = sum - move_average_readings[index] + new_flex_val; // subtract oldest reading and add new one
                move_average_readings[index] = new_flex_val; // store the new reading
        
                average = sum / sample_size; // calculate the moving average
                
        
                index = (index + 1) % sample_size; // this will increment and wrap index
        //        
                if (abs(new_average - average) >= 5) {
                    new_average = average;
                    printf("%d\n", new_average);
                    ToneGeneration_SetFrequency(((average-500)/2) + frequency);
                    ToneGeneration_ToneOn();
                }

    }
}