#pragma config(Sensor, in1,    leftLineFollower, sensorLineFollower)
#pragma config(Sensor, in2,    centerLineFollower, sensorLineFollower)
#pragma config(Sensor, in3,    rightLineFollower, sensorLineFollower)
#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  limitSwitch,    sensorTouch)
#pragma config(Sensor, dgtl8,  sonar,          sensorSONAR_cm)
#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port6,           armMotor,      tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           clawMotor,     tmotorServoContinuousRotation, openLoop)

//Basically got this value by moving a piece of tape over the sensors a couple of times and averaged the measurements
static const int threshold = 500;

#include "../robot_functions.h"

task main()
{
  //Occasionally the claw gets stuck, so I use this to unstick it
  /*
  motor[clawMotor] = 127;
  wait1Msec(500);
  */

  clawReset();

  while (SensorValue[sonar] > 21)
    if (followLine(threshold))
      break;

  wait(1000);
  clawGrab();

  wait(1000);
  turnAround();

  while (SensorValue[sonar] > 20)
    if (followLine(threshold))
      break;

  wait(500);
  motor[clawMotor] = 60;
  wait1Msec(500);

}
