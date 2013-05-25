void FlipIntake(){
		IntakeMove(-60);
		wait1Msec(500);
		driveMecanum(0,0,0);	IntakeMove(0);
}

void LeftAuton(){
		FlipIntake();
		driveMecanum(30,0,0);	IntakeMove(127);	//move to center stack
		wait1Msec(2000);
		driveMecanum(0,0,0);
		wait1Msec(1000);
		driveMecanum(30,0,0);
		wait1Msec(1000);
		driveMecanum(-30,0,0);
		wait1Msec(800);
		driveMecanum(0,0,0);
		wait1Msec(500);
		driveMecanum(-30,0,0);
		wait1Msec(1200);
		driveMecanum(0,0,0);	gyroSetPoint=getGyro()+200;	//turn to face goal
		armSetPoint=1700;
		wait1Msec(1000);
		IntakeMove(0);
		driveMecanum(0,-60,0); 	//goto goal
		wait1Msec(1750);
		armSetPoint=scoreHeight;
		wait1Msec(450);
		driveMecanum(90,0,0);
		wait1Msec(1000);
		driveMecanum(0,0,0);
		wait1Msec(300);
		IntakeMove(-127);
}

void RightAuton(){
		FlipIntake();
		driveMecanum(30,0,0);	IntakeMove(127);	//move to center stack
		wait1Msec(2000);
		driveMecanum(0,0,0);
		wait1Msec(1000);
		driveMecanum(30,0,0);
		wait1Msec(1000);
		driveMecanum(-30,0,0);
		wait1Msec(800);
		driveMecanum(0,0,0);
		wait1Msec(500);
		driveMecanum(-30,0,0);
		wait1Msec(1200);
		driveMecanum(-80,0,0);
		wait1Msec(200);
		driveMecanum(0,0,0);	gyroSetPoint=getGyro()-200;	//turn to face goal
		armSetPoint=1700;
		wait1Msec(1000);
		IntakeMove(0);
		driveMecanum(0,-60,0); 	//goto goal
		wait1Msec(1750);
		armSetPoint=scoreHeight;
		wait1Msec(450);
		driveMecanum(90,0,0);
		wait1Msec(1000);
		driveMecanum(0,0,0);
		wait1Msec(300);
		IntakeMove(-127);
}

void DoNothingAuton(){
	FlipIntake();
}
