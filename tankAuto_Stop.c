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
#pragma config(Servo,  srvo_S1_C3_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    USfrontserv,          tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    liftRelease,          tServoStandard)
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
	nMotorEncoder[BackLeft] = 0;
	nMotorEncoder[FrontRight] = 0;
	nMotorEncoder[BackRight] = 0;
}

void move(float speed, float distance)
{
	resetEncoders();
	motor[FrontLeft] = speed;
	motor[FrontRight]= speed;
	motor[BackLeft]= speed;
	motor[BackRight] = speed;
	while(abs(nMotorEncoder[FrontRight]) <= encoderScale * (distance / wheelCircumference) )
	{
	}
	motor[FrontLeft] = 0;
	motor[FrontRight] = 0;
	motor[BackLeft] = 0;
	motor[BackRight] = 0;
	resetEncoders();
}


void turn(float speed){//+ = turn right     - = turn left
	motor[FrontLeft] = speed;
	motor[FrontRight]= -speed;
	motor[BackLeft]= speed;
	motor[BackRight] = -speed;
}

void turnWithGyro(int speed, float degrees) {
	float delTime = 0;
	float curRate = 0;
	float currHeading = 0;
	motor[FrontLeft] = 0;
	motor[FrontRight] = 0;
	motor[BackLeft] = 0;
	motor[BackRight] = 0;
	wait1Msec(500);
  HTGYROstartCal(gyro);
 	wait1Msec(500);
 	playSound(soundBeepBeep);
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

	int pos1 = 0;
	int pos2 = 255;
	servo[grabber1]=pos2;//they are switched
	servo[grabber2]=pos1;

	move(50.0, 170.0);

	wait1Msec(500);

}
