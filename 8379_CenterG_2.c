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
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
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

//------------------------------nonmotor related basic stuffs--------------------------------------------------------------------------------------------
void initUS()
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

void moveTillUS(float speed, float degrees, float speedRotation, float threshold, bool till)//if till = true, move until sees something; if till = false, move until not seeing something
{
	mecJustMove(speed, degrees, speedRotation);
	if (till){
		while ((SensorValue(USfront) > threshold)){}}
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
void ballRelease(int time)//in second
{

}

bool alignRecursiveT()//true = we are all set, false = nope not even touching now and need to realign
//don't know if RobotC allows me to do recursive or would the robot crash...?
//**the function can stop and alarm when it is not aligning anymore, which is better than alignT()
{
	nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));

	if (TSreadState(TOUCHfront) == 1 && TSreadState(TOUCHback) == 1)// if both of them are touching
	{
		nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
		//mecMove(-20, 0, 0, 3);
		playSound(soundUpwardTones);
		wait1Msec(1000);
		return true;
	}
	if (counter >= 10){
		playSound(soundDownwardTones);
		wait1Msec(1000);
		return false;
	}

	counter++;
	bool result;
	if (TSreadState(TOUCHfront) == 1 || TSreadState(TOUCHback) == 1)//run if at least one of them is touching, else... it is just unfortunate
	{

		//		nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
		//mecMove(10, 90, 0, 2);
		short reading1 = TSreadState(TOUCHfront), reading2 = TSreadState(TOUCHback);
		short direction = TSreadState(TOUCHfront)? 1:-1;//if only the front sensor is active, move forward
		int tempspeed = 10*direction;//positive speed = forward, negative = backward
		mecJustMove(tempspeed, 0, 0);
		while(TSreadState(TOUCHfront) == reading1 && TSreadState(TOUCHback) == reading2)//stop if at least one of them is different from the beginning
		{
			if (counter>=10)
				break;
			nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
		}
		Stop();
		result = alignRecursiveT();
//		result==true?	playSound(soundUpwardTones): playSound(soundDownwardTones);
		return result;
	}

	else{
		moveTillTouch(10, 90, 0, true);
		result = alignRecursiveT();
//		result==true?	playSound(soundUpwardTones): playSound(soundDownwardTones);
		return result;
	}
}

//----------------Sequential Stuffs--------------------------------------------------------------------------------------------------------------------------------

void kickstand()
{
	mecMove(80,180, 0, 38);
	turnMecGyro(50, 270);
	armOut();
	mecMove(80, 180, 0, 90);
}

void liftUp()
{
	nMotorEncoder[Lift]=0;
	motor[Lift]=-38;
	//servo[liftRelease] = 255;
	while(abs(nMotorEncoder[Lift])<encoderScale*13.2) //up ratio -38/(255-127) = -.297
	{
	}
	motor[Lift]=0;
	//servo[liftRelease] = 127;
}


void liftDown()
{
	nMotorEncoder[Lift]=0;
	motor[Lift]=10;
	//servo[liftRelease] = 105;
	while(abs(nMotorEncoder[Lift])<encoderScale*12.6) //down ratio 10/(105-127) = -.455
	{
	}
	motor[Lift]=0;
	//servo[liftRelease] = 127;
}


void readUSavg(float &frontS, float &backS)
{
	int f=0, b=0;
	for (int i=0; i<10; i++)
	{
		f+=SensorValue(USfront);
		b+=USreadDist(USback);
	}
	frontS=f/10.0;
	backS=b/10.0;
}

//===================================================================================================================================
task simuLift()
{
	liftUp();
}

task kickStand()
{
	kickstand();
}

task timePos1()
{
	time1[T2]=0;
	while(true)
	{
		if (T2> 20000){
			counter = 10;
			break;
		}
	}

}

task timePos2()
{
	time1[T2]=0;
	while(true)
	{
		if (T2> 20000){
			counter = 10;
			break;
		}
	}

}

task timePos3()
{
	time1[T2]=0;
	while(true)
	{
		if (T2> 20000){
			counter = 10;
			break;
		}
	}
}
//============================================================================================
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
//	StartTask(simuLift);
	nxtDisplayCenteredTextLine(2, "%d", delay);
	wait1Msec(1000*delay);

	eraseDisplay();
//*************Initialization******************************
	int pos3 = 40;
	servo[hood] = pos3;//hood in place
	initUS();
	int Cposition;

//********Position detection*******************************************************************
/*	DisplayCenteredTextLine(2, "%d, %d", USreadDist(USfront),SensorValue(USback));
	wait1Msec(500);
	eraseDisplay();
	DisplayCenteredTextLine(2, "%d, %d", USreadDist(USfront),SensorValue(USback));
	wait1Msec(500);
	eraseDisplay();
	DisplayCenteredTextLine(2, "%d, %d", USreadDist(USfront),SensorValue(USback));
	wait1Msec(500);
	eraseDisplay();*/
	float frontS=0, backS=0;
	readUSavg(frontS, backS);

	if (frontS > 250 && backS > 250) {
		Cposition = 2;
		DisplayCenteredTextLine(2, "%d", Cposition);
		playSound(soundDownwardTones);
	}
	else if(frontS > 100 && frontS < 113){//the other value is... doesn't matter
		Cposition = 3;
		DisplayCenteredTextLine(2, "%d", Cposition);
		playSound(soundUpwardTones);
	}
	else{ //values between 128-134, but don't really need it
		Cposition = 1;
		DisplayCenteredTextLine(2, "%d", Cposition);
		playSound(soundBeepBeep);
	}


	DisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));

mecMove(10, 90, 0, 3);//move away from the wall

	switch (Cposition)
	{
	case 1:{
			startTask(timePos1);
			mecMove(90, 90, 0, 80);//move sideway
			mecMove(90, 0, 0, 100);//move forward
			//moveTillUS(60, 0, 0, 70, false);//move until the center goal is out of the sight of both us
			turnMecGyro(60, 78.0);//turn parallel to the wall
			wait1Msec(1000);
			mecMove(90, 0, 0, 60);
			wait1Msec(1000);
			moveTillUS(90, 0, 0, 60, true);
			mecMove(90, 0, 0, 28);
			wait1Msec(1000);
			moveTillTouch(60, 90, 0, true);
			wait1Msec(500);
			alignRecursiveT();
			wait1Msec(500);
		//ballRelease(2.0);
			//mecMove(60, 270, 0, 15);
			//StartTask(kickStand);
			//liftDown();
		};
		break;
	case 2:{
			startTask(timePos2);
			mecMove(90, 90, 0, 50);
			turnMecGyro(60, 17.0);
			mecMove(90, 0, 0, 60);
			//wait1Msec(1000);
			moveTillUS(90, 0, 0, 60, true);
			wait1Msec(200);
			mecMove(90, 0, 0, 10);
			wait1Msec(200);
			moveTillTouch(60, 90, 0, true);
			wait1Msec(200);
			alignRecursiveT();
			wait1Msec(200);
			//ballRelease(2.0);
			//mecMove(60, 270, 0, 15);
			//StartTask(kickStand);
			//liftDown();
		};
		break;
	case 3:{
			startTask(timePos3);
			mecMove(40, 0, 0, 18);
			moveTillTouch(70, 90, 0, true);
			wait1Msec(500);
			alignRecursiveT();
			wait1Msec(500);
			//ballRelease(2.0);
			//mecMove(60, 270, 0, 15);
			//StartTask(kickStand);
			//liftDown();
		}
		break;
	}


//---------------------------------------------------------------------------
	}
