#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
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
#pragma config(Servo,  srvo_S1_C3_5,    trigger,              tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 1/15/15 tested out for Meet 3, adjusted with mecanum wheels
//!!!!!!!!!!!!!!!!!!!!!!!!ALWAYS CHANGE SENSOR S4 HTSMUX to sensorLowSpeed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "JoystickDriver.c"
#include "include\hitechnic-irseeker-v2.h"
#include "include\hitechnic-sensormux.h"
#include "include\lego-touch.h"
#include "include\lego-ultrasound.h"
#include "include\hitechnic-gyro.h"

//everything is in centimeters
static float encoderScale=1120.0;
static float wheelRadius=(9.7/2); //was 9.7/2
static float wheelCircumference=PI*2*wheelRadius;
//static float AngleRatio = ? //used as angle(in degree) * AngleRatio


//------------------------initialization etc-----------------------------//
void initUS() //Initializes ultrasonic sensors
{
	servo[USfrontservo] = 120;
	servo[USbackservo] = 108;
}

void resetEncoders(){
	nMotorEncoder[FrontLeft] = 0;
	nMotorEncoder[FrontRight] = 0;
	nMotorEncoder[BackLeft] = 0;
	nMotorEncoder[BackRight] = 0;
}

void Stop()
{
	motor[BackLeft] = 0;
	motor[BackRight] = 0;
	motor[FrontLeft] = 0;
	motor[FrontRight] = 0;
}

//----------------------------------plain movement stuffs---------------------------------------------------------------------------------
void mecJustMove(int speed, float degrees, float speedRotation)
{
	motor[FrontLeft] = speed * sinDegrees(degrees + 45) + speedRotation;
	motor[FrontRight] = speed * cosDegrees(degrees + 45) - speedRotation;
	motor[BackLeft] = speed * cosDegrees(degrees + 45) + speedRotation;
	motor[BackRight] = speed * sinDegrees(degrees + 45) -  speedRotation;
}

void mecMove(float speed, float degrees, float speedRotation, float distance)
{ //speed [-100,100], degrees [0, 360] to the right, speedRotation [-100,100], distance cm

	resetEncoders();
	float min = 0.0;
	if (cosDegrees(degrees) == 0.0 || sinDegrees(degrees) == 0.0)
	{
		min = 1.0;
	}
	else if (abs(1.0/cosDegrees(degrees))<= abs(1.0/sinDegrees(degrees)))
	{
		min = 1.0/cosDegrees(degrees);
	}
	else
	{
		min = 1.0/sinDegrees(degrees);
	}

	float scaled = abs(encoderScale* (distance * min / wheelCircumference));
	mecJustMove(speed, degrees, speedRotation);
	while((abs(nMotorEncoder[FrontLeft])<scaled) && (abs(nMotorEncoder[FrontRight])<scaled) && (abs(nMotorEncoder[BackLeft])< scaled) && (abs(nMotorEncoder[BackRight])< scaled))
	{}
	Stop();
	resetEncoders();
	wait1Msec(10);
}

task main()
{
	int delay=0;
	while(nNxtButtonPressed!=3){
		if(nNxtButtonPressed==1) delay++;
		else if(nNxtButtonPressed==2 && delay>0) delay--;
		nxtDisplayCenteredTextLine(2, "%d", delay);
		wait1Msec(200);
	}
	waitForStart();
	nxtDisplayCenteredTextLine(2, "%d", delay);
	wait1Msec(1000*delay);

	initUS();
	servo[grabber] = 255;
	servo[trigger] = 220;
	servo[hood] = 60;//hood in place

	mecMove(60, 0, 0, 170.0);

}