#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S3,     USfront,        sensorSONAR)
#pragma config(Sensor, S4,     HTSMUX,         sensorLowSpeed)
#pragma config(Motor,  motorA,          arm,           tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     FrontRight,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackRight,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Flipper,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lift,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     BackLeft,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     FrontLeft,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    grabber,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    USfrontservo,         tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    USbackservo,          tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    trigger,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//!!!!!!!!!!!!!!!!!!!!!!!!ALWAYS CHANGE SENSOR S4 HTSMUX to sensorLowSpeed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*version 2/10/15
*author: Eula, May, Kara
*status: post league championship fix-ups
*/

#include "JoystickDriver.c"
#include "include\hitechnic-irseeker-v2.h"
#include "include\hitechnic-sensormux.h"
#include "include\lego-touch.h"
#include "include\lego-ultrasound.h"
#include "include\hitechnic-gyro.h"

//const tMUXSensor USback = msensor_S4_1;
const tMUXSensor USback = msensor_S4_2;
const tMUXSensor TOUCHfront = msensor_S4_3;
const tMUXSensor TOUCHback = msensor_S4_4;

//everything is in centimeters
static float encoderScale=1120.0;
static float wheelRadius=((9.7)/2);
static float wheelCircumference=PI*2*wheelRadius;
static int counter = 0;
static bool isUp = false;
task main()
{
	nMotorEncoder[Lift]=0;
	motor[Lift]=50;
//	while(abs(nMotorEncoder[Lift])<encoderScale*9.0) //!!REMBER TO CHANGE TO THIS!!!
	while(abs(nMotorEncoder[Lift])<encoderScale*12.5)
	{
	}
	motor[Lift]=0;
}
