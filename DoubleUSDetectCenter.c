#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S3,     USback,         sensorSONAR)
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
#pragma config(Servo,  srvo_S1_C3_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    USfrontserv,          tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    USbackserv,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//!!!!!!!!!!!!!!!!!!!!!!!!ALWAYS CHANGE SENSOR S4 HTSMUX to sensorLowSpeed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*version 2/1/15
 *author: Eula, May, Kara
 *status: alignT, parallel function modified, tested with position 3, kickstand, alignUS, alignRecursiveT, turnMecGyro function added; I feel sketchy about the alignT function after adding the recursive one though
 */

#include "JoystickDriver.c"
#include "C:\Users\Robot1\Documents\Programming\robotcdriversuite\include\hitechnic-irseeker-v2.h"
#include "C:\Users\Robot1\Documents\Programming\robotcdriversuite\include\hitechnic-sensormux.h"
#include "C:\Users\Robot1\Documents\Programming\robotcdriversuite\include\lego-touch.h"
#include "C:\Users\Robot1\Documents\Programming\robotcdriversuite\include\lego-ultrasound.h"
#include "C:\Users\Robot1\Documents\Programming\robotcdriversuite\include\hitechnic-gyro.h"

//const tMUXSensor USback = msensor_S4_1;
const tMUXSensor USfront = msensor_S4_2;
const tMUXSensor TOUCHFront = msensor_S4_3;
const tMUXSensor TOUCHBack = msensor_S4_4;

//everything is in centimeters
static float encoderScale=1120.0;
static float wheelRadius=((9.7)/2);
static float wheelCircumference=PI*2*wheelRadius;

//------------------------------nonmotor related basic stuffs--------------------------------------------------------------------------------------------
void initUS()
{
		servo[USfrontserv] = 220;
		servo[USbackserv] = 118;
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
	while((nMotorEncoder[FrontLeft]<scaled) && (nMotorEncoder[FrontRight]<scaled) && (nMotorEncoder[BackLeft]< scaled) && (nMotorEncoder[BackRight]< scaled))
	{}
	Stop();
	resetEncoders();
	wait1Msec(10);
}

/*void tankTurnMec(int speed, float degrees) {
	turnMec(speed);
	while (nMotorEncoder(FrontLeft) < 25.5 * PI * (degrees/360)){}
	Stop();
}*/

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

/*int centergoalPositionIR()
{
	// Create struct to hold sensor data
	tHTIRS2 irSeeker;

	// Initialise and configure struct and port
	initSensor(&irSeeker, S3);

	int position;
	float reading1;
	float reading2;
	irSeeker.mode = DSP_1200;

	readSensor(&irSeeker);
	reading1 = irSeeker.acDirection;
	wait1Msec(1000);
	mecMove(80, 0, 0, 50);
	wait1Msec(1000);
	readSensor(&irSeeker);
	reading2 = irSeeker.acDirection;
	wait1Msec(1000);
	//playSound(soundFastUpwardTones);
	//displayTextLine(2, "K: %d", reading2);
	nxtdisplayTextLine(3, "1: %d", reading1);
	nxtdisplayTextLine(4, "2: %d", reading2);
	wait1Msec(5000);
	/*if (reading1 == 5 && reading2 == 5)
	{
		nxtdisplayTextLine(3, "1: %d", reading1);
		nxtdisplayTextLine(4, "2: %d", reading2);
	//wait1Msec(5000);
		playSound(soundBeepBeep);
	wait1Msec(300);
		position = 1;
		displayTextLine(5, "pos: %d", position);
		//mecMove(80, -90, 0, 90);
		wait1Msec(5000);
	}
	else if (reading1 == 5 && (reading2 == 4 || reading2 == 3))
	{
		playSound(soundLowBuzz);
		wait1Msec(3000);
		position = 3;
		displayTextLine(5, "pos: %d", position);
		wait1Msec(5000);
	}
	else
	{
		playSound(soundFastUpwardTones);
		wait1Msec(3000);
		position = 2;
		displayTextLine(5, "pos: %d", position);
		wait1Msec(5000);
	}
	return position;
}*/

void moveTillUS(float speed, float degrees, float speedRotation, float threshold, bool till)//if till = true, move until sees something; if till = false, move until not seeing something
{
   mecJustMove(speed, degrees, speedRotation);
   if (till){
   	while ((USreadDist(USfront) > threshold) && (SensorValue(USback) > threshold)){}}
   else{
    	while ((USreadDist(USfront)) < threshold || (SensorValue(USback) < threshold)){}}//should be ||, so stop when none of them is in the threshold
		Stop();
}

void parallel()
{
	float difference=USreadDist(USfront) > readingB = SensorValue(USback)? 20:-20;//so that the sensors doesn't have to detect twice; to save batteries
	mecJustMove(0, 0, difference);
	while(abs(USreadDist(USfront)-SensorValue(USback))>5)
	{}
	Stop();
}

void moveTillTouch(float speed, float degrees, float speedRotation, bool till)
{
    mecJustMove(speed, degrees, speedRotation);
    if (till)
    {while ((!TSreadState(TOUCHFront)) && (!TSreadState(TOUCHBack))){}}
    else
    {while ((TSreadState(TOUCHFront)) || (TSreadState(TOUCHBack))){}}
		wait1Msec(500);
		Stop();
}

//--------------------Align stuffs-----------------------------------------------------------------------------------------------------------------------------------
void alignT(int speed, float degrees, float speedRotation)
//do we need the degrees and the speedrotation in the future?
{
	playSound(soundLowBuzzShort);
	nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
	wait1Msec(4000);
	eraseDisplay();
	int direction;
	int tempspeed;
	while (!(TSreadState(TOUCHFront))||!(TSreadState(TOUCHBack)))
	{
		nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
		direction = TSreadState(TOUCHFront)? 1:-1;//if only the front sensor is active, move forward
		tempspeed = speed*direction;
		mecJustMove(tempspeed, 0, 0);
		nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
	}
	Stop();
}

void alignUS(int speed, float degrees, float speedRotation, float threshold)//just in case that aligning with the touch sensors doesn't work
{
	playSound(soundLowBuzzShort);
	nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
	wait1Msec(4000);
	eraseDisplay();

	initUS();
	int direction;
	direction = TSreadState(TOUCHFront)? 1:-1;//if only the front sensor is active, move forward
	int tempspeed = speed * direction;
	moveTillUs(tempspeed, 0, 0, threshold, true);//threshold  = 50? might detect the kickstand though. and when it crashes into the center goal, one US might have sensed something
}

bool alignRecursiveT()//true = we are all set, false = nope not even touching now and need to realign
//don't know if RobotC allows me to do recursive or would the robot crash...?
//**the function can stop and alarm when it is not aligning anymore, which is better than alignT()
{
	nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));

	if (TSreadState(TOUCHFront) && TSreadState(TOUCHBack))// if both of them are touching
	{
		wait1Msec(1000);
		playSound(soundUpwardTones);
		return true;
	}

	if (TSreadState(TOUCHFront) || TSreadState(TOUCHBack))//run if at least one of them is touching, else... it is just unfortunate
	{
		short reading1 = TSreadState(TOUCHFront), reading2 = TSreadState(TOUCHBack);
		direction = TSreadState(TOUCHFront)? 1:-1;//if only the front sensor is active, move forward
		tempspeed = 20*direction;//positive speed = forward, negative = backward
		mecJustMove(tempspeed, 0, 0);
		while(TSreadState(TOUCHFront) == reading1 && TSreadState(TOUCHBack) == reading2)//stop if at least one of them is different from the beginning
		{
			nxtDisplayCenteredTextLine(2, "%d, %d", TSreadState(TOUCHfront), TSreadState(TOUCHback));
		}
		Stop();
		bool result = alignRecursiveT();
		result? playSound(soundUpwardTones) : playSound(soundDownwardTones);
		wait1Msec(700);
		return result;
	}
	playSound(soundDownwardTones);
	wait1Msec(1000);
	return false;
}

void adjustment()
{
	bool aligned;
//	do{
		 aligned = alignRecursiveT();
		 //do some adjustment stuffs? go off and find the location? go for the kickstand (since it shouldn't be that far off but yet we don't know exactly where the robot is)?
//	}while (!aligned);
	 kickstand();
}
//----------------Sequential Stuffs--------------------------------------------------------------------------------------------------------------------------------
void lift()
{

}

void kickstand()//starting from the bottom of the center goal
{
	mecMove(50, -90, 0, 10);//move away a little
	turnMecGyro(50, 160);
	parallel();
	moveTillUS(50, 0, 0, 30, false);//to make sure that it is lined up using US
	mecMove(20, 0, 0, 20);//make sure that the arm passes the kickstand
	turnMecGyro(50, -90);//eh can't use parallel
	mecMove(50, 0, 0, 30);//make sure that the arm passes the kickstand but the robot is still next to the kickstand
	armOut();
	mecMove(50, -90, 0, 10);//move sideway so the robot is touching the kickstand or at least reachable
	mecMove(-50, 0, 0, 50);//move backward to hook down the kickstand
}

//===================================================================================================================================
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
	int pos3 = 40; //should be 40
	servo[hood] = pos3;//hood in place

//----------------------------------------------------------------
	initUS();
	int Cposition;
	nxtDisplayCenteredTextLine(2, "%d", USreadDist(USfront));
	wait1Msec(3000);
	eraseDisplay();
	wait1Msec(2000);
	nxtDisplayCenteredTextLine(2, "%d", SensorValue(USback));
	wait1Msec(3000);
	eraseDisplay();


	if (USreadDist(USfront) > 250 && SensorValue(USback) > 250) {
		Cposition = 2;
		wait1Msec(1000);
		playSound(soundDownwardTones);
		wait1Msec(2000);
		}

	else if(USreadDist(USfront) > 105 && USreadDist(USfront) < 115){//the other value is... doesn't matter
		Cposition = 3;
		wait1Msec(1000);
		playSound(soundUpwardTones);
		wait1Msec(2000);
		}

	else{ //values between 128-134, but don't need it really
			Cposition = 1;
			wait1Msec(1000);
			playSound(soundBeepBeep);
			wait1Msec(2000);
		}

switch (Cposition)
	{
		case 1:{
			mecMove(70, 90, 0, 50);//move away from the wall
			//parallel();
			//mecMove(80, -180, 0, 60);
			moveTillUS(60, 0, 0, 70, false);//move until the center goal is out of the sight of both us
			turnMecGyro(80, 90);
			moveTillUS(60, 0, 0, 50, true);
			moveTillTouch(40, -90, 0, true);
			alignT(50, 0, 0);
		};
		break;
		case 2:{
			mecMove(50, 0, 0, 40);
			mecMove(50, 90, 0, 50);
			turnMecGyro(-80, 5);
			//moveTillUS(80, 0, 0, 50, true);
			moveTillTouch(50, 90, 0, true);
			wait1Msec(1000);
			alignT(50, 0, 0);
		};
		break;
		case 3:{
			mecMove(50, 0, 0, 20);
			moveTillTouch(30, 90, 0, true);
			wait1Msec(2000);
			alignRecursiveT();
			/*
			 * adjustment();
			 */
		}
		break;
	}
	lift();
//-----------------------------------------------------------------------------
}
