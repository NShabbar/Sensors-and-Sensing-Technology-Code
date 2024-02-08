/* 
 * File:   Lab3Main.c
 * Author: Nadia
 *
 * Created on February 7, 2024, 7:37 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <BNO055.h>
#include <BOARD.h>
#include <I2C.h>
#include <serial.h>



int main(int argc, char** argv) {
    BOARD_Init();
    BNO055_Init();
    
    int accel_x = BNO055_ReadAccelX();
    int accel_y = BNO055_ReadAccelY();
    int accel_z = BNO055_ReadAccelZ();
    
    while(1){
        printf("Begin Calibration\n");
        printf("(X, Y, Z): (%d, %d, %d)", accel_x, accel_y, accel_z);
    }
    
    return (EXIT_SUCCESS);
}

