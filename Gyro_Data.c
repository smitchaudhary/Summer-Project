//Takes 'Angular Velocity about the three axes' from the Gyroscope and prints it in degrees per second.

#include "stdio.h"
#include "xparameters.h" //Contains base addresses of peripherals. Available here : https://github.com/Xilinx/embeddedsw/blob/master/lib/sw_apps/zynq_fsbl/misc/zed/xparameters.h
#include "PmodGYRO.h" //Contains functions for Gyroscope. Available here :  https://github.com/Digilent/vivado-library/blob/master/ip/Pmods/PmodGYRO_v1_0/drivers/PmodGYRO_v1_0/src/PmodGYRO.h

//Function returns the twos complement of the input
int twosComplement(int16_t);

int main(){
	//Initializes the PmodGYRO object
	PmodGYRO gyro;
	//Starts the sensor
	GYRO_begin(&gyro, XPAR_PMODGYRO_0_AXI_LITE_SPI_BASEADDR, XPAR_PMODGYRO_0_AXI_LITE_GPIO_BASEADDR);

	//Variables to store the initial bias of the gyroscope
	double initialX = 0, initialY = 0, initialZ = 0;

	//The number of samples to be taken
	int readings = 100;

	for(int i = 0; i < readings; i++){
		printf("Initializing.....\n\r");
		printf("\x1B[2J");
		printf("\x1B[H");
		initialX = initialX + twosComplement(GYRO_getX(&gyro));
		initialY = initialY + twosComplement(GYRO_getY(&gyro));
		initialZ = initialZ + twosComplement(GYRO_getZ(&gyro));
	}
	initialX = initialX/readings;
	initialY = initialY/readings;
	initialZ = initialZ/readings;

	//Variables to store the gyroscope data
	double omegaX = 0, omegaY = 0, omegaZ = 0;
	while(1){
		//Read the gyroscope data
		omegaX = (twosComplement(GYRO_getX(&gyro)) - initialX)*0.00875;
		omegaY = (twosComplement(GYRO_getY(&gyro)) - initialY)*0.00875;
		omegaZ = (twosComplement(GYRO_getZ(&gyro)) - initialZ)*0.00875;
		//Print the gyroscope data
		printf("Angular Velocity:\t(%lf,\t%lf,\t%lf)\n\r", omegaX, omegaY, omegaZ);
	}
	return 0;
}

int twosComplement(int16_t num){
	int result = 0;
	if((num & 0b1000000000000000) == 0b1000000000000000){
		result = num & 0b0111111111111111;
		result = result ^ 0b111111111111111;
		result = -(result + 1);
	}
	else
		result = num;
	return result;
}
