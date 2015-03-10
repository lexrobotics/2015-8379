#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S3,     USfront,        sensorSONAR)
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

//#include "JoystickDriver.c"
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

//------------------------------nonmotor related basic stuffs--------------------------------------------------------------------------------------------
void initUS() //Initializes ultrasonic sensors
{
	servo[USfrontservo] = 120;
	servo[USbackservo] = 108;
}

void armOut(){
	time1[T3]=0;
	while((float)time1[T3]<2000){
		motor[arm] = -50;
	}
	motor[arm] = 0;
}

void armIn(){
	time1[T3]=0;
	while((float)time1[T3] < 2000){
		motor[arm] = 50;
	}
	motor[arm] = 0;
}
//----------------------------------plain movement stuffs---------------------------------------------------------------------------------
void resetEncoders(){
	nMotorEncoder[FrontLeft] = 0;
	nMotorEncoder[FrontRight] = 0;
	nMotorEncoder[BackLeft] = 0;
	nMotorEncoder[BackRight] = 0;
	wait1Msec(50);
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

//-------------Move Till stuffs-------------------------------------------------------------------------------------------------------------
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
	{
		wait1Msec(50);
		writeDebugStreamLine("%d, %d, %d, %d ", (nMotorEncoder[FrontLeft]), (nMotorEncoder[FrontRight]), (nMotorEncoder[BackLeft]), (nMotorEncoder[BackRight]));
	}
	//DisplayCenteredTextLine(2, "%d, %d, %d, %d",(int)nMotorEncoder[FrontRight]/100,(int)nMotorEncoder[BackRight]/100, (int)nMotorEncoder[FrontLeft]/100, (int)nMotorEncoder[BackLeft]/100);}
	eraseDisplay();
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

void moveTillUS(float speed, float degrees, float speedRotation, float threshold, bool till)//if till = true, move until sees something; if till = false, move until not seeing something
{
	mecJustMove(speed, degrees, speedRotation);
	if (till){
		while ((SensorValue(USfront) > threshold)){
			nxtDisplayCenteredTextLine(2, "%d, %d", (SensorValue(USfront)), (USreadDist(USback));
	}}
	else{
		while ((SensorValue(USfront)) < threshold || (USreadDist(USback) < threshold)){}}//should be ||, so stop when none of them is in the threshold
	Stop();
}

void parallel()
{
float difference=SensorValue(USfront) > USreadDist(USback)? 20:-20;//so that the sensors doesn't have to detect twice; to save batteries
	mecJustMove(0, 0, difference);
	while(abs(SensorValue(USfront)-USreadDist(USback))>5)
	{}
	Stop();
}

void moveTillTouch(float speed, float degrees, float speedRotation, bool till)
{
	mecJustMove(speed, degrees, speedRotation);
	if (till){
		while ((!TSreadState(TOUCHFront)) && (!TSreadState(TOUCHBack))){
			nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
			//if(HTGYROreadRot(gyro)>5){break;}
			if (counter>=10)
				break;
		}
	}
	else
	{
		while ((TSreadState(TOUCHFront)) || (TSreadState(TOUCHBack))){
			//if(HTGYROreadRot(gyro)>5){break;}
			if (counter>=10)
				break;
		}
	}
	Stop();
}

//--------------------Align stuffs-----------------------------------------------------------------------------------------------------------------------------------

void readUSavg(float &frontS, float &backS)
{
	int f=0, b=0;
	int tfront, tback;
	float tcountF = 0.0, tcountB = 0.0;
	for (int i=0; i<20; i++)
	{
		tfront = SensorValue(USfront);
		tback = USreadDist(USback);
		DisplayCenteredTextLine(2, "%d, %d", tfront, tback);
		if (tfront > 40)//95
		{
			tcountF++;
			f+=tfront;
		}
		if (tback > 40)//95
		{
			tcountB++;
			b+=tback;
		}
		wait1Msec(50);

	}
	frontS=f/tcountF;
	backS=b/tcountB;
	DisplayCenteredTextLine(2, "%d, %d", tcountF, tcountB);
}




//============================================================================================
task main()
{
	/*int delay=0;
	while(nNxtButtonPressed!=3){
		if(nNxtButtonPressed==1) delay++;
		else if(nNxtButtonPressed==2 && delay>0) delay--;
		nxtDisplayCenteredTextLine(2, "%d", delay);
		wait1Msec(200);
	}
	waitForStart();
	nxtDisplayCenteredTextLine(2, "%d", delay);
	wait1Msec(1000*delay);
	eraseDisplay();*/

	while(true)
	{
		displayCenteredTextLine(2, "%d", nMotorEncoder[FrontLeft] );
			displayCenteredTextLine(3, "%d", nMotorEncoder[FrontRight] );
				displayCenteredTextLine(4, "%d", nMotorEncoder[BackLeft] );
					displayCenteredTextLine(5, "%d", nMotorEncoder[BackRight] );
	}
/*
	//*************Initialization******************************
	servo[hood] = 60;//hood in place
	initUS();
	servo[grabber] = 150;
	servo[trigger] = 120;
	int Cposition;

	//********Position detection*******************************************************************
	for(int i = 0; i<5; i++){
		//writeDebugStreamLine("************************************************************");
		mecMove(78, 0, 0, 80);
		//mecMove(80, 90, 0, 30);
		//mecMove(80, 135, 0, 30);
	}
	*/
}


//---------------------------------------------------------------------------
