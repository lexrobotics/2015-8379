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
#pragma config(Servo,  srvo_S1_C3_4,    holder,               tServoContinuousRotation)
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
int grabberDelayTime;


//----------------------------------plain movement stuffs---------------------------------------------------------------------------------
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

void turnMecGyro(int speed, float degrees) {
	float delTime = 0;
	float curRate = 0;
	float currHeading = 0;
	Stop();
	wait1Msec(500);
	HTGYROstartCal(gyro);
	wait1Msec(500);
	//playSound(soundBeepBeep);
	mecJustMove (0, 0, speed);//+ = right   - = turn left
	while (abs(currHeading) < abs(degrees)) {
		time1[T1] = 0;
		curRate = HTGYROreadRot(gyro);
		if (abs(curRate) > 3) {
			currHeading += curRate * delTime; //Approximates the next heading by adding the rate*time.
			if (currHeading > 360) currHeading -= 360;
			else if (currHeading < -360) currHeading += 360;
		}
		wait1Msec(5);
		delTime = ((float)time1[T1]) / 1000; //set delta (zero first time around)
	}
	Stop();
}


void liftPart()
{
	nMotorEncoder[Lift]=0;
	motor[Lift]=-100;
	while(abs(nMotorEncoder[Lift])<encoderScale*2.6) //up ratio -38/(255-127) = -.297
	{
	}
	motor[Lift]=0;
}


//============================================================================================

task hoodRelease()
{
	servo[holder] = 100;
	wait1Msec(200);
	servo[holder] = 127;
}

task hoodHold()
{
servo[holder] = 255; //rotate forward
wait1Msec(200);
servo[holder] = 127;
}

task grabbersDownDelay()
{
	wait1Msec(1000*grabberDelayTime);
	servo[grabber] = 150;
}

task grabbersUpDelay()
{
	wait1Msec(1000*grabberDelayTime);
	servo[grabber] = 255;
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
	eraseDisplay();

	//*************Initialization******************************

	servo[grabber] = 255;
	servo[trigger] = 220;
	servo[hood] = 60;//hood in place
	startTask(hoodRelease);

	//liftPart();

	mecJustMove(60, 0, 0);
	wait1Msec(3500);
	Stop();
	wait1Msec(250);

	mecMove(-78, 90, 0,  52.0);//strafe left
	wait1Msec(250);

	grabberDelayTime = 3.0;
	startTask(grabbersDownDelay);
	mecMove(78, 0, 0, 150.0);//forward toward goal

	wait1Msec(500);
	mecMove(-78, 0, 0, 10.0);//back a bit
	wait1Msec(100);
	servo[hood] = 130;//hood in place
	mecMove(-78, 90, 0, 10.0);//side shift a bit

	//turnMecGyro(-60.0,155.0);//turn toward the PK
	motor[Flipper] = -100.0; //start thrower motor
	mecMove(-78.0, 0, 0, 242.0);//**length: move pass the kick stand
	wait1Msec(250);
	mecMove(-78.0, 90, 0, 30.0);//**right strafe significantly pz
	wait1Msec(250);
	turnMecGyro(-60.0,160.0)//turn inside pz
	mecMove(78.0, 90, 0, 10.0);

	wait1Msec(15000);
	//---------------------------------------------------------------------------
}
