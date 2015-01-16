#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S3,     irSeeker,       sensorSONAR)
#pragma config(Sensor, S4,     ,               sensorSONAR)
#pragma config(Motor,  motorA,          arm,           tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     FrontRight,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     BackRight,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Flipper,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     BackLeft,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     FrontLeft,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 1/15/15 tested out for Meet 3, adjusted with mecanum wheels

#include "JoystickDriver.c"
#include "hitechnic-irseeker-v2.h"

#include "include\hitechnic-gyro.h"

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
{ //speed [-100,100], degrees [0, 360], speedRotation [-100,100], distance cm
		playSound(soundBeepBeep);
		resetEncoders();
		float min = 0.0;
		if (cosDegrees(degrees) == 0 || sinDegrees(degrees) == 0)
			{
				min = 1;
			}
		else if (abs(1/cosDegrees(degrees))<= abs(1/sinDegrees(degrees)))
			{
				min = 1/cosDegrees(degrees);
			}
		else
			{
				min = 1/sinDegrees(degrees);
			}

		float scaled = encoderScale* (distance * min / wheelCircumference);

		motor[FrontLeft] = speed * sinDegrees(degrees + 45) + speedRotation;
		motor[FrontRight] = speed * cosDegrees(degrees + 45) - speedRotation;
		motor[BackLeft] = speed * cosDegrees(degrees + 45) + speedRotation;
		motor[BackRight] = speed * sinDegrees(degrees + 45) -  speedRotation;
		while((nMotorEncoder[FrontLeft]<scaled) && (nMotorEncoder[FrontRight]<scaled) && (nMotorEncoder[BackLeft]< scaled) && (nMotorEncoder[BackRight]< scaled))
		{
			//wait1Msec(10);
  	}
		motor[BackLeft] = 0;
		motor[BackRight] = 0;
		motor[FrontLeft] = 0;
		motor[FrontRight] = 0;
		resetEncoders();
		wait1Msec(10);
}
void tankTurnMec(int speed, float degrees) {
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
}

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

void centergoalPositionIR()
{
  // Create struct to hold sensor data
  tHTIRS2 irSeeker;

  // Initialise and configure struct and port
  initSensor(&irSeeker, S2);

  if (true)
  {
  	int position;
  	float reading1;
  	float reading2;
  	irSeeker.mode = DSP_1200;

  	readSensor(&irSeeker);
  	reading1 = irSeeker.acDirection;
  	wait1Msec(1000);
  	mecMove(100, 0, 0, 50);

  	readSensor(&irSeeker);
  	reading2 = irSeeker.acDirection;
  	//playSound(soundFastUpwardTones);
  	//displayTextLine(2, "K: %d", reading2);
  	//displayTextLine(3, "1: %d", reading1);
  	//displayTextLine(4, "2: %d", reading2);
  	wait1Msec(5000);
  	if (reading1 == 5 && reading2 == 5)
  	{
  		playSound(soundBeepBeep);
  		wait1Msec(3000);
  	 	position = 1;
  	 	displayTextLine(5, "pos: %d", position);
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

  /* switch(position)
   {
     case 1: playSound(soundBeepBeep);
     wait1Msec(2000);
     displayTextLine(1, "at: %d", position);
     break;
     case 2: playSound(soundFastUpwardTones);
     wait1Msec(2000);
     displayTextLine(1, "at: %d", position);
     break;
     case 3: playSound(soundLowBuzz);
     wait1Msec(2000);
     displayTextLine(1, "at: %d", position);
     break;
   }*/
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

	int pos3 = 10;
	servo[hood] = pos3;

	time1[T2]=0;

	centergoalPositionIR();
	armOut();
	mecMove(100, 35, 0, 50);
	armIn();


	/*move(100.0, 15.0); //**1st length: move away from the wall

	turnWithGyro(100, 3.5);// 1st turn: turn in parking zone
	move(100.0,139.0);//**2nd length: move toward the ramp

	int pos1 = 0;//grabbers up
	int pos2 = 255;//grabbers up
	servo[grabber1]=pos2;//grabbers and pos switched
	servo[grabber2]=pos1;
	motor[motorLeft] = 0;
	motor[motorRight] = 0;



	turnWithGyro(-100, 5.5);//2nd turn: turn towards goal
	move(100.0,128.0);//**3rd length: move toward the goal, may want to ramp
	move(60.0, 28.0);

	wait1Msec(200);
	pos1=255;
	pos2=0;//grabber down
	servo[grabber1]=pos2;
	servo[grabber2]=pos1;
	wait1Msec(500);

	move(-100.0,10.0);// **length: back away a little
	move(100.0, 10.0);
	move(-100.0, 10.0);

	pos3 = 120;
	servo[hood] = pos3;//hood in place

	turnWithGyro(-100,165.0);//turn back to return
	motor[thrower]=-100.0;
	move(100.0,170.0);//**length: move pass the kick stand
	turnWithGyro(100.0,26.0);//turn toward the PK
	move(100.0, 70.0);*/

	float totaltime = (float)Time1[T2]/1000.0;
	wait1Msec((30.0-totaltime-(float)delay)*1000.0);
	pos3 = 10;
	servo[hood] = pos3;//to prevent the hood from falling onto the tube when the program stops
	wait1Msec(500);

}
