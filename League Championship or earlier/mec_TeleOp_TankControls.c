#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S3,     irSeeker,       sensorI2CCustom)
#pragma config(Sensor, S4,     HTSMUX,         sensorLowSpeed)
#pragma config(Motor,  motorA,          arm,           tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     BackLeft,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     FrontLeft,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     thrower,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     FrontRight,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     BackRight,     tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    liftRelease,          tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//version 1/21/15   Status: May's edition of tank drive added

#include "JoystickDriver.c"
#include "include\hitechnic-irseeker-v2.h"
#include "include\hitechnic-sensormux.h"
#include "include\lego-touch.h"
#include "include\lego-ultrasound.h"
#include "include\hitechnic-gyro.h"


//const tMUXSensor TOUCHFront = msensor_S4_3;
//const tMUXSensor TOUCHBack = msensor_S4_4;

//--------------------------------------------------------------------------------------

task main()
{
	static float encoderScale=1120.0;
	int pos1 = 255;
	int pos2 = 0;
	int countArm=0;
	servo[grabber1]=pos1;
	servo[grabber2]=pos2;

	int pos4 = 50;
	servo[hood] = pos4;

	int frontback = 1;

	servo[liftRelease] = 127;

	waitForStart();
    while(true){
	   getJoystickSettings(joystick);

/*	   //-TESTING TOUCH SENSORS-----//
	  if(TSreadState(TOUCHFront)==0|| TSreadState(TOUCHBack)==0){
		playSound(soundLowBuzz);
		wait1Msec(1000);
}
if(TSreadState(TOUCHFront)==0 && TSreadState(TOUCHBack)==0){
		playSound(soundDownwardTones);
	wait1Msec(1000);
		}
		*/


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
			/*int Rot = joystick.joy1_x2;//Right joystick X-axis
			int Fwd = joystick.joy1_y1;//-Left joystick Y-axis
			int Side= joystick.joy1_x1;// Left joystick X-axis
			motor[FrontLeft] = 100.0/384 * (-Fwd - Rot - Side);
			motor[FrontRight] = 100.0/384 * (-Fwd + Rot + Side);
			motor[BackLeft] = 100.0/384* (-Fwd - Rot + Side);
			motor[BackRight] = 100.0/384 * (-Fwd + Rot - Side);*/


			if(frontback==1){
			motor[FrontLeft] = (-100.0/256 * (-joystick.joy1_y1 - (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[BackLeft] = (-100.0/256 * (-joystick.joy1_y1 + (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[FrontRight] =  (-100.0/256 * (-joystick.joy1_y2 + (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[BackRight] = (-100.0/256 * (-joystick.joy1_y2 - (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			}

			else{
			motor[FrontRight] = frontback * (-100.0/256 * (-joystick.joy1_y1 - frontback * (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[BackRight] = frontback * (-100.0/256 * (-joystick.joy1_y1 + frontback * (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[FrontLeft] = frontback * (-100.0/256 * (-joystick.joy1_y2 + frontback * (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			motor[BackLeft] = frontback * (-100.0/256 * (-joystick.joy1_y2 - frontback * (joystick.joy1_x1 + joystick.joy1_x2) / 2));
			}


			wait1Msec(10);



//-Thrower------------------------------------------------------------------------------
	   if(joy1Btn(1)) {
	     motor[thrower] = -100;
	}

	   if(joy1Btn(3)) { //runs motor backwards
	     motor[thrower] = 0;
	   }

	   if(joy1Btn(5)) {
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
	   if (joy2Btn(5)){
	     /*if(countArm<4){
	       pos4=pos4;
	     }
	     else{*/
	       pos4 = 107;
	     //}
	     wait1Msec(10);
	     servo[hood] = pos4;
	   }

	   if (joy2Btn(7)){
	     /*if(countArm<4){
	       pos4=pos4;
	     }
	     else{*/
	       pos4 = 50;
	     //}
	     wait1Msec(10);
	     servo[hood] = pos4;
	   }
//-Kickstand arm---------------------------------------------------------------------------------------
	   if (joy1Btn(6)){//kickstand arm out
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

//---Lift--------------------------------------------------------------------------------------------------------
	   if(joy2Btn(1)){ //lift down
	     //countArm-=1;
	     nMotorEncoder[Lift]=0;
				motor[Lift]=25;
				servo[liftRelease] = 60; //down ratio of 25/(60-127) = -.373
	   		while(nMotorEncoder[Lift]<encoderScale)
	   		{
	   		}
	   		motor[Lift]=0;
	   		servo[liftRelease] = 127;
	   	}

	   if(joy2Btn(3)){ //lift up
	     //countArm+=1;
	     	nMotorEncoder[Lift]=0;
				motor[Lift]=-30;
				servo[liftRelease] = 143; //up ratio of -30/(143-127) =-1.875
	   		while(abs(nMotorEncoder[Lift])<encoderScale)
	   		{
	   			//wait1Msec(10);
	   		}
	   		motor[Lift]=0;
	   		servo[liftRelease] = 127;
	   	}

	   	if(joy2Btn(6)){
	   		servo[liftRelease] = 150;
	   		while(joy2Btn(6))
	   		{
	   		}
	   		servo[liftRelease] = 127;
	   	}

	   	if(joy2Btn(8)){ //tighten up the servo
	   		servo[liftRelease] = 90;
	   		while(joy2Btn(8))
	   		{
	   		}
	   		servo[liftRelease] = 127;
	   	}

//---Switch Front and Back---------------------------------------------------------------------------------------
		if (joy1Btn(2)){
	   	frontback = -1;
	   	}
		if (joy1Btn(4)){
	   	frontback = 1;
	   	}

     //wait1Msec(100);
  }
}
