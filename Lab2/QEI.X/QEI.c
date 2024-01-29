/* 
 * File:   QEI.c
 * Author: Nadia
 *
 * Created on January 29, 2024, 1:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <QEI.h>

#define Pin_36 PORTDbits.pin36
#define Pin_37 PORTDbits.pin37

unsigned int encoder_count = 0;
unsigned int current_state = 0;

typedef enum{
    state_00 = 0,
    state_01 = 1,
    state_10 = 2,
    state_11 = 3    
} encoder_states;

char QEI_Init(void) {
               // INIT Change notify
                CNCONbits.ON = 1; // Change Notify On
                CNENbits.CNEN15 = 1; //enable one phase
                CNENbits.CNEN16 = 1; //enable other phase
                int temp = PORTD; // this is intentional to ensure a interrupt occur immediately upon enabling
                IFS1bits.CNIF = 0; // clear interrupt flag
                IPC6bits.CNIP = 1; //set priority
                IPC6bits.CNIS = 3; // and sub priority
                IEC1bits.CNIE = 1; // enable change notify
  
                // the rest of the function goes here
                return SUCCESS;
}
 
void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
            readPort = PORTD; // this read is required to make the interrupt work
            IFS1bits.CNIF = 0;
               //anything else that needs to happen goes here
            switch(current_state){
                case state_00:
                    if (readPort & (Pin_36&&Pin_37)){
                        current_state = state_11;
                    }else if (readPort & (!Pin_36 & Pin_37)){
                        current_state = state_01;
                        encoder_count++;
                    }
                    break;
                case state_01:
                    if (readPort & (!Pin_36&&Pin_37)){
                        current_state = state_01;
                    }
                    break;
                case state_10:
                    if (readPort & (1 <<15)){
                        current_state = state_10;
                    }else if (readPort & (1 << 16)){
                        current_state = state_00;
                    }
                    break;
                case state_11:
                    break;
            }
}

/**
 * @function QEI_GetPosition(void) 
 * @param none
 * @brief This function returns the current count of the Quadrature Encoder in ticks.      
*/
int QEI_GetPosition(void){
    return encoder_count;
}

/**
 * @Function QEI_ResetPosition(void) 
 * @param  none
 * @return none
 * @brief  Resets the encoder such that it starts counting from 0.
*/
void QEI_ResetPosition(){
    encoder_count = 0;
} 


//int main(int argc, char** argv) {
//
//    return (EXIT_SUCCESS);
//}

