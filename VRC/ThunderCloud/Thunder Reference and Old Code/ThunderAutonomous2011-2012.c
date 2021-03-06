#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    armPot,         sensorPotentiometer)
#pragma config(Sensor, in2,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  area,           sensorDigitalIn)
#pragma config(Sensor, dgtl2,  color,          sensorDigitalIn)
#pragma config(Sensor, dgtl3,  button,         sensorTouch)
#pragma config(Sensor, I2C_1,  encoderRight,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  encoderLeft,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           backleft,      tmotorVex393, openLoop, reversed, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port2,           left,          tmotorVex393, openLoop)
#pragma config(Motor,  port3,           armright1,     tmotorVex269, openLoop)
#pragma config(Motor,  port4,           armleft1,      tmotorVex269, openLoop, reversed)
#pragma config(Motor,  port5,           spin,          tmotorVex269, openLoop)
#pragma config(Motor,  port6,           spin2,         tmotorVex269, openLoop, reversed)
#pragma config(Motor,  port7,           armleft2,      tmotorVex269, openLoop, reversed)
#pragma config(Motor,  port8,           armright2,     tmotorVex269, openLoop)
#pragma config(Motor,  port9,           right,         tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port10,          backright,     tmotorVex393, openLoop, encoder, encoderPort, I2C_1, 1000)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(60)
#pragma userControlDuration(0)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!
#include "PIDController.c"

/////////////////////////////////////////////////////////////////////////////////////////
//
//                         Driver Skills Template
//
// This is a template for the driver skills competition. It is identical in structure to
// the standard competition template except that the two "#pragma' statements above (and
// copied below) adjust the time limits for the competition.
//				#pragma autonomousDuration(0)
//				#pragma userControlDuration(60)
// The autonomous duration for "Driver Skills" is zero; i.e. there is no autonomous phase.
// The user control duration is 60 seconds
//
// NOTE: The above two pragmas only impact the Crystal implementation. For systems using
//       VEXNET, the VEXNET system ignores these statements as it has the durations
//       for the competition built-in.
//
// Whenever this user program is run, the duration is automatically sent to the VEX master
// processor. The master CPU will only accept these commands once per power=on cycle. So, it's
// best to power cycle your VEX to ensure that they get recognized in case you were
// previously running a standard competition program with different durations.
//
int leftDrivePower, rightDrivePower, x, y, rampLimit = 2, leftDrivePrevious, rightDrivePrevious;//drivetrain variables
int gyroDrift=0;

void resetDriveVariables(){
  leftDrivePower = rightDrivePower = x = y = leftDrivePrevious = rightDrivePrevious = 0;
}
void setDriveMotors(){
  motor[left] = leftDrivePower;
  motor[backleft] = leftDrivePower;
  motor[right] = rightDrivePower;
  motor[backright] = rightDrivePower;
}

void stopDrive(){
  rightDrivePower = leftDrivePower = 0;
  setDriveMotors();
}

void driveArcade(int power, int turn, bool square, bool ramp){
  if(square)
  {
    if(power != 0)
    {
      y = ((power*power)/127)*(power/abs(power));
    }
    else
    {
      y = 0;
    }

    if(turn!=0)
    {
      x = ((turn*turn)/127)*(turn/abs(turn));
    }
    else
    {
      x = 0;
    }
  }
  else
  {
    y = power;
    x = turn;
  }

  leftDrivePower = y+x;
  rightDrivePower = y-x;

  if(ramp == true)
  {
    if(abs(rightDrivePower) > abs(rightDrivePrevious) + rampLimit)
    {
      if(rightDrivePower > (rightDrivePower + rampLimit))
      {
        rightDrivePower = rightDrivePrevious + rampLimit;
      }
      else
      {
        rightDrivePower = rightDrivePrevious - rampLimit;
      }
    }

    if(abs(leftDrivePower) > abs(leftDrivePrevious) + rampLimit)
    {
      if(leftDrivePower > (leftDrivePower + rampLimit))
      {
        leftDrivePower = leftDrivePrevious + rampLimit;
      }
      else
      {
        leftDrivePower = leftDrivePrevious - rampLimit;
      }
    }
  }

  setDriveMotors();

  leftDrivePrevious = leftDrivePower;
  rightDrivePrevious = rightDrivePower;
}// end driveArcade

void turn(int angle, int power, bool left){
  time1[T2]=0;
  SensorValue[gyro]=0;
  float gyroError=0;
  int startAngle=SensorValue[gyro];
  int currentAngle=SensorValue[gyro];
  int OriginalTargetAngle=angle*10+startAngle; //gyros measure in tenths of a degree
  int targetAngle=OriginalTargetAngle;
  if(!left){
    OriginalTargetAngle=-OriginalTargetAngle;
    power=-power;
  }

  while(abs(currentAngle)<abs(targetAngle)){
      gyroError=time1[T2]/1000*gyroDrift;
      targetAngle=OriginalTargetAngle+gyroError;
      driveArcade(0,-power,false,false);//right is positive, according to driveArcade, but negative according to gyro.
      currentAngle=SensorValue[gyro];
  }
  driveArcade(0,power/5,false,false);//brake
  driveArcade(0,0,false,false);
}//end turn
/*******************************************************************************************Arm & Code****************************************************************************************/

/*********PID Variables****************/
/**************************************/
PIDController Arm;

int ArmSetPoint;
int ArmKp = 10,ArmKi = 0, ArmKd = 0;

/**************************************/
/**************************************/

void setArmPids(kp, ki, kd){
  ArmKp = kp;
  ArmKi = ki;
  ArmKd = kd;
  setPIDs(Arm, ArmKp, ArmKi, ArmKd);
}

// LDR November 11, 2011 -- armPot: armFloor = 1975, armHighPlace = 3730, armMax =
int armPower, spinPower, armPosition, armTolerance, initialArmPosition, armOffset;//arm tolerance to be set
int armFloor = 1750, armLowScore = 2570, armMidScore = 2940, armHighScore = 3400, armMax = 3845, armSetPoint, armState = 0;//arm positions. Place is position above goal, Score is on goal for tubes, floor is start position
int armInput, armPreviousInput, armRampLimit = 2;//arm variables used in competitions code. Placed here to keep them with the rest of the arm variables
bool armPositionReached, slowClose = false;
int isolation;


void setArmMotors(){

  motor[armleft1] = motor[armleft2] = motor[armright1] = motor[armright2] = armPower;
}

void armMove(int aPower){
  armPower = aPower;
  motor[armleft1] = motor[armleft2] = motor[armright1] = motor[armright2] = armPower;
}

void spinMove(int sPower)
{
  spinPower = sPower;
  motor[spin] = motor[spin2] = spinPower;
}


/*****************************************************************************************Competition Code*****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////
void pre_auton()
{
  //LDR Intialize Gyro on in2
SensorType[in2] = sensorNone;
wait1Msec(1000);
SensorType[in2] = sensorGyro; //+ is ccw, - is cw
wait1Msec(2000);
//calculate gyroscope drift
SensorValue[gyro]=0;
ClearTimer(T2);//we will use T2 for the gyroscope
int original=SensorValue[gyro];
wait1Msec(1000);
int final=SensorValue[gyro];
gyroDrift=(final-original);
ClearTimer(T2);
SensorValue[gyro]=0;
SensorValue[encoderRight]=SensorValue[encoderLeft]=0;
  //init(Arm,1);
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{

  {
      time1[T1] = 0;
		  while(time1[T1] < 1700) //flip out rotors and intake pre-load
			    spinMove(128);
			spinMove(0);

			time1[T1] = 0;
		  while(time1[T1] <1500)  // raise arm to high goal level
			    armMove(100);

		  time1[T1] = 0;
		  while (time1[T1] < 3000) //hold up arm to allow for loading
		      armMove(50);

		  time1[T1] = 0;
		  while (time1[T1] < 4500) //Drive forwards to goal, (while holding up arm)
		     driveArcade(40,0,false,false);

		  driveArcade(20,0,false,false);
		  time1[T1]= 0;
		  while (time1[T1] < 3000){ //Release load and score!! (while holding up arm)
			  spinMove(-80);
			  armMove(50);
			}
		  armMove(0);
		  spinMove(0);
		  driveArcade(-100,0,false,false);

		  time1[T1]=0;//Drive home and lower arm
		  while (time1[T1] < 1600){
		    if(time1[T1]>500){
		      armMove(-60);
		      if(SensorValue[armPot]<=(armFloor)) armMove(0);
		    }
		  }
		  armMove(0);
		  driveArcade(0,0,false,false);

		  time1[T1]=0;//wait to repostition robot for 5 whole seconds
		  while(time1[T1]<5000){
		    if(SensorValue[button]==1)//raise arm to doubler height
		      armMove(35);
		    else
		      armMove(0);
		  }

		  time1[T1]=0;//Drive like an old Asian lady to the doubler
		  spinMove(128);//because omnomnomnom
		  while(time1[T1]<5000){
		    driveArcade(40,0,false,false);
		  }
		  spinMove(0);

		  time1[T1]=0;//Drive home
		  while(time1[T1]<2200){
		    driveArcade(-99,0,false,false);
		  }
		  driveArcade(0,0,false,false);

		  time1[T1]=0;//Wait for only 3 seconds
		  while(time1[T1]<3000){
		  }

      //Do the same thing as before to put the doubler in the goal
		  time1[T1] = 0;
		  while(time1[T1] <1500)  // raise arm to high goal level
			    armMove(100);

      armMove(50);
		  time1[T1] = 0;
		  while (time1[T1] < 4500) //Drive forwards to goal, (while holding up arm)
		     driveArcade(40,0,false,false);

      driveArcade(10,0,false,false);
		  time1[T1] = 0;
		  while (time1[T1] < 3000) //Release load and score!! (while holding up arm)
			  spinMove(-80);

      time1[T1]=0;//Drive home again
		  while(time1[T1]<2000){
		    driveArcade(-100,0,false,false);
		  }

			armMove(0);
			spinMove(0);
			driveArcade(0,0,false,false);

			//make sure it's never the programmer's fault
			bool RickIsDumb=true;
			bool itsRicksFault=false;
			if(RickIsDumb){
			itsRicksFault=true;
			}
  }

}
/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
//I don't know why the programming skills program has a user control part, so don't ask

task usercontrol()
{
	// User control code here, inside the loop
      resetDriveVariables();
      armState = 0;
	    // This is the main execution loop for the user control program. Each time through the loop
	    // your program should update motor + servo values based on feedback from the joysticks.
    	while (true){}
}
