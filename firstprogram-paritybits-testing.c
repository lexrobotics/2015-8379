#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorRight,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorLeft,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     thrower,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
task main()
{
	int pos1 = 0;
	int pos2 = 255;
	servo[grabber1]=pos1;
	servo[grabber2]=pos2;


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
     motor[motorRight] = 100.0/128 * joystick.joy1_y1;//right = joy1_y1, the right button
     motor[motorLeft] = 100.0/128 * joystick.joy1_y2;

		if(joy1Btn(1)) { //runs thrower
			motor[thrower] = -100.0;
	   }

	   if(joy1Btn(3)) { //stops thrower
			motor[thrower] = 0;
	   }

		if(joy1Btn(2)) { //moves grabbers down

			 	pos1+=5;
			 	pos2-=5;
			 	if (pos1 >= 255) pos1=255;
			 	if(pos2<=0) pos2=0;
	     	servo[grabber1] = (pos1);
	     	servo[grabber2] = (pos2);
	   }

	   if(joy1Btn(4)) { //moves grabbers up
	     if(pos1 >= 0 && pos1 <= 255){
	      pos1-=5;
	     	pos2+=5;
	     	if (pos1 <= 0) pos1=0;
			 	if(pos2>=255) pos2=255;
	     	servo[grabber1] = (pos1);
	     	servo[grabber2] = (pos2);
	    }
	   }
     wait10Msec(1);
  }
}
