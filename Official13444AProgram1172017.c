#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           leftSlave,     tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port3,           rightSlave,    tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port4,           leftMotor,     tmotorVex393_MC29, PIDControl, reversed, driveLeft, encoderPort, I2C_1)
#pragma config(Motor,  port5,           rightMotor,    tmotorVex393_MC29, PIDControl, driveRight, encoderPort, I2C_2)
#pragma config(Motor,  port6,           rightLift,     tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_3)
#pragma config(Motor,  port7,           leftLift,      tmotorVex393_MC29, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port8,           linkageMotor,  tmotorVex393_MC29, PIDControl, encoderPort, I2C_5)
#pragma config(Motor,  port9,           clawMotor,     tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

float countsPerYard = 600;
float countsPerRev = 627.2;
float leftSpeed = 0;
float rightSpeed = 0;
bool pinch = false;

void LCDManager (const string line1, const string line2) {
	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDString(0, 0, line1);
	displayLCDString(1, 0, line2);
}


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);
	//resetMotorEncoder(clawMotor);
	resetMotorEncoder(linkageMotor);
	resetMotorEncoder(leftLift);
	resetMotorEncoder(rightLift);

	slaveMotor(rightLift, leftLift);
	slaveMotor(leftSlave, leftMotor);
	slaveMotor(rightSlave, rightMotor);
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	bDisplayCompetitionStatusOnLcd = false;

	LCDManager("Pre_Autonomous", "Status: Success");


}

static const float fudgeCountsWhenSomethingsTotallyJankedUp = 1.34;
static const float ticksPerInch = 0.95 * (627.2 / (4.1 * PI));
static const float ticksPerRev = fudgeCountsWhenSomethingsTotallyJankedUp * (ticksPerInch * 15.125 * PI);

//Drive Straight
void straight(float dist, int speed, bool hold)
{
	LCDManager("Straight Move", "");

	resetMotorEncoder(rightMotor);
	resetMotorEncoder(leftMotor);

	setMotorTarget(rightMotor, dist*ticksPerInch, speed, hold);
	setMotorTarget(leftMotor, dist*ticksPerInch, speed, hold);

	while (!getMotorTargetCompleted(rightMotor) && !getMotorTargetCompleted(leftMotor))
		wait1Msec(10);

}

void turn(float dist, int speed, bool hold)
{

	LCDManager("Turn Move       ", "                ");

	resetMotorEncoder(rightMotor);
	resetMotorEncoder(leftMotor);

	setMotorTarget(rightMotor, dist*ticksPerRev, speed, hold);
	setMotorTarget(leftMotor, -dist*ticksPerRev, speed, hold);

	while (!getMotorTargetCompleted(rightMotor) && !getMotorTargetCompleted(leftMotor))
		wait1Msec(10);

}



/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/



task autonomous()
{
	LCDManager("Autonomous", "Beginning");
	nMotorPIDSpeedCtrl[leftLift] = RegIdle;
	nMotorPIDSpeedCtrl[rightLift] = RegIdle;

	motor[clawMotor] = -15;
	sleep(2500);
	setMotorTarget(linkageMotor, 7 * -.46 * countsPerRev, -100, true);
	sleep(3500);
	straight(19, 127, false);
	sleep(1000);
	moveMotorTarget(linkageMotor, 2000, 20, true);
	sleep(1500)
	motor[clawMotor] = 50;
	sleep(500);
	motor[clawMotor] = 50;
	straight(-17, 127, false);
	LCDManager("Autonomous", "Completed");
	//setMotorTarget(clawMotor, -.80 * countsPerRev, -25, false);
	/*motor[clawMotor] = -20;
	sleep(1000);
	motor[clawMotor] = -15;
	setMotorTarget(linkageMotor, -1500, -75, false);
	sleep(500);
	straight(43, 127, false);
	setMotorTarget(linkageMotor, -650, 40, false);
	sleep(1500);
	motor[clawMotor] = 75;
	//setMotorTarget(clawMotor, .50 * countsPerRev, 75, false);
	sleep(750);
	setMotorTarget(linkageMotor, 0, 40, false);
	sleep(500);
	straight(-10, 127, false);
	turn(.5, 100, false);
	//setMotorTarget(leftLift, -900, 100, false);
	motor[leftLift] = 100;
	sleep(600);
	motor[leftLift] = 0;
	//straight(-24, 127, false);
	//motor[leftLift] = -100;
	//sleep(600);
	//motor[leftLift] = 0;
	//straight(60, 127, false);*/



}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
	LCDManager("Driver-Ctlr", "");
	//PID fixup
	nMotorPIDSpeedCtrl[rightMotor] = RegIdle;
	nMotorPIDSpeedCtrl[leftMotor] = RegIdle;
	nMotorPIDSpeedCtrl[clawMotor] = RegIdle;
	nMotorPIDSpeedCtrl[linkageMotor] = RegIdle;
	nMotorPIDSpeedCtrl[leftLift] = RegIdle;
	nMotorPIDSpeedCtrl[rightLift] = RegIdle;

	while (true)
	{

		leftSpeed = (vexRT[Ch3] * 3) / 4;
		rightSpeed = (vexRT[Ch2] * 3) / 4;

		// Fork D-Pad
		if (vexRT[Btn8U]) {
			leftSpeed = -80;
			rightSpeed = -80;
		} else if (vexRT[Btn8R]) {
			leftSpeed = 55;
			rightSpeed = -55;
		} else if (vexRT[Btn8D]) {
			leftSpeed = 80;
			rightSpeed = 80;
		} else if (vexRT[Btn8L]) {
			leftSpeed = -55;
			rightSpeed = 55;
		}

		// Claw D-Pad
		if (vexRT[Btn7U]) {
			leftSpeed = 80;
			rightSpeed = 80;
		} else if (vexRT[Btn7R]) {
			leftSpeed = -55;
			rightSpeed = 55;
		} else if (vexRT[Btn7D]) {
			leftSpeed = -80;
			rightSpeed = -80;
		} else if (vexRT[Btn7L]) {
			leftSpeed = 55;
			rightSpeed = -55;
		}

		motor[leftMotor] = leftSpeed;
		motor[rightMotor] = rightSpeed;

		if (fabs(vexRT[Ch2Xmtr2]) < 10) {
			motor[linkageMotor] = -15;
		} else {
			motor[linkageMotor] = -(vexRT[Ch2Xmtr2] / 2);
		}

		motor[leftLift] = -vexRT[Ch3Xmtr2];



		if (vexRT[Btn7UXmtr2]) {
			motor[clawMotor] = 127;
			pinch = false;
		} else if (vexRT[Btn7DXmtr2]) {
			motor[clawMotor] = -127;
			pinch = true;
		} else if (pinch) {
			motor[clawMotor] = -16;
		} else if (!pinch) {
			motor[clawMotor] = 0;
		}

	}
}
