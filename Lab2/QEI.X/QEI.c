/* 
 * File:   QEI.c
 * Author: Nadia
 *
 * Created on January 29, 2024, 1:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <QEI.h>
#include <pwm.h>

static volatile int A = 0;
static volatile int B = 0;

int encoder_count = 0;
int current_state = 0;
int color = 0;

typedef enum{
    state_1 = 0,
    state_2 = 1,
    state_3 = 2,
    state_4 = 3    
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
                
                TRISDSET = 0xC0; // turning on pin 6 and 7
  
                // the rest of the function goes here
                return SUCCESS;
}
 
void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
            readPort = PORTD; // this read is required to make the interrupt work
            IFS1bits.CNIF = 0;
            
            A = PORTDbits.RD6;
            B = PORTDbits.RD7;
            
            printf("Encoder Count value: %d\n", encoder_count);
            printf("A: %d\n", A);
            printf("B: %d\n", B);
            printf("color: %d\n", color);
            
            if (encoder_count == 24 || encoder_count == -24){
                QEI_ResetPosition();
            }
            
               //anything else that needs to happen goes here
            switch(current_state){
                case state_1:
                    if ((A == 1) && (B == 0)){
                        current_state = state_2;
                        encoder_count--;
                    }else if ((A == 0) && (B == 1)){
                        current_state = state_4;
                        encoder_count++;
                    }
                    break;
                case state_2:
                    if ((A == 0) && (B == 0)){
                        current_state = state_3;
                    }else if ((A == 1) && (B == 1)){
                        current_state = state_1;
                    }
                    break;
                case state_3:
                    if ((A == 0) && (B == 1)){
                        current_state = state_4;
                    }else if ((A == 1) && (B == 0)){
                        current_state = state_2;
                    }
                    break;
                case state_4:
                    if ((A == 1) && (B == 1)){
                        current_state = state_1;
                    }else if ((A == 0) && (B == 0)){
                        current_state = state_3;
                    }
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

int encoder_to_degrees(int value){
    int degrees = value*15;
    return degrees;
}

int main(int argc, char** argv) {
    BOARD_Init();
    QEI_Init();
    PWM_Init();
    PWM_AddPins(PWM_PORTZ06);
    PWM_AddPins(PWM_PORTY12);
    PWM_AddPins(PWM_PORTY10);
    PWM_SetFrequency(PWM_1KHZ);
    
    int red = PWM_PORTZ06;
    int green = PWM_PORTY12;
    int blue = PWM_PORTY10;
    
   
    while(1){
        if (encoder_count >= 0){
            color = (encoder_count % 4) * 250;
        }
        if (encoder_count < 0){
            color = 1000 - ((encoder_count % 4) * 250);
        }
        if ((encoder_count >= 0 && encoder_count <= 3) || (encoder_count >= -23 && encoder_count <= -20)){ // Yellow range
            PWM_SetDutyCycle(red, 0);
            PWM_SetDutyCycle(green, color);
            PWM_SetDutyCycle(blue, 1000);
        }
        if ((encoder_count >= 4 && encoder_count <= 7) || (encoder_count >= -19 && encoder_count <= -16)){ // Red range
            PWM_SetDutyCycle(red, 0);
            PWM_SetDutyCycle(green, 1000);
            PWM_SetDutyCycle(blue, 1000 - color);
        }
        if ((encoder_count >= 8 && encoder_count <= 11) || (encoder_count >= -15 && encoder_count <= -12)){ // Magenta range
            PWM_SetDutyCycle(red, color);
            PWM_SetDutyCycle(green, 1000);
            PWM_SetDutyCycle(blue, 0);
        }
        if ((encoder_count >= 12 && encoder_count <= 15) || (encoder_count >= -11 && encoder_count <= -8)){ // Blue range
            PWM_SetDutyCycle(red, 1000);
            PWM_SetDutyCycle(green, 1000 - color);
            PWM_SetDutyCycle(blue, 0);
        }
        if ((encoder_count >= 16 && encoder_count <= 19) || (encoder_count >= -7 && encoder_count <= -4)){ // Teal range
            PWM_SetDutyCycle(red, 1000);
            PWM_SetDutyCycle(green, 0);
            PWM_SetDutyCycle(blue, color);
        }
        if ((encoder_count >= 20 && encoder_count <= 23) || (encoder_count >= -3 && encoder_count < 0)){ // Green range
            PWM_SetDutyCycle(red, 1000 - color);
            PWM_SetDutyCycle(green, 0);
            PWM_SetDutyCycle(blue, 1000);
        }
    }
    return (EXIT_SUCCESS);
}

