#pragma config(Sensor, in2,    gyro,           sensorGyro)
#pragma config(Sensor, in8,    pot,            sensorPotentiometer)
#pragma config(Sensor, dgtl1,  dig1,           sensorDigitalIn)
#pragma config(Sensor, dgtl2,  dig2,           sensorDigitalIn)
#pragma config(Motor,  port1,           liftLeft2,     tmotorVex393, openLoop)
#pragma config(Motor,  port2,           liftLeft,     tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port3,           spinMotor2,    tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port4,           backRight,     tmotorVex393, openLoop)
#pragma config(Motor,  port5,           backLeft,      tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port6,           liftRight,     tmotorVex393, openLoop)
#pragma config(Motor,  port7,           frontLeft,      tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port8,           spinMotor,     tmotorVex393, openLoop)
#pragma config(Motor,  port9,           frontRight,    tmotorVex393, openLoop)
#pragma config(Motor,  port10,          liftRight2,    tmotorVex393, openLoop)

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

void pre_auton()
{
}

task autonomous()
{

}

task usercontrol()
{
	while (true)
	{
	  motor[frontRight] = motor[backRight] = -vexRT[Ch2];
    motor[frontLeft] = motor[backLeft] = -vexRT[Ch3];
    motor[liftLeft] = motor[liftRight] = motor[liftLeft2] = motor[liftRight2] = (vexRT[Btn6D] - vexRT[Btn6U]) * 127;
    motor[spinMotor] = motor[spinMotor2] = (vexRT[Btn5U] - vexRT[Btn5D]) * 127;
	}
}
