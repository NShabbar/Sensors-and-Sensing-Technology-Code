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

//#define ACCEL_data
//#define MAG_data
#define Gyr_10s_data

#define NOPS_FOR_20_MS 12500


//void NOP() {
//    int i;
//    for (i = 0; i < NOPS_FOR_20_MS; i++) {
//        asm("nop");
//    }
//}

int main(int argc, char** argv) {
    BOARD_Init();

    printf("Board");

    for (int zc = 0; zc < 10000; zc++) {
        asm("nop");
    }
    printf("Board2");
    BNO055_Init();
    printf("BNO055");
    for (int zc = 0; zc < 10000; zc++) {
        asm("nop");
    }
    int count = 0;

    while (1) {
#ifdef ACCEL_data
        int accel_x = BNO055_ReadAccelX();
        int accel_y = BNO055_ReadAccelY();
        int accel_z = BNO055_ReadAccelZ();

        printf("%d, %d, %d \n", accel_x, accel_y, accel_z);
#endif 

#ifdef MAG_data
        int mag_x = BNO055_ReadMagX();
        int mag_y = BNO055_ReadMagY();
        int mag_z = BNO055_ReadMagZ();

        printf("%d, %d, %d \n", mag_x, mag_y, mag_z);
#endif

#ifdef Gyr_10s_data
        int gyr_x = BNO055_ReadGyroX();
        int gyr_y = BNO055_ReadGyroY();
        int gyr_z = BNO055_ReadGyroZ();


        printf("%d, %d, %d \n", gyr_x, gyr_y, gyr_z);

#endif   
//        NOP();

                if (count < NOPS_FOR_20_MS) {
                    asm("nop");
                    count++;
                } else {
                    count = 0;
                }

    }

    return (EXIT_SUCCESS);
}

