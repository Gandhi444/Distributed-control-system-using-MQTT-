/*
 * PID.c
 *
 *  Created on: Jan 22, 2023
 *      Author: anton_000
 */
#include "PID.h"

void PIDInit(PID_typedef* PID,float KP,float KI,float KD,float TS,float Fcoef,float KW)
{
    float a0 = (1 + Fcoef * TS);
	float a1 = -(2 + Fcoef * TS);
	float a2 = 1;

	float b0=KP*(1+Fcoef * TS) + KI*TS*(1+ Fcoef*TS) + KD*Fcoef;
	float b1= -(KP*(2+Fcoef*TS) + KI*TS + 2*KD*Fcoef);
	float b2=KP + KD * Fcoef;

	PID->KU1=a1/a0;
	PID->KU2=a2/a0;
	PID->KE0=b0/a0;
	PID->KE1=b1/a0;
	PID->KE2=b2/a0;
	PID->tmp=KW*(-PID->KU1- PID->KU2)/2;
}
float PIDCall(PID_typedef* PID,float Setpoint,float Sensor)
{
	float e0=Setpoint-Sensor;
	float u0 = -PID->KU1*PID->u1 - PID->KU2 * PID->u2 + (PID->KE0*e0) + (PID->KE1*PID->e1) + (PID->KE2*PID->e2)+PID->tmp*PID->integral_error;
    if(u0 > 100)
    	PID->integral_error=  100-u0;
    else if( u0 < 0)
    	PID->integral_error= 0-u0;
    else
    	PID->integral_error=0;
    PID->e2=PID->e1;
    PID->e1=e0;
    PID->u2=PID->u1;
    PID->u1=u0;

    if(u0 > 100)
        u0= 100;
    else if( u0 < 0)
        u0= 0;
    return u0;
}
