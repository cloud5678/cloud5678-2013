#pragma config(Sensor, in2,    gyro,          sensorGyro)
#pragma config(Sensor, in8,    pot,           sensorPotentiometer)
#pragma config(Sensor, dgtl1,  dig1,          sensorDigitalIn)
#pragma config(Sensor, dgtl2,  dig2,          sensorDigitalIn)
#pragma config(Motor,  port1,  liftLeft2,     tmotorVex393, openLoop)
#pragma config(Motor,  port2,  liftLeft,      tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port3,  spinMotor2,    tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port4,  backRight,     tmotorVex393, openLoop)
#pragma config(Motor,  port5,  backLeft,      tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port6,  liftRight,     tmotorVex393, openLoop)
#pragma config(Motor,  port7,  frontLeft,     tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port8,  spinMotor,     tmotorVex393, openLoop)
#pragma config(Motor,  port9,  frontRight,    tmotorVex393, openLoop)
#pragma config(Motor,  port10, liftRight2,    tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(125)

#include "Vex_Competition_Includes.c"
//#include "\Sounds\Sounds.c"

void warningKiller() {
  UserControlCodePlaceholderForTesting();
  AutonomousCodePlaceholderForTesting();
}

void doNothing(int a) {
  return;
}
const int maxArm = 1700;
const int highArm = 1700; //Must get to this SLOWLY
const int middleArm = 1025;
const int lowArm = 0;
volatile armSetpoint = lowArm;

void lift(int liftPower) {
    liftPower = -liftPower;
    motor[liftLeft] = motor[liftRight] = liftPower;
	  motor[liftLeft2] = motor[liftRight2] = liftPower;
}

void spin(int spinPower) {
	int localSpinPower = spinPower;
	motor[spinMotor] = motor[spinMotor2] = localSpinPower;
}

void drive(int drivePower, int turn){

  // Exp. Drive Code goes here
  // Consider using scale of 0 to 1

  /*float tF = drivePower == 128 ? 1 : drivePower / 127;
  float tT = drivePower == 128 ? 1 : drivePower / 127;

  float fF = 127 * tF * tF * (drivePower < 0 ? -1 : 1);
  float fT = 127 * tT * tT * (turn < 0 ? -1 : 1);

  motor[frontRight] = -fF + fT;
  motor[backRight] = -fF + fT;
  motor[frontLeft] = -fF - fT;
  motor[backLeft] = -fF - fT;*/

  int leftDrivePower, rightDrivePower;
  leftDrivePower = (-drivePower)-turn;
  rightDrivePower = (-drivePower)+turn;

  motor[frontRight] = rightDrivePower;
  motor[backRight] = rightDrivePower;
  motor[frontLeft] = leftDrivePower;
  motor[backLeft] = leftDrivePower;
}
void driveStraight(int drivePower) {
  int turn = 0;
  int leftDrivePower, rightDrivePower;
  leftDrivePower = (-drivePower)-turn;
  rightDrivePower = (-drivePower)+turn;

  motor[frontRight] = rightDrivePower;
  motor[backRight] = rightDrivePower;
  motor[frontLeft] = leftDrivePower;
  motor[backLeft] = leftDrivePower;
}

void turn(int drivePower, int turnSpeed, int targetAngle) {
  // turnSpeed = negative = left
  // turnSpeed = positive = right
  // targetAngle = negative then make turnSpeed = negative
	int turnNumber = 0;
	  doNothing(turnNumber);
  if (drivePower != 0)
    drivePower = -drivePower;
  else
    drivePower = 0; int gyroval;
  gyroval = SensorValue[gyro]/10;
  while (targetAngle != gyroval) {
    gyroval = SensorValue[gyro]/10;
    motor[frontRight] = motor[backRight] = drivePower-turnSpeed;
    motor[frontLeft] = motor[backLeft] = drivePower+turnSpeed;
  }
  motor[frontRight] = motor[backRight] = motor[frontLeft] = motor[backLeft] = 0;
  turnNumber = 0;
}

bool usingPID = false;
task armPID() {
  usingPID = false;
  int prevDiff = 0;
  float kP = 0.3;
  float kD = 0.0;
  while(true) {
    if (usingPID) {

	    int diff = armSetpoint - SensorValue[pot];
	    // Positive = Higher - Lower. Higher than setpoint.
	    // Negative = Lower - Higher. Lower than setpoint.
	    int output = ((float) diff * (float) kP) + (((float) diff - (float) prevDiff) * (float) kD);
	    lift(output);
	    prevDiff = diff;
	  }
	  wait1Msec(20);
  }
}

void pre_auton() {
	SensorType[gyro] = sensorNone;
	wait1Msec(1000);
	SensorType[gyro] = sensorGyro;
	wait1Msec(2000);
	SensorValue[gyro]=0;
	SensorFullCount[gyro]=3800;
}

task autonomous() {

    // Main PIDless
	  spin(-90); wait1Msec(500); spin(0);
		driveStraight(100); wait1Msec(300);
		driveStraight(100); wait1Msec(500);
	  driveStraight(50); lift(-20); spin(120); wait1Msec(600);
	  allMotorsOff(); spin(120); wait1Msec(500);
	  driveStraight(100); wait1Msec(400); driveStraight(0);
		lift(10); turn(0, -60, -60);
	  driveStraight(0); spin(25); lift(120); wait1Msec(600);
	  driveStraight(0); spin(25); lift(18); wait1Msec(500);
		driveStraight(120); spin(20); wait1Msec(300);
		driveStraight(110); wait1Msec(550); // Changed from 1400 to 1000 to 550
		driveStraight(0); lift(5); spin(-100); wait1Msec(3000);
	  allMotorsOff();

	  // Main PID

	/*  StartTask(armPID);
	  spin(-90); wait1Msec(500); spin(0);
		driveStraight(100); wait1Msec(500); // Changed 300 to 500
		driveStraight(100); wait1Msec(600);
	  driveStraight(50); lift(-5); spin(120); wait1Msec(700);
	  allMotorsOff(); spin(120); wait1Msec(500);
	  driveStraight(100); wait1Msec(400); driveStraight(0);
		lift(10); turn(0, -60, -30);
	  driveStraight(0); spin(25);
	     armSetpoint = middleArm;
	     usingPID = true;
	     wait1Msec(500);
	  driveStraight(0); spin(25); wait1Msec(500);
		driveStraight(120); spin(20); wait1Msec(300);
		driveStraight(110); wait1Msec(1500);
		driveStraight(0); spin(-100); wait1Msec(3000);
	  usingPID = false; allMotorsOff(); StopTask(armPID);

	  // Alternate PID
	  StartTask(armPID);
	  spin(-90); wait1Msec(100); spin(0);
	  driveStraight(120); wait1Msec(600);
	  driveStraight(30); lift(-10); spin(120); wait1Msec(700);
	  armSetpoint = middleArm; usingPID = true; wait1Msec(500);
	  driveStraight(120); wait1Msec(800);
	  driveStraight(0); spin(-100); wait1Msec(5000);
    usingPID = false; allMotorsOff(); StopTask(armPID);*/
}
bool override = false;
task usercontrol() {
  StartTask(armPID);
  int da,db, liftPower;
  if (0 == 1) {
    warningKiller();
  }
	int spinPower;
	while (true) {
	  // Intake code
		if (vexRT[Btn6D] == 1 || vexRT[Btn6U] == 1) {
		spinPower = 127;
		} else if (vexRT[Btn5D] == 1 || vexRT[Btn5U] == 1) {
	  spinPower = -128;
		}

		// Lift Code & Arm Lock
		if (vexRT[Btn7L] == 1) {
		  armSetpoint = SensorValue[pot];
		  usingPID = true;
	  }
	  if (vexRT[Btn7D] == 1) {
    	usingPID = false;
	  }
		liftPower = vexRT[Ch2];
		if (usingPID && abs(vexRT[Ch2]) > 20) {
		  usingPID = false;
	  }

    // Drive Code
    da = vexRT[Ch3]; db = vexRT[Ch4];
		if(vexRT[Btn8L] == 1) {
      da = 0; db = 0;
      liftPower = 0;
      spinPower = 0;
    }

    // Dead Lock & Drive Code & Run Code
	  if ((abs(da) < 2) && (abs(db) < 2))
		  drive(0, 0);
		else
		  drive(da, db);

		if (!usingPID)
      lift(liftPower);
		spin(-spinPower);
		spinPower = da = db = 0;
  }
}
