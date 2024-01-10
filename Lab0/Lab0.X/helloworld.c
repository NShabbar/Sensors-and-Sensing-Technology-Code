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

int main(int argc, char** argv) {
    BOARD_Init();
    AD_Init();
    AD_AddPins(AD_A0);
    printf("Hello world!\n");
    BOARD_End();
    unsigned int pot_val = 0;

    while (1) {
        unsigned int new_pot_val = AD_ReadADPin(AD_A0);
        if (pot_val != new_pot_val) {
            pot_val = new_pot_val;
            printf("%d\n", pot_val);
        }
    }
}

