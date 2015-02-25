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

//const tMUXSensor TOUCHFront = msensor_S4_3;
//const tMUXSensor TOUCHBack = msensor_S4_4;

	static float encoderScale=1120.0;
	static int pos1 = 215;
	static int pos4 = 60;
	static int frontback = 1;

//===========================Thrower====================================
task throwerStop()
{
	motor[Flipper] = 0;
}

task throwerForward()
{
	motor[Flipper] = -100;
}

task throwerReverse()
{
	motor[Flipper] = 50;
}
//==========================Grabber==============================================
task grabberUp()
{
	pos1+=5;
	if (pos1 >= 215) pos1=215;
	servo[grabber] = (pos1);
}

task grabberDown()
{
	pos1-=5;
	if (pos1 <= 125) pos1=125;
	servo[grabber] = (pos1);
}

//=========================Kickstand Arm======================================================
task kArmOut()
{
	motor[arm] = 50;
	while(joy1Btn(5))
	{
		wait1Msec(10);
	}
	motor[arm]=0;
}

task kArmIn()
{
	motor[arm] = -50;
	while(joy1Btn(7))
	{
		wait1Msec(10);
	}
	motor[arm]=0;
}

//=========================Lift===================================================================
task liftUp()
{
	nMotorEncoder[Lift]=0;
	motor[Lift]=-100;
//ervo[liftRelease] = 143; //up ratio of -30/(143-127) =-1.875
	while(abs(nMotorEncoder[Lift])<encoderScale)
//hile(joy2Btn(6))
	{
		wait1Msec(10);
	}
	motor[Lift]=0;
	//servo[liftRelease] = 127;
}

task liftDown()
{
	nMotorEncoder[Lift]=0;
	motor[Lift]=50;
//	servo[liftRelease] = 60;
	while(abs(nMotorEncoder[Lift])<encoderScale)
//	while(joy2Btn(8))
	{
		wait1Msec(10);
	}
	motor[Lift]=0;
	//servo[liftRelease] = 127;
}

//========================Hood===========================================================
task hoodOut()
{
	pos4 = 125;
//	wait1Msec(1);
	servo[hood] = pos4;
}

task hoodIn()
{
	pos4 = 60;
//	wait1Msec(1);
	servo[hood] = pos4;
}

//=========================Change Direction=============================================
task grabberFront()
{
	frontback = -1;
}

task flipperFront()
{
	frontback = 1;
}

//--------------------------------------------------------------------------------------
task main()
{
	servo[grabber]=pos1;
	servo[hood] = pos4;
	//initial lift
	waitForStart();

	while(true){
//===================================================driving stuffs======================================
		getJoystickSettings(joystick);
		if(abs(joystick.joy1_y1)<10)
		{
			joystick.joy1_y1=0;
		}
		if(abs(joystick.joy1_y2)<10)
		{
			joystick.joy1_y2=0;
		}
		if(abs(joystick.joy1_x1)<10)
		{
			joystick.joy1_x1=0;
		}
		if(abs(joystick.joy1_x2)<10)
		{
			joystick.joy1_x2=0;
		}

		if(frontback==1){
			motor[FrontLeft] = (-100.0/256 * (-joystick.joy1_y1 - (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[BackLeft] = (-100.0/256 * (-joystick.joy1_y1 + (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[FrontRight] =  (-100.0/256 * (-joystick.joy1_y2 + (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[BackRight] = (-100.0/256 * (-joystick.joy1_y2 - (joystick.joy1_x1 + joystick.joy1_x2) / 2));
		}

		else{
			motor[FrontRight] = frontback * (-100.0/256 * (-joystick.joy1_y1 - frontback * (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[BackRight] = frontback * (-100.0/256 * (-joystick.joy1_y1 + frontback * (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[FrontLeft] = frontback * (-100.0/256 * (-joystick.joy1_y2 + frontback * (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[BackLeft] = frontback * (-100.0/256 * (-joystick.joy1_y2 - frontback * (joystick.joy1_x1 + joystick.joy1_x2) / 2));
		}
//=================================================Thrower======================================================
		if(joy1Btn(1))	{startTask(throwerStop);}
		if(joy1Btn(6))	{startTask(throwerForward);}
		if(joy1Btn(8))	{startTask(throwerReverse);}
//=================================================kickstand arm====================================================
		if (joy1Btn(5))	{startTask(kArmOut);}
		if (joy1Btn(7))	{startTask(kArmIn);}
//=================================================change direction====================================================
		if (joy1Btn(2))	{startTask(grabberFront);}
		if (joy1Btn(4))	{startTask(flipperFront);}
//-------------------------------------------------Joystick 2 ------------------------------------------------------
//==================================================Grabber=======================================================
		if(joy2Btn(4))	{startTask(grabberUp);}
		if(joy2Btn(2))	{startTask(grabberDown);}
//==================================================Hood==============================================================
		if(joy2Btn(5))	{startTask(hoodOut);}
		if(joy2Btn(7))	{startTask(hoodIn);}
//==================================================Lift======================================================================================
		if(joy2Btn(6))	{startTask(liftUp);}
		if(joy2Btn(8))	{startTask(liftDown);}

		wait1Msec(10);
	}
}
