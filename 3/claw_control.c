#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port6,           armMotor,      tmotorServoContinuousRotation, openLoop)

// Verkefni 3 - Hluti 2

task main()
{
    while (1) {
        motor[leftMotor] = vexRT[Ch2];
        motor[rightMotor] = vexRT[Ch3];

        if (vexRT[Btn8U])
            motor[armMotor] = 40;
        else if (vexRT[Btn8D])
            motor[armMotor] = -40;
        else
            motor[armMotor] = 0;
    }
}
