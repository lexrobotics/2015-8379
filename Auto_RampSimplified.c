#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S2,     gyro,           sensorAnalogInactive)
#pragma config(Motor,  mtr_S1_C1_1,     motorRight,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorLeft,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     thrower,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
//version 12/15/14

#include "JoystickDriver.c"
#include "include\hitechnic-gyro.h"

//everything is in centimeters
static float encoderScale=1440.0;
static float wheelRadius=((9.7)/2);
static float wheelCircumference=PI*2*wheelRadius;
//static float AngleRatio = ? //used as angle(in degree) * AngleRatio

void resetEncoders(){
	nMotorEncoder[motorRight] = 0;
	nMotorEncoder[motorLeft] = 0;
}

void move(float speed, float distance)
{
	resetEncoders();
	motor[motorLeft] = speed;
	motor[motorRight] = speed;
	while(abs(nMotorEncoder[motorLeft]) <= encoderScale * (distance / wheelCircumference) )
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

	move(50.0, 167.0);//**1st length: move down the ramp
	turnWithGyro(-100.0, 8.5);//1st turn: turn to face rolling goal
	ramp(50.0,112.0);//**2nd length: move toward the 30 cm goal

	wait10Msec(50);
	pos1=255;
	pos2=0;//grabber completely down
	servo[grabber1]=pos2;
	servo[grabber2]=pos1;
	wait10Msec(50);

	move(-100.0,10.0);//**back up 5 cm

	turnWithGyro(-100.0,135.0);//turn back to straight
	motor[thrower] = -60.0; //start thrower motor

	move(100.0,180.0);//**length: move pass the kick stand
	turnWithGyro(100.0,15.0);//turn toward the PK
	move(100.0, 80.0);//**length: move into the PK

	/******move to the right side of PZ**********************
	move(100.0, 75.0);//**length: move back
	turnWithGyro(100, 50.0);//turn towards parking zone
	move(100, 65.0);//**length: move right to the right side of the PZ
	turnWithGyro(-100, 60.0);//turn towards right side of parking zone
	move(100,10.0);//**length: move into the PZ
	**********************************************************/

	wait1Msec(10000);

}
