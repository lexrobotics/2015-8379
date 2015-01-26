#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S3,     irSeeker,       sensorI2CCustom)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Motor,  motorA,          arm,           tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     FrontRight,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackRight,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Flipper,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     BackLeft,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     FrontLeft,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 1/25/15  
//authot: Eula
//status: detecting center goal position using parallelity with double ultrasonic sensor, mecJustMove, moveTillUs, and moveTillTouch function added, align function modified


#include "JoystickDriver.c"
#include "hitechnic-irseeker-v2.h"
#include "include\hitechnic-sensormux.h"
#include "include\lego-touch.h"
#include "include\hitechnic-gyro.h"

const tMUXSensor USback = msensor_S4_1;
const tMUXSensor USfront = msensor_S4_2;
const tMUXSensor TOUCHFront = msensor_S4_3;
const tMUXSensor TOUCHBack = msensor_S4_4;

//everything is in centimeters
static float encoderScale=1440.0;
static float wheelRadius=((9.7)/2);
static float wheelCircumference=PI*2*wheelRadius;


void resetEncoders(){
	nMotorEncoder[FrontLeft] = 0;
	nMotorEncoder[FrontRight] = 0;
	nMotorEncoder[BackLeft] = 0;
	nMotorEncoder[BackRight] = 0;
}

void mecMove(float speed, float degrees, float speedRotation, float distance)
{ //speed [-100,100], degrees [0, 360] to the right, speedRotation [-100,100], distance cm
	playSound(soundBeepBeep);
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

	/*motor[FrontLeft] = speed * sinDegrees(degrees + 45) + speedRotation;
	motor[FrontRight] = speed * cosDegrees(degrees + 45) - speedRotation;
	motor[BackLeft] = speed * cosDegrees(degrees + 45) + speedRotation;
	motor[BackRight] = speed * sinDegrees(degrees + 45) -  speedRotation;*/
	mecJustMove(speed, degrees, speedRotation)
	while((nMotorEncoder[FrontLeft]<scaled) && (nMotorEncoder[FrontRight]<scaled) && (nMotorEncoder[BackLeft]< scaled) && (nMotorEncoder[BackRight]< scaled))
	{
	}
	motor[BackLeft] = 0;
	motor[BackRight] = 0;
	motor[FrontLeft] = 0;
	motor[FrontRight] = 0;
	resetEncoders();
	wait1Msec(10);
}

void mecJustMove(int speed, float degrees, float speedRotation)
{
	motor[FrontLeft] = speed * sinDegrees(degrees + 45) + speedRotation;
	motor[FrontRight] = speed * cosDegrees(degrees + 45) - speedRotation;
	motor[BackLeft] = speed * cosDegrees(degrees + 45) + speedRotation;
	motor[BackRight] = speed * sinDegrees(degrees + 45) -  speedRotation;
}


void align(int speed, float degrees, float speedRotation)//if only the front sensor is active, move forward
{
	int direction = TSreadState(TOUCHFront)? 1:-1;
	speed *= direction;
	mecJustMove(speed, degrees, speedRotation);
	while (!(TSreadState(TOUCHFront))&&!(TSreadState(TOUCHBack)))
	{
	}
	motor[BackLeft] = 0;
	motor[BackRight] = 0;
	motor[FrontLeft] = 0;
	motor[FrontRight] = 0;

}

void tankTurnMec(int speed, float degrees) {
	motor[BackRight] = speed;
	motor[FrontRight] = speed;
	motor[FrontLeft] = 0-speed;
	motor[BackLeft] = 0-speed;
	while (nMotorEncoder(FrontLeft) < 25.5 * PI * (degrees/360)){}
	motor[BackRight] = 0;
	motor[FrontRight] = 0;
	motor[FrontLeft] = 0;
	motor[BackLeft] = 0;
}

void turnMec(float speed){//+ = turn right     - = turn left
	motor[BackRight] = speed;
	motor[FrontRight] = speed;
	motor[FrontLeft] = 0-speed;
	motor[BackLeft] = 0-speed;
}

void turnMecGyro(int speed, float degrees) {
	float delTime = 0;
	float curRate = 0;
	float currHeading = 0;
	motor [BackLeft]=0;
	motor [BackRight]=0;
	motor [BackLeft]=0;
	motor [BackRight]=0;
	wait1Msec(500);
	HTGYROstartCal(gyro);
	wait1Msec(500);
	PlaySound(soundBeepBeep);
	turnMec (speed);//+ = right   - = turn left
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
	turnMec(0);
}

void armOut(){
	time1[T3]=0;
	while((float)time1[T3]<2000){
		motor[arm] = 50;
	}
	motor[arm] = 0;
}

void armIn(){
	time1[T3]=0;
	while((float)time1[T3] < 2000){
		motor[arm] = -50;
	}
	motor[arm] = 0;
}

int centergoalPositionIR()
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
	}*/
	return position;
}

void moveTillUS(float speed, float degrees, float speedRotation, float threshold, boolean till)//if till = true, move until sees something; if till = false, move until not seeing something
{
     mecJustMove(speed, degrees, speedRotation);
     if (till){
     	while (TSreadState(USfront) > threshold && TSreadState(USback) > threshold){}}
     else 
      	while (TSreadState(USfront) < threshold || TSreadState(USback) < threshold){}}
    motor[BackRight] = 0;
	motor[FrontRight] = 0;
	motor[FrontLeft] = 0;
	motor[BackLeft] = 0;
}

void moveTillTouch(float speed, float degrees, float speedRotation, boolean till)
{
    mecJustMove(speed, degrees, speedRotation);
    if (till)
    {while (!TSreadState(TOUCHfront) && !TSreadState(TOUCHfront)){}}
    else
    {while (TSreadState(TOUCHfront) || TSreadState(TOUCHfront)){}}
	wait1Msec(500);
    motor[BackRight] = 0;
	motor[FrontRight] = 0;
	motor[FrontLeft] = 0;
	motor[BackLeft] = 0;
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
	int pos3 = 40;
	servo[hood] = pos3;//hood in place
//----------------------------------------------------------------	
	int Cposition;

	if (abs(TSreadState(USfront) - TSreadState(USback)) < 10 && TSreadState(USfront) < 150)
		Cposition = 1;
	else if (abs(TSreadState(USfront) - TSreadState(USback)) < 70 && TSreadState(USfront) < 150)
		Cposition  = 2;
	else 
		Cposition = 3;
	
	switch (Cposition)
	{
		case 1:{
			mecMove(70, 90, 0, 20);//move away from the wall
			moveTillUS(60, 0, 0, 70, false);//move until the center goal is out of the sight of both us
			turnMecGyro(80, 90);
			moveTillUS(60, 0, 0, 50, true);
			moveTillTouch(40, -90, 0, true);
			align(50, 0, 0);
		};
		break;
		case 2:{
			mecMove(50, 90, 0, 20);
			turnMecGyro(80, 45);
			moveTillUS(80, 0, 0, 50, true);
			moveTillTouch(50, 90, 0, true);
			align(50, 0, 0);	
		};
		break;
		case 3:{ 
			moveTillTouch(50, 90, 0, true);
			align(50,0,0);
		} 
		break;
	}
	//and then do the lifting
//-----------------------------------------------------------------------------
}
