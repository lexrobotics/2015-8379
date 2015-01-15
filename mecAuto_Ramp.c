#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorAnalogInactive)
#pragma config(Motor,  motorA,          arm,           tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     FrontRight,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackRight,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Flipper,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     BackLeft,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     FrontLeft,     tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 12/17/14 total time function added

#include "JoystickDriver.c"
#include "include\hitechnic-gyro.h"

//everything is in centimeters
static float encoderScale=1440.0;
static float wheelRadius=((9.7)/2);
static float wheelCircumference=PI*2*wheelRadius;
//static float AngleRatio = ? //used as angle(in degree) * AngleRatio

void resetEncoders(){
	nMotorEncoder[FrontLeft] = 0;
	nMotorEncoder[FrontRight] = 0;
	nMotorEncoder[BackLeft] = 0;
	nMotorEncoder[BackRight] = 0;
}

void mecMove(float speed, float degrees, float speedRotation, float distance)
{ //should be speed [-100,100], degrees [0, 360], speedRotation [-100,100], distance cm, but it's not
		playSound(soundBeepBeep);
		resetEncoders();
		float min = 0.0;


		if(cosDegrees(degrees) == 0 || sinDegrees(degrees) == 0)
			{
				min = 1;
			}
		if (abs(1/cosDegrees(degrees))<= abs(1/sinDegrees(degrees)))
			{
				min = 1/cosDegrees(degrees);
			}
		else
			{
				min = 1/sinDegrees(degrees);
			}

		if(cosDegrees(degrees) == 0 || sinDegrees(degrees) == 0)
			{
				min = 1;
			}
		float scaled = encoderScale* (distance * min / wheelCircumference);

		motor[FrontLeft] = speed * sinDegrees(degrees + 45) + speedRotation;
		motor[FrontRight] = speed * cosDegrees(degrees + 45) - speedRotation;
		motor[BackLeft] = speed * cosDegrees(degrees + 45) + speedRotation;
		motor[BackRight] = speed * sinDegrees(degrees + 45) -  speedRotation;
		while((nMotorEncoder[FrontLeft]<scaled) && (nMotorEncoder[FrontRight]<scaled) && (nMotorEncoder[BackLeft]< scaled) && (nMotorEncoder[BackRight]< scaled))
		{
			wait1Msec(10);
  	}
		motor[BackLeft] = 0;
		motor[BackRight] = 0;
		motor[FrontLeft] = 0;
		motor[FrontRight] = 0;
		resetEncoders();
}

/*void tankTurnMec(int speed, float degrees) {
	while (nMotorEncoder(FrontLeft) < 25.5 * PI * (degrees/360)){
	motor[BackRight] = speed;
	motor[FrontRight] = speed;
	motor[FrontLeft] = 0-speed;
	motor[BackLeft] = 0-speed;
}
	motor[BackRight] = 0;
	motor[FrontRight] = 0;
	motor[FrontLeft] = 0;
	motor[BackLeft] = 0;
}*/

/*void turnMec(float speed){//+ = turn right     - = turn left
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
}*/

void armOut(){
	nMotorEncoder[arm] = 0;
	while(nMotorEncoder[arm] < 90){
    motor[arm] = 50;
	 }
	 motor[arm] = 0;
}

void armIn(){
	nMotorEncoder[arm] = 0;
	while(nMotorEncoder[arm] < 90){
    motor[arm] = -50;
	 }
	 motor[arm] = 0;
}

/*void move(float speed, float distance)
{
	resetEncoders();
	motor[motorLeft] = speed;
	motor[motorRight] = speed;
	while(abs(nMotorEncoder[motorLeft]) <= encoderScale * (distance / wheelCircumference) )//min(abs(sec theta), abs(csc theta)
	{
	}
	motor[motorLeft] = 0;
	motor[motorRight] = 0;
	resetEncoders();
}


void turn(float speed){//+ = turn right     - = turn left
	motor[motorLeft] = speed;
	motor[motorRight] = 0-speed;
}

void turnWithGyro(int speed, float degrees) {
	float delTime = 0;
	float curRate = 0;
	float currHeading = 0;
	motor [motorLeft]=0;
	motor [motorRight]=0;
	wait1Msec(500);
  HTGYROstartCal(gyro);
 	wait1Msec(500);
 	PlaySound(soundBeepBeep);
 	turn (speed);//+ = right   - = turn left
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
  turn(0);
}

void ramp(float speed, float distance)
{
	while(abs(nMotorEncoder[motorLeft]) <= encoderScale * (distance / wheelCircumference) )
	{
			motor[motorLeft] = speed *( (encoderScale * distance / wheelCircumference- nMotorEncoder[motorLeft]) / ( encoderScale * distance / wheelCircumference));
			motor[motorRight] = speed *( (encoderScale * distance / wheelCircumference - nMotorEncoder[motorRight]) / ( encoderScale * distance / wheelCircumference));
	}

}*/

task main()
{
	playSound(soundBeepBeep);
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

	//armOut();
	mecMove(70, 0, 0, 855);
	wait1Msec(10);
	playSound(soundDownwardTones);
	//mecMove(100, 310, 0, 150);
	//armIn();

	/*
	while(nMotorEncoder[arm]<90)
	{
		motor[arm]=100;
	}
	motor[arm]=0;
	*/

	/*int pos1 = 0;
	int pos2 = 255;
	servo[grabber1]=pos2;//they are switched
	servo[grabber2]=pos1;

	int pos3 = 10;
	servo[hood] = pos3;

	time1[T2]=0;

	move(.5, 170.0, .5);//**1st length: move down the ramp
	turnWithGyro(-100.0, 72.0);//1st turn: turn to face rolling goal
	move(100.0, 42.0);
	turnWithGyro(100.0, 67.0);
	move(50.0,120.0);//**2nd length: move toward the 30 cm goal possibly to be ramped?

	wait1Msec(250);
	pos1=255;
	pos2=0;//grabber completely down
	servo[grabber1]=pos2;
	servo[grabber2]=pos1;
	wait1Msec(500);

	move(-100.0,10.0);//**back up 5 cm
	wait1Msec(100);
	move(100.0,10.0);
	wait1Msec(100);
	move(-100.0,10.0);

	pos3 = 115;
	servo[hood] = pos3;//hood in place

	turnWithGyro(-100.0,165.0);//turn back to straight
	motor[thrower] = -100.0; //start thrower motor

	move(100.0,180.0);//**length: move pass the kick stand
	turnWithGyro(100.0,20.0);//turn toward the PK
	move(100.0,60.0);*/

	/******move to the right side of PZ**********************
	move(100.0, 130.0);//**length: move back
	turnWithGyro(100, 50.0);//turn towards parking zone
	move(100, 120.0);//**length: move right to the right side of the PZ
	turnWithGyro(-100, 60.0);//turn towards right side of parking zone
	move(100,20.0);//**length: move into the PZ
	**********************************************************/
	/*float totaltime = (float)Time1[T2]/1000.0;
	wait1Msec((30.0-totaltime-(float)delay)*1000.0);
	pos3 = 10;
	servo[hood] = pos3;//to prevent the hood from falling onto the tube when the program stops
	wait1Msec(500);
	*/

}
