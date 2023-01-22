/*
 * PID.h
 *
 *  Created on: Jan 21, 2023
 *      Author: anton_000
 */

#ifndef INC_PID_H_
#define INC_PID_H_

typedef struct{
	float KU1;
	float KU2;
	float KE0;
	float KE1;
	float KE2;
	float e1;
	float e2;
	float u1;
	float u2;
	float tmp;
	float integral_error;
}PID_typedef;
void PIDInit(PID_typedef* PID,float KP,float KI,float KD,float TS,float Fcoef,float KW);
float PIDCall(PID_typedef* PID,float Setpoint,float Sensor);
#endif /* INC_PID_H_ */
