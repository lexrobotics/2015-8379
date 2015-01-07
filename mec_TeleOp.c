#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     touch,          sensorTouch)
#pragma config(Sensor, S3,     light,          sensorLightActive)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     FrontLeft,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     FrontRight,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     BackLeft,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     BackRight,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 12/16/14   test waiting time changed(Btn 5&7)   hood angle changed 100 --> 110

#include "JoystickDriver.c"
task main()
{
	int pos1 = 255;
	int pos2 = 0;
//	int pos3 = 150;
	/*servo[grabber1]=pos1;
	servo[grabber2]=pos2; temporaroily commented out to test with 2bits robot*/
//	servo[arm] = pos3;

	int pos4 = 10;
//	servo[hood] = pos4;temporaroily commented out to test with 2bits robot

	waitForStart();
    while(true){
	   getJoystickSettings(joystick);

	   if(abs(joystick.joy1_y1)<10)
	   {
	    joystick.joy1_y1=0;
	   }
	   if(abs(joystick.joy1_y2)<10)
	   {
	    joystick.joy1_y2=0;
	   }
	   if(abs(joystick.joy1_x1)<10)
	   {
	    joystick.joy1_x1=0;
	   }
	   if(abs(joystick.joy1_x2)<10)
	   {
	    joystick.joy1_x2=0
	   }
	 /*  FrontLeft = Ch3 + Ch1 + Ch4
RearLeft = Ch3 + Ch1 - Ch4
FrontRight = Ch3 - Ch1 - Ch4
RearRight = Ch3 - Ch1 + Ch4

Where:*/
int Ch1 = joystick.joy1_x2;//Right joystick X-axis
int Ch3 = joystick.joy1_y1;//-Left joystick Y-axis
int Ch4 = joystick.joy1_x1;// Left joystick X-axis

	    motor[FrontLeft] = 50.0/384 * (-Ch3 + Ch1 + Ch4);
			motor[FrontRight] = 50.0/384 * (-Ch3 - Ch1 - Ch4);
			motor[BackLeft] = 50.0/384* (-Ch3 + Ch1 - Ch4);
			motor[BackRight] = 50.0/384 * (-Ch3 - Ch1 + Ch4);
		/*if(joy2Btn(1)) { //runs thrower
			motor[thrower] = -10 0.0;
	   }

	   if(joy2Btn(3)) { //stops thrower
			motor[thrower] = 0;
	   }

		if(joy2Btn(4)) { //moves grabbers up

			 	pos1+=5;
			 	pos2-=5;
			 	if (pos1 >= 255) pos1=255;
			 	if(pos2<=0) pos2=0;
	     	servo[grabber1] = (pos1);
	     	servo[grabber2] = (pos2);
	   }

	   if(joy2Btn(2)) { //moves grabbers down

	      pos1-=5;
	     	pos2+=5;
	     	if (pos1 <= 0) pos1=0;
			 	if(pos2>=255) pos2=255;
	     	servo[grabber1] = (pos1);
	     	servo[grabber2] = (pos2);

	    }


	   if (joy2Btn(5)){
	     if (pos4 >= 0 && pos4 <= 255)
	     {
	       pos4 +=5;//20
	       if (pos4 <= 0) pos4=0;
	     	 if (pos4 >=255) pos4=255;
	     	 servo[hood] = pos4;
	     	 wait1Msec(500);
	     }
	     pos4 = 110;
	     servo[hood] = pos4;
	   }

	   if (joy2Btn(7)){
     if (pos4 >= 0 && pos4 <= 255)
	     {
	       pos4 -=5;
	       if (pos4 <= 0) pos4=0;
	     	 if (pos4 >=255) pos4=255;
	     	 servo[hood] = pos4;
	     	 wait1Msec(500);
	     }
	     pos4 = 10;
	     servo[hood] = pos4;
	   }

	   if (joy2Btn(6))//runs thrower backward just in case a ball is stuck or something; up right side button
	     motor[thrower] = 50; temporaroily commented out to test with 2bits robot
	   */


     wait1Msec(10);
  }
}
