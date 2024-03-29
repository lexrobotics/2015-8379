#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S3,     TOUCHback,      sensorTouch)
#pragma config(Sensor, S4,     TOUCHfront,     sensorTouch)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
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
#pragma config(Servo,  srvo_S1_C3_3,    arm,                  tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    holder,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_5,    trigger,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 5/19/15   Status: config changed for Discovery Day after Next Gen stuffs
//Remember to switch controller mode from X mode to D mode if it is not already there


#include "JoystickDriver.c"
#include "include\hitechnic-irseeker-v2.h"
#include "include\hitechnic-sensormux.h"
#include "include\lego-touch.h"
#include "include\lego-ultrasound.h"
#include "include\hitechnic-gyro.h"

//const tMUXSensor TOUCHFront = msensor_S4_3;
//const tMUXSensor TOUCHBack = msensor_S4_4;

static float encoderScale=1120.0;
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
	//pos1+=5;
	//if (pos1 >= 215) pos1=215;
	servo[grabber] = 255;
}

task grabberDown()
{
	//pos1-=5;
	//if (pos1 <= 125) pos1=125;
	servo[grabber] = 150;
}

//=========================Kickstand Arm======================================================
task kArmOut()
{
	servo[arm] = 200;
}

task kArmIn()
{
	servo[arm] = 25;
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

task triggerDown()
{
	servo[trigger] = 25;
}

//========================Hood===========================================================
task hoodOut()
{
	servo[holder] = 100;
	wait1Msec(50);
	servo[holder] = 127;
	//	wait1Msec(1);
	servo[hood] = 130;
}

task hoodIn()
{
	servo[hood] = 45;
	wait1Msec(50);
	servo[holder] = 255; //rotate forward
	wait1Msec(100);
	servo[holder] = 127;
	//	wait1Msec(1);
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

	servo[grabber] = 255;
	servo[hood] = 50;
	servo[trigger] = 182;
	servo[holder] = 127;
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
			motor[FrontLeft] = (-100.0/256 * (-joystick.joy1_y1 - (((joystick.joy1_x1 + joystick.joy1_x2) / 2) * 1.2)));
			motor[BackLeft] = (-100.0/256 * (-joystick.joy1_y1 + (((joystick.joy1_x1 + joystick.joy1_x2) / 2) * 1.2)));
			motor[FrontRight] =  (-100.0/256 * (-joystick.joy1_y2 + (((joystick.joy1_x1 + joystick.joy1_x2) / 2) * 1.2)));
			motor[BackRight] = (-100.0/256 * (-joystick.joy1_y2 - (((joystick.joy1_x1 + joystick.joy1_x2) / 2) * 1.2)));
		}

		else{
			motor[FrontRight] = frontback * (-100.0/256 * (-joystick.joy1_y1 - frontback * (((joystick.joy1_x1 + joystick.joy1_x2) / 2) * 1.2)));
			motor[BackRight] = frontback * (-100.0/256 * (-joystick.joy1_y1 + frontback * (((joystick.joy1_x1 + joystick.joy1_x2) / 2) * 1.2)));
			motor[FrontLeft] = frontback * (-100.0/256 * (-joystick.joy1_y2 + frontback * (((joystick.joy1_x1 + joystick.joy1_x2) / 2) * 1.2)));
			motor[BackLeft] = frontback * (-100.0/256 * (-joystick.joy1_y2 - frontback * (((joystick.joy1_x1 + joystick.joy1_x2) / 2) * 1.2)));
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
		if(joy2Btn(5))	{startTask(hoodIn);}
		if(joy2Btn(7))	{startTask(hoodOut);}
		//==================================================Lift======================================================================================
		if(joy2Btn(6))	{startTask(liftUp);}
		if(joy2Btn(8))	{startTask(liftDown);}
		if(joy1Btn(3)) 	{startTask(triggerDown);}

		wait1Msec(10);
	}
}
