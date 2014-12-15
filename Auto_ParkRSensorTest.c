//version 12/11/14   hardware configs not rewired   sensor stuffs line 81 and 85   move function not *2
#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S2,     gyro,           sensorAnalogInactive)
#pragma config(Sensor, S3,     front,          sensorSONAR)
#pragma config(Sensor, S4,     back,           sensorSONAR)
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
	wait1Msec(300);
  HTGYROstartCal(gyro);
 	wait1Msec(300);
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
/*
//everything is in centimeters
static float encoderScale=1440.0;
static float wheelRadius=((9.7)/2);
static float wheelCircumference=PI*2*wheelRadius;
//static float AngleRatio = ? //used as angle(in degree) * AngleRatio
*/
function parallel(int speed){
}
task main()
{
	SensorValue[front];



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

	move(100.0, 20.0); //**1st length: move away from the wall

	turnWithGyro(100, 3.2);// 1st turn: turn in parking zone
	move(100.0,132.0);//**2nd length: move toward the ramp

	int pos1 = 0;
	int pos2 = 255;
	servo[grabber1]=pos2;//grabbers and pos switched
	servo[grabber2]=pos1;
	motor[motorLeft] = 0;
	motor[motorRight] = 0;

	turnWithGyro(-100, 10.0);//2nd turn: turn towards goal
	move(100.0,128.0);//**3rd length: move toward the goal, may want to ramp
	move(60.0, 28.0);

	wait10Msec(50);
	pos1=255;
	pos2=0;//grabber down
	servo[grabber1]=pos2;
	servo[grabber2]=pos1;
	wait10Msec(50);

	move(-100.0,10.0);// **length: back away a little
	turnWithGyro(-100,172.0);//turn back to return
	motor[thrower] = -100.0; //start thrower motor
	move(100.0, 150.0);//**length: move back
	turnWithGyro(100, 50.0);//turn towards parking zone
	move(100, 130.0);//**length: move right to the right side of the PZ
	turnWithGyro(-100, 60.0);//turn towards right side of parking zone
	move(100,20.0);//**length: move into the PZ

	wait1Msec(10000);

}
