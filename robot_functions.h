#ifndef ROBOT_FUNCTIONS_H_
#define ROBOT_FUNCTIONS_H_

int checkButtonSequence()
{
    if (vexRT[Btn7L] && vexRT[Btn7D])
        return 1;
    return 0;
}

int basicMove(int speed) {

    motor[leftMotor] = speed;
    motor[rightMotor] = speed;

    return checkButtonSequence();
}

void wait(int milliseconds) {
    motor[leftMotor] = 0;
    motor[rightMotor] = 0;

    wait1Msec(milliseconds);
}

void basicSwingTurn(int direction, int speed) {
    if (direction == -1) {
        motor[rightMotor] = speed;
        motor[leftMotor] = 0;
    }
    else {
        motor[rightMotor] = 0;
        motor[leftMotor] = speed;
    }
}

int followLine(int threshold)
{
    if (checkButtonSequence())
        return 1;

    //Turn right
    if (SensorValue[rightLineFollower] > threshold) {
        basicSwingTurn(1, 50);
    }

    if (SensorValue[centerLineFollower] > threshold) {
        if (basicMove(40))
            return 1;
    }

    //Turn left
    if (SensorValue[leftLineFollower] > threshold) {
        basicSwingTurn(-1, 50);
    }

    return 0;
}

void clawGrab()
{  
    while (SensorValue[limitSwitch] == 0)
        motor[armMotor] = -20;

    motor[armMotor] = 0;
    motor[clawMotor] = -40;
    wait1Msec(1000);
    motor[clawMotor] = 0;

    motor[armMotor] = 60;
    wait1Msec(700);
    motor[armMotor] = 0;
}

void clawReset()
{
    motor[clawMotor] = 60;
    wait1Msec(500);
    motor[clawMotor] = 0;

    while (SensorValue[limitSwitch] == 0)
        motor[armMotor] = -20;
    motor[armMotor] = 0;

    motor[armMotor] = 60;
    wait1Msec(500);
    motor[armMotor] = 0;
}

void turnAround()
{
    wait(500);
    motor[leftMotor] = 60;
    motor[rightMotor] = -60;
    wait1Msec(1200);

    motor[leftMotor] = 0;
    motor[rightMotor] = 0;
}

#endif
