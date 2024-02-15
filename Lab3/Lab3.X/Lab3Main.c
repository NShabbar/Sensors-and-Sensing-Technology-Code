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
//#define Gyr_10s_data
//#define accel_test
//#define mag_test
#define gyro_test


#define accel_x0  -16.71
#define accel_y0 -94.60
#define accel_z0 -49.46
#define accel_a -891.75
#define accel_b  -1272.71
#define accel_c  -1049.44

#define mag_x0 588.96
#define mag_y0 349.44
#define mag_z0 -411.15
#define mag_a -296.01
#define mag_b  -533.91
#define mag_c -530.01

#define X_bias -16.20960699
#define Y_bias 28.33973081
#define Z_bias 13.71325617


#define NOPS_FOR_20_MS 12500
#define sample_size 150

int move_average_readings[sample_size] = {0}; // array to store samples of frequency
int index = 0; // current index
int sum = 0; // sum of all readings
int new_average = 0;
int average = 0;
int max_x = 0;
int min_x = 0;
int max_y = 0;
int min_y = 0;
int max_z = 0;
int min_z = 0;

float angle_x = 0;
float angle_x = 0;
float angle_x = 0;

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


        printf("%d,%d,%d\n", gyr_x, gyr_y, gyr_z);

#endif  

#ifdef accel_test
        int accel_x = BNO055_ReadAccelX();
        int accel_y = BNO055_ReadAccelY();
        int accel_z = BNO055_ReadAccelZ();

        float x_hat = (accel_x - accel_x0) / accel_a;
        float y_hat = (accel_y - accel_y0) / accel_b;
        float z_hat = (-1 * (accel_z - accel_z0) / accel_c);
        printf("%f, %f, %f\n", x_hat, y_hat, z_hat);

#endif

#ifdef mag_test
        int mag_x = BNO055_ReadMagX();
        int mag_y = BNO055_ReadMagY();
        int mag_z = BNO055_ReadMagZ();

        sum = sum - move_average_readings[index] + mag_x + mag_y + mag_z; // subtract oldest reading and add new one
        move_average_readings[index] = mag_x + mag_y + mag_z; // store the new reading

        average = sum / sample_size; // calculate the moving average


        index = (index + 1) % sample_size; // this will increment and wrap index
        if (max_x < mag_x) { // max of x
            max_x = mag_x;
        }
        if (max_y < mag_y) { // max of y
            max_y = mag_y;
        }
        if (max_z < mag_z) { // max of z
            max_z = mag_z;
        }
        if (min_x > mag_x) { // min of x
            min_x = mag_x;
        }
        if (min_y > mag_y) { // min of y
            min_y = mag_y;
        }
        if (min_z > mag_z) { // min of z
            min_z = mag_z;
        }

        float x_hat = (mag_x - mag_x0) / mag_a; // calibrated x
        float y_hat = (mag_y - mag_y0) / mag_b; // calibrated y
        float z_hat = (mag_z - mag_z0) / mag_c; // calibrated z

        printf("X:%f, Y:%f, Z:%f\nAverage: %d\n", x_hat, y_hat, z_hat, average);
        printf("X Max: %d\nX Min: %d\n", max_x, min_x);
        printf("Y Max: %d\nY Min: %d\n", max_y, min_y);
        printf("Z Max: %d\nZ Min: %d\n", max_z, min_z);
#endif

#ifdef gyro_test
        int gyr_x = BNO055_ReadGyroX();
        int gyr_y = BNO055_ReadGyroY();
        int gyr_z = BNO055_ReadGyroZ();
        float x_hat = (gyr_x - X_bias) / 131; // calibrated x, divide by 131 or 16 depends on the board firmware of current board
        float y_hat = (gyr_y - Y_bias) / 131; // calibrated y, divide by 131 or 16 depends on the board firmware of current board
        float z_hat = (gyr_z - Z_bias) / 131; // calibrated z, divide by 131 or 16 depends on the board firmware of current board

        integration = x_hat + y_hat + z_hat;
        printf("Angle: %d\n", integration);
#endif        
        if (count < NOPS_FOR_20_MS) {
            asm("nop");
            count++;
        } else {
            count = 0;
        }

    }

    return (EXIT_SUCCESS);
}

