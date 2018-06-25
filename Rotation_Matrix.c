#include "PmodGYRO.h" //Contains Gyroscope functions. Available at : https://github.com/Digilent/vivado-library/blob/master/ip/Pmods/PmodGYRO_v1_0/drivers/PmodGYRO_v1_0/src/PmodGYRO.h
#include "xparameters.h" //Contains base addresses of peripherals. Available here : https://github.com/Xilinx/embeddedsw/blob/master/lib/sw_apps/zynq_fsbl/misc/zed/xparameters.h
#include "stdio.h"
#include "xtime_l.h" //To note time
#include "myGYRO.h" //Library created of other useful functions. Available at : https://github.com/smitchaudhary/Summer-Project/blob/master/myGYRO.h

int main(){
	//Initialize the PmodGYRO object
	PmodGYRO gyro;
	GYRO_begin(&gyro, XPAR_PMODGYRO_0_AXI_LITE_SPI_BASEADDR, XPAR_PMODGYRO_0_AXI_LITE_GPIO_BASEADDR);

	//"avgX", "avgY", "avgZ" store the initial X, Y, Z axis biases
	float avgX = 0, avgY = 0, avgZ = 0;
	//"readings" stores the number of samplings to be taken
	int readings = 500;
	for(int i = 0; i < readings; i++){
		printf("Initializing.....\n\r");
		printf("\x1B[2J");
		printf("\x1B[H");
		for(int j = 0; j < 1000000; j++);
		avgX = avgX + (double)myGYRO_twosComplement(GYRO_getX(&gyro));
		avgY = avgY + (double)myGYRO_twosComplement(GYRO_getY(&gyro));
		avgZ = avgZ + (double)myGYRO_twosComplement(GYRO_getZ(&gyro));
	}
	avgX = avgX/readings;
	avgY = avgY/readings;
	avgZ = avgZ/readings;

	//XTime variables are initialized to measure the time
	XTime prev, curr;
	//"wX", "wY", "wZ" store the angular velocity data; "prevTime" and "currTime" store the time in seconds
	double wX = 0, wY = 0, wZ = 0, prevTime = 0, currTime = 0, deltaT = 0;

	//"rotMat" stores the rotation matrix
	mat3 rotMat;
	myGYRO_setIdentity(&rotMat);

	XTime_GetTime(&prev);
	prevTime = ((double) prev)/((double) COUNTS_PER_SECOND);

	while(1){
		//Measure the current time
		XTime_GetTime(&curr);
		currTime = ((double) curr)/((double) COUNTS_PER_SECOND);

		//Take the angular velocity data
		wX = (myGYRO_twosComplement(GYRO_getX(&gyro)) - avgX)*0.00875;
		wY = (myGYRO_twosComplement(GYRO_getY(&gyro)) - avgY)*0.00875;
		wZ = (myGYRO_twosComplement(GYRO_getZ(&gyro)) - avgZ)*0.00875;

		//The rotation matrix is calculated
		mat3 tempX, tempY, tempZ, temp1, temp2, tempM;
		deltaT = currTime - prevTime;
		myGYRO_RotX(myGYRO_degToRad(wX*deltaT), &tempX);
		myGYRO_RotY(myGYRO_degToRad(wY*deltaT), &tempY);
		myGYRO_RotZ(myGYRO_degToRad(wZ*deltaT), &tempZ);
		myGYRO_Multiply(tempX, tempY, &temp1);
		myGYRO_Multiply(temp1, tempZ, &temp2);
		myGYRO_Multiply(temp2, rotMat, &tempM);
		rotMat = tempM;

		prevTime = currTime;

		//The rotation matrix and the angular velocity is printed
		printf("Rotation Matrix:\n\r");
		myGYRO_printMat(rotMat);
		printf("\n\rAngular Velocity = (%lf,\t%lf,\t%lf)\n\r", wX, wY, wZ);
		printf("\x1B[2J");
		printf("\x1B[H");
	}

	return 0;
}
