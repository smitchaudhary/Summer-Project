// The myGYRO.h library
//
//
//
//////////////////////////////////////////////////////////////////////
#ifndef SRC_MYGYRO_H_
#define SRC_MYGYRO_H_
#endif /* SRC_MYGYRO_H_ */

#include "stdio.h"

// Structure of rotational matrices
typedef struct mat3{
	double mat[3][3];
} mat3;

// Multiplies "mat1" and "mat2", and stores the result in "res"
void myGYRO_Multiply(mat3 mat1, mat3 mat2, mat3* res){
	(*res).mat[0][0] = (mat1.mat[0][0]) * (mat2.mat[0][0]) + (mat1.mat[0][1]) * (mat2.mat[1][0]) + (mat1.mat[0][2]) * (mat2.mat[2][0]);
	(*res).mat[0][1] = (mat1.mat[0][0]) * (mat2.mat[0][1]) + (mat1.mat[0][1]) * (mat2.mat[1][1]) + (mat1.mat[0][2]) * (mat2.mat[2][1]);
	(*res).mat[0][2] = (mat1.mat[0][0]) * (mat2.mat[0][2]) + (mat1.mat[0][1]) * (mat2.mat[1][2]) + (mat1.mat[0][2]) * (mat2.mat[2][2]);
	(*res).mat[1][0] = (mat1.mat[1][0]) * (mat2.mat[0][0]) + (mat1.mat[1][1]) * (mat2.mat[1][0]) + (mat1.mat[1][2]) * (mat2.mat[2][0]);
	(*res).mat[1][1] = (mat1.mat[1][0]) * (mat2.mat[0][1]) + (mat1.mat[1][1]) * (mat2.mat[1][1]) + (mat1.mat[1][2]) * (mat2.mat[2][1]);
	(*res).mat[1][2] = (mat1.mat[1][0]) * (mat2.mat[0][2]) + (mat1.mat[1][1]) * (mat2.mat[1][2]) + (mat1.mat[1][2]) * (mat2.mat[2][2]);
	(*res).mat[2][0] = (mat1.mat[2][0]) * (mat2.mat[0][0]) + (mat1.mat[2][1]) * (mat2.mat[1][0]) + (mat1.mat[2][2]) * (mat2.mat[2][0]);
	(*res).mat[2][1] = (mat1.mat[2][0]) * (mat2.mat[0][1]) + (mat1.mat[2][1]) * (mat2.mat[1][1]) + (mat1.mat[2][2]) * (mat2.mat[2][1]);
	(*res).mat[2][2] = (mat1.mat[2][0]) * (mat2.mat[0][2]) + (mat1.mat[2][1]) * (mat2.mat[1][2]) + (mat1.mat[2][2]) * (mat2.mat[2][2]);
}

// Sets "matrix" to an identity matrix
void myGYRO_setIdentity(mat3* matrix){
	for(int i = 0; i<3; i++){
		for(int j = 0; j<3; j++){
			if(i == j)
				(*matrix).mat[i][j] = 1;
			else
				(*matrix).mat[i][j] = 0;
		}
	}
}

// Returns the signed 2's complement of "num"
int myGYRO_twosComplement(int16_t num){
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

// Stores the inverse of "matrix" in "inv_matrix"
void myGYRO_Inverse(mat3 matrix, mat3* inv_matrix){
	for(int i = 0; i<3; i++){
		for(int j = 0; j<3; j++){
			(*inv_matrix).mat[i][j] = matrix.mat[j][i];
		}
	}
}

// Returns the value of "deg" in radians
double myGYRO_degToRad(double deg){
	double rad = deg*(3.141529635/180);

	return rad;
}

// Gives the rotation matrix due to rotation about the X-Axis by an infinitesimal amount "thetaX"
// which transforms a vector in the ground frame to a vector in the GYRO frame
void myGYRO_RotX(double thetaX, mat3* res){
	(*res).mat[0][0] = 1;
	(*res).mat[0][1] = 0;
	(*res).mat[0][2] = 0;
	(*res).mat[1][0] = 0;
	(*res).mat[1][1] = 1 - thetaX*thetaX/2;
	(*res).mat[1][2] = thetaX - thetaX*thetaX*thetaX/6;
	(*res).mat[2][0] = 0;
	(*res).mat[2][1] = -(thetaX - thetaX*thetaX*thetaX/6);
	(*res).mat[2][2] = 1 - thetaX*thetaX/2;
}

// Gives the rotation matrix due to rotation about the Y-Axis by an infinitesimal amount "thetaY"
// which transforms a vector in the ground frame to a vector in the GYRO frame
void myGYRO_RotY(double thetaY, mat3* res){
	(*res).mat[0][0] = 1 - thetaY*thetaY/2;
	(*res).mat[0][1] = 0;
	(*res).mat[0][2] = -(thetaY - thetaY*thetaY*thetaY/6);
	(*res).mat[1][0] = 0;
	(*res).mat[1][1] = 1;
	(*res).mat[1][2] = 0;
	(*res).mat[2][0] = (thetaY - thetaY*thetaY*thetaY/6);
	(*res).mat[2][1] = 0;
	(*res).mat[2][2] = 1 - thetaY*thetaY/2;
}

// Gives the rotation matrix due to rotation about the Z-Axis by an infinitesimal amount "thetaZ"
// which transforms a vector in the ground frame to a vector in the GYRO frame
void myGYRO_RotZ(double thetaZ, mat3* res){
	(*res).mat[0][0] = 1 - thetaZ*thetaZ/2;
	(*res).mat[0][1] = thetaZ - thetaZ*thetaZ*thetaZ/6;
	(*res).mat[0][2] = 0;
	(*res).mat[1][0] = -(thetaZ - thetaZ*thetaZ*thetaZ/6);
	(*res).mat[1][1] = 1 - thetaZ*thetaZ/2;
	(*res).mat[1][2] = 0;
	(*res).mat[2][0] = 0;
	(*res).mat[2][1] = 0;
	(*res).mat[2][2] = 1;
}

// Gives the rotation matrix due to rotation about the X-Axis, Y-Axis, and Z-Axis by infinitesimal
// amounts "thetaX", "thetaY", and "thetaZ" which transforms a vector in the ground frame to a
// vector in the GYRO frame

void myGYRO_Rot(double thetaX, double thetaY, double thetaZ, mat3* res){
	(*res).mat[0][0] = 1;
	(*res).mat[0][1] = thetaZ;
	(*res).mat[0][2] = -thetaY;
	(*res).mat[1][0] = -thetaZ;
	(*res).mat[1][1] = 1;
	(*res).mat[1][2] = thetaX;
	(*res).mat[2][0] = thetaY;
	(*res).mat[2][1] = -thetaX;
	(*res).mat[2][2] = 1;
}

// Function to print the rotation matrix
void myGYRO_printMat(mat3 matrix){
	printf("%lf\t%lf\t%lf\n\r", matrix.mat[0][0], matrix.mat[0][1], matrix.mat[0][2]);
	printf("%lf\t%lf\t%lf\n\r", matrix.mat[1][0], matrix.mat[1][1], matrix.mat[1][2]);
	printf("%lf\t%lf\t%lf\n\r", matrix.mat[2][0], matrix.mat[2][1], matrix.mat[2][2]);
}
