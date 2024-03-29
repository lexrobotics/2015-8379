#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     gyro,           sensorAnalogInactive)
#pragma config(Sensor, S3,     IRSeeker,       sensorNone)
#pragma config(Motor,  motorA,          arm,           tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     FrontRight,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackRight,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     thrower,       tmotorTetrix, openLoop)
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

//version 1/15/15 tested out for Meet 3, adjusted with mecanum wheels
//!!!!!!!!!!!!!!!!!!!!!!wait what check the configurations... all sensors are named differently in each autonomous program!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

	time1[T2]=0;

	move(100.0, 15.0); //**1st length: move away from the wall

	turnWithGyro(-100, 3.5);// 1st turn: turn in parking zone
	move(100.0,139.0);//**2nd length: move towards opposite ramp

	int pos1 = 0;
	int pos2 = 255;
	servo[grabber1]=pos2;//grabbers and pos switched
	servo[grabber2]=pos1;
	motor[motorLeft] = 0;
	motor[motorRight] = 0;

	int pos3 = 10;
	servo[hood] = pos3;//initialize the hood for 30 cm*/
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
	time1[T2]=0;

	int pos3 = 10;
	servo[hood] = pos3;

	time1[T2]=0;
	move(100.0, 15.0); //**1st length: move away from the wall

	turnWithGyro(-100, 3.0);// 1st turn: turn in parking zone
	move(100.0,135.0);//**2nd length: move toward the ramp

	int pos1 = 0;//grabbers up
	int pos2 = 255;//grabbers up
	servo[grabber1]=pos2;//grabbers and pos switched
	servo[grabber2]=pos1;


	turnWithGyro(100, 40.0);//2nd turn: turn toward opposite parking zone (should be parallel)
	move(100.0,103.0);//**3rd length: move past center goal on left
	turnWithGyro(100, 28.0);//3rd turn: turn right towards right wall
	move(100.0, 96.0);//4th length: move past center goal
	turnWithGyro(-100, 75.0);//4th turn: turn left toward goal
	move(100.0, 60.0);//5th lenghth: move toards goal

	wait1Msec(50);
	pos1=255;
	pos2=0;//grabber down
	servo[grabber1]=pos2;
	servo[grabber2]=pos1;
	wait1Msec(300);

	move(-100.0,10.0);// **length: back away a little
	//move(100.0, 10.0);
	//move(-100.0, 10.0);

	pos3 = 120;
	servo[hood] = pos3;//hood in place
	motor[thrower]=-100;
	move(-100.0, 45.0);//back out
	turnWithGyro(-100, 80.0);//turn, parallel to other parking zone
	move(100.0, 120.0);//move past the other parking zone
	turnWithGyro(-100, 60.0);//turn left
	move(100.0, 90.0);
	turnWithGyro(-100, 30.0);//turn left towards goal
	move(100.0,70.0);//turn left into parking zone

	/*turnWithGyro(-100,172.0);//turn back to return
	motor[thrower] = -100.0; //start thrower motor
	move(100.0, 155.0);//**length: move back
	turnWithGyro(100, 50.0);//turn towards parking zone
	move(100, 130.0);//**length: move right to the right side of the PZ
	turnWithGyro(-100, 60.0);//turn towards right side of parking zone
	move(100,20.0);//**length: move into the PZ
`*/

	float totaltime = (float)Time1[T2]/1000.0;
	wait1Msec((30.0-totaltime-(float)delay)*1000.0);
	pos3 = 10;
	servo[hood] = pos3;//to prevent the hood from falling onto the tube when the program stops
	wait1Msec(500);
}
