#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     touch,          sensorTouch)
#pragma config(Sensor, S3,     light,          sensorLightActive)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          arm,           tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     BackLeft,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     FrontLeft,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     thrower,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     FrontRight,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     BackRight,     tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 1/15/15   program more organized and kick-stand arm commands added
//joystick1 = primary, and drives and controls the kick stand arm    joystick2 = secondary, and controls everything else except for the kick stand arm
//!!!!!!!!!!!!!!!!!!!!!!wait what check the configurations... all sensors are named differently in each autonomous program!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//and why are touch and light sensors used here...?

#include "JoystickDriver.c"

//--------------------------------------------------------------------------------------

task main()
{
	int pos1 = 255;
	int pos2 = 0;
	servo[grabber1]=pos1;
	servo[grabber2]=pos2;

	int pos4 = 10;
	servo[hood] = pos4;
	//int armcounter = 0;

	//int throwerCount = 0;
	//int kArmCount = 0;
	//int hoodCount=0;

	waitForStart();
    while(true){
	   getJoystickSettings(joystick);

//-Movement--------------------------------------------------------------------------
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
	    joystick.joy1_x2=0;
	   }
	 /*  FrontLeft = Ch3 + Ch1 + Ch4
RearLeft = Ch3 + Ch1 - Ch4
FrontRight = Ch3 - Ch1 - Ch4
RearRight = Ch3 - Ch1 + Ch4

Where:*/
			int Rot = joystick.joy1_x2;//Right joystick X-axis
			int Fwd = joystick.joy1_y1;//-Left joystick Y-axis
			int Side= joystick.joy1_x1;// Left joystick X-axis
			motor[FrontLeft] = 100.0/384 * (-Fwd - Rot - Side);
			motor[FrontRight] = 100.0/384 * (-Fwd + Rot + Side);
			motor[BackLeft] = 100.0/384* (-Fwd - Rot + Side);
			motor[BackRight] = 100.0/384 * (-Fwd + Rot - Side);
			/*while(joystick.joy1_x2>=10 || joystick.joy1_y1>=10 || joystick.joy1_x1>=10)
			{
			}*/

			wait1Msec(10);



//-Thrower------------------------------------------------------------------------------

		/* if(joy2Btn(1)) { //runs thrower normal way and stop, left of the 4 buttons
				if ((throwerCount%2) == 0)
					motor[thrower] = -100.0;
				else
					motor[thrower] = 0;
			throwerCount++;
			wait1Msec(300);
	   }

	   if(joy2Btn(3)) { //runs thrower in reverse and stop, right of the 4 buttons
				if ((throwerCount%2) == 0)
					motor[thrower] = 50.0;
				else
					motor[thrower] = 0;
			throwerCount++;
			wait1Msec(300);
	   }*/

	   if(joy2Btn(1)) {
	     motor[thrower] = -100;
	}

	   if(joy2Btn(3)) { //runs motor backwards
	     motor[thrower] = 0;
	   }

	   if(joy2Btn(6)) {
	     motor[thrower] = 100;
	   }
//-Grabber--------------------------------------------------------------------------------
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
//-Hood---------------------------------------------------------------------------------------
	   /*if (joy2Btn(5)){//left side top button, move hood up and down
				if ((hoodCount%2) == 0)
					servo[hood]=110;
				else
					servo[hood]=10;
			hoodCount++;
	     wait1Msec(300);
	   }*/
	   if (joy2Btn(5)){
	     pos4 = 110;
	     servo[hood] = pos4;
	   }
	   if (joy2Btn(7)){
	     pos4 = 10;
	     servo[hood] = pos4;
	   }
//-Kickstand arm---------------------------------------------------------------------------------------
	   /*if (joy2Btn(6))//right side top button
	   {
	     nMotorEncoder[arm] = 0;
	     if (kArmCount%2==0)
	     {
	       while(nMotorEncoder[arm] < 150){
	     	 motor[arm] = -50;}
	     }
	     else
	     {
	       while(abs(nMotorEncoder[arm]) < 130){
	    	 motor[arm] = 50;}
	     }
	     kArmCount++;
	     motor[arm] = 0;
	     wait1Msec(300);

	   }*/
	   if (joy1Btn(6)){
	   		while(joy1Btn(6))
	   		{
	   			motor[arm] = 50;
	   			wait1Msec(10);
	   		}
	   		motor[arm]=0;
	   	}

	   if (joy1Btn(8)){
	   		while(joy1Btn(8))
	   		{
	   			motor[arm] = -50;
	   			wait1Msec(10);
	   		}
	   		motor[arm]=0;
	   	}
//------------------------------------------------------------------------------------------------
     //wait1Msec(100);
  }
}
