#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorAnalogInactive)
#pragma config(Motor,  mtr_S1_C1_1,     motorRight,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     thrower,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorLeft,     tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C2_1,    grabber1,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    grabber2,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    hood,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
//version 12/16/14   test waiting time changed(Btn 5&7)   hood angle changed 100 --> 110

#include "JoystickDriver.c"
task main()
{
	int pos1 = 255;
	int pos2 = 0;
//	int pos3 = 150;
	servo[grabber1]=pos1;
	servo[grabber2]=pos2;
//	servo[arm] = pos3;

	int pos4 = 10;
	servo[hood] = pos4;

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

	  /* if (abs(joystick.joy1_y1)>=10 && abs (joystick.joy1_y1) <50)
	   {
	     motor[motorRight] = 50.0/128 * joystick.joy1_y1;//right = joy1_y1, the right button
	   }
	   else
	   {*/
	      motor[motorRight] = 100.0/128 * joystick.joy1_y1;//right = joy1_y1, the right button
	   /*}

	   if (abs(joystick.joy1_y2)>=10 && abs (joystick.joy1_y2) <50)
	   {
	     motor[motorLeft] = 50.0/128 * joystick.joy1_y2;//left = joy1_y2, the left button
	   }
	   else
	   {*/
	      motor[motorLeft] = 100.0/128 * joystick.joy1_y2;//left = joy1_y2, the left button
	   //}

		if(joy2Btn(1)) { //runs thrower
			motor[thrower] = -100.0;
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

/*	    if (joy2Btn(5)){// up left move arm up; up left side button
				if(pos3 >= 0 && pos3 <= 150){
	      pos3+=5;
	     	if (pos3 <= 0) pos3=0;
	     	if(pos3>=150) pos3=150;
	     	servo[arm] = (pos3);
	     	wait1Msec(5);
	    	}
	    }

	    if (joy2Btn(7)){ //down left move arm down; bottom left side button
				if(pos3 >= 0 && pos3 <= 150){
	      pos3-=5;
	     	if (pos3 <= 0) pos3=0;
	     	if (pos3 >=150) pos3=150;
	     	servo[arm] = (pos3);
	     	wait1Msec(5);
	    }
	   }*/
	   if (joy2Btn(5)){
/*	     if (pos4 >= 0 && pos4 <= 255)
	     {
	       pos4 +=5;//20
	       if (pos4 <= 0) pos4=0;
	     	 if (pos4 >=255) pos4=255;
	     	 servo[hood] = pos4;
	     	 wait1Msec(500);
	     }*/
	     pos4 = 110;
	     servo[hood] = pos4;
	   }

	   if (joy2Btn(7)){
     /*if (pos4 >= 0 && pos4 <= 255)
	     {
	       pos4 -=5;
	       if (pos4 <= 0) pos4=0;
	     	 if (pos4 >=255) pos4=255;
	     	 servo[hood] = pos4;
	     	 wait1Msec(500);
	     }*/
	     pos4 = 10;
	     servo[hood] = pos4;
	   }

	   if (joy2Btn(6))//runs thrower backward just in case a ball is stuck or something; up right side button
	     motor[thrower] = 50;


     wait1Msec(10);
  }
}