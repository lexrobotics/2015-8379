#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S3,     USback,         sensorSONAR)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
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
#pragma config(Servo,  srvo_S1_C3_3,    USbackservo,          tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    USfrontservo,         tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    trigger,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 2/11/15   Status: different function = different task

#include "JoystickDriver.c"
#include "include\hitechnic-irseeker-v2.h"
#include "include\hitechnic-sensormux.h"
#include "include\lego-touch.h"
#include "include\lego-ultrasound.h"
#include "include\hitechnic-gyro.h"
 task main()
 {
//const tMUXSensor TOUCHFront = msensor_S4_3;
//const tMUXSensor TOUCHBack = msensor_S4_4;
	servo[grabber] = 255;//up
	wait1Msec(2000);
	servo[grabber] = 150;//down
	wait1Msec(2000);
	}
