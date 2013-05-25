//The values that a PIDController "object" has
typedef struct {
	float k_P;
	float k_I;
	float k_D;

	bool enabled;

	int minInput;
	int maxInput;
	int minOutput;
	int maxOutput;

	int setpoint;
	int result;
	int input;
	int error;
	int prevError;
	int totalError;

	int inputSensorIndex;
	int outputMotorIndex[4];
} PIDController;

//Set up the controller
void init(PIDController controller) {
	controller.k_P = 0;
	controller.k_I = 0;
	controller.k_D = 0;
	controller.enabled = 0;
	controller.minInput = 0;
	controller.maxInput = 0;
	controller.minOutput = -127;
	controller.maxOutput = 127;
	controller.totalError = 0;
	controller.prevError = 0;
}

//Set which sensor we are controlling. This wont' really work unless you know the sensor port. Which is bad coding.
void init(PIDController controller, int inputIndex) {
	init(controller);
	controller.inputSensorIndex = inputIndex;
}

//Remember to enable your PIDController before you use it
void enable(PIDController controller) {
	controller.enabled = true;
}

void disable(PIDController controller) {
	controller.enabled = false;
}

//Set our target value
void setSetpoint(PIDController controller, int newSetpoint, bool zero) {
	//zero integration and old error if we want and only if we are changing the setpoint
	if(zero && controller.setpoint!=newSetpoint){
		controller.totalError=0;
		controller.prevError=0;
	}
	controller.setpoint = newSetpoint;
}

//Set the PID variables
void setPIDs(PIDController controller, float kP, float kI, float kD) {
	controller.k_P = kP;
	controller.k_I = kI;
	controller.k_D = kD;
}

//Set the extreme values our controller setpoint. Not implemented, so useless.
void setInputRange(PIDController controller, int min, int max) {
	if(max > min) {
		controller.minInput = min;
		controller.maxInput = max;
	}
}

//Set the power range we can send to motors
void setOutputRange(PIDController controller, int min, int max) {
	if(max > min && min >= -127 && max <= 127) {
		controller.minOutput = min;
		controller.maxOutput = max;
	}
}

//Calculate the output for the PIDController
int calculatePID(PIDController controller, int delta/*unused*/) {
	//use local storage for efficiency
	int error_local=0;
	if (controller.enabled) {
		controller.input = SensorValue[controller.inputSensorIndex];
		error_local = controller.setpoint - controller.input;
		//Calculate integral value
		controller.totalError+=error_local;
		//Calculate and sum the rest of the PID values
		        controller.result = (error_local * controller.k_P +
                                 controller.totalError * controller.k_I +
                                (error_local - controller.prevError) * controller.k_D);

		        if (controller.result > controller.maxOutput) {
		            controller.result = controller.maxOutput;
		        } else if (controller.result < controller.minOutput) {
		            controller.result = controller.minOutput;
		        }
		controller.error=error_local;
		controller.prevError = error_local;
		return controller.result;
    } else {
    return 0.0;
  }
}

//Calculate the output for the PIDController with input not from a sensor
int calculatePID(PIDController controller, int input,int delta/*unused*/) {
	//use local storage for efficiency
	int error_local=0;
	if (controller.enabled) {
		controller.input =input;
		error_local = controller.setpoint - controller.input;
		//Calculate integral value
		controller.totalError+=error_local;
		//Calculate and sum the rest of the PID values
		        controller.result = error_local * controller.k_P +
                                 controller.totalError * controller.k_I +
                                (error_local - controller.prevError) * controller.k_D;

		        if (controller.result > controller.maxOutput) {
		            controller.result = controller.maxOutput;
		        } else if (controller.result < controller.minOutput) {
		            controller.result = controller.minOutput;
		        }
		controller.error=error_local;
		controller.prevError = error_local;
		return controller.result;
    } else {
    return 0.0;
  }
}
