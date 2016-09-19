#pragma config(Sensor, dgtl1,  Encoder1,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  Encoder2,    sensorQuadEncoder)
#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop, reversed)

//Constants defined, see comments for a detail explanation behind these values
static const int FORWARDS = 1;
static const int FULL_SPEED = 127;
static const int N = 720;
static const float M = 2.896;

/*

   --- BEGIN PSUEDOCODE ---

   do 3 times:
       i = iteration number
       move(forwards, 0.5 meter)
       if i is:
           0: turn left 90 deg   (90 deg)
           1: turn right 90 deg  (-90 deg)
           2: turn right 90 deg  (-90 deg)

--- END PSUEDOCODE ---

    Circumference of wheel = 2 * pi * 4 = 25,13 cm
    50 / 25,13 = 1,9896538002387584560286510147234
    ~2 rotations for a half meter

    2 * 360 = 720
    ~720 deg of rotation on the shaft encoder for half a meter

    --- Determining the ratio between rotation of the wheels and robot ---

    R = average distance from the centrum of the robot to the wheels
    r = radius of a wheel

    C = 2 * pi * R
    c = 2 * pi * r
    M = ratio = C/c
    m = M/360

    M is how often a wheel has to turn to go around the "radius" of the robot
    m is turns of a wheel to a single degree of rotation of the robot

    R = 13cm
    r = 4.5cm

    C = 81.86cm
    c = 28.27cm

    M = 2,8956490979837283339228864520693
    M = ~2.896

    m = 2.896

*/

int move(int speed, int direction, int rotations) {
    //Self-correcting movement function, moves based on rotations of the wheels

    //Reset the encoders
    SensorValue[Encoder1] = 0;
    SensorValue[Encoder2] = 0;

    while (abs(SensorValue[Encoder1]) < rotations) {

        //Emergency stop sequence
        if (vexRT[Btn7L] && vexRT[Btn7D])
          return -1;

        motor[rightMotor] = speed * direction;
        motor[leftMotor]  = speed * direction;

        // If the robot is moving forwards, execute the following
        if (direction == 1) {
            if (SensorValue[Encoder1] == SensorValue[Encoder2]) {
                // The motors are synchronized
                motor[rightMotor] = speed * direction;
                motor[leftMotor]  = speed * direction;
            }
            else if (SensorValue[Encoder1] > SensorValue[Encoder2]) {
                // Motors are not synchronized, correct with motor port 2
                motor[rightMotor] = 0.7 * speed * direction;
                motor[leftMotor]  = speed * direction;
            }
            else {
                motor[rightMotor] = speed * direction;
                motor[leftMotor] = 0.7 * speed * direction;
            }
        }
        // The robot is moving backwards, do this
        else {
            if (SensorValue[Encoder1] == SensorValue[Encoder2]) {
                // The motors are synchronized
                motor[rightMotor] = speed * direction;
                motor[leftMotor]  = speed * direction;
            }
            else if (SensorValue[Encoder1] < SensorValue[Encoder2]) {
                // Motors are not synchronized, correct with motor port 2
                motor[rightMotor] = 0.7 * speed * direction;
                motor[leftMotor]  = speed * direction;
            }
            else {
                motor[rightMotor] = speed * direction;
                motor[leftMotor] = 0.7 * speed * direction;
            }
        }
    }
    return 0;
}

void wait(int milliseconds) {
    motor[rightMotor] = 0;
    motor[leftMotor] = 0;
    wait1Msec(milliseconds);
}

int turn(int degrees, int speed) {
    /*  Degrees is between -180 and 180
        Positive numbers are counter-clockwise
        Negative numbers are clockwise

        Turning either 180 degrees or -180 degrees is a full circle
        90 deg turn is full left
        -90 deg is full right

        Left turn = leftMotor backwards, rightMotor forwards
        Right turn = rightMotor backwards, leftMotor fowards
        */

    //Reset the encoders
    SensorValue[Encoder1] = 0;
    SensorValue[Encoder2] = 0;

    int rotations = degrees * M;
    if (rotations > 0) {
        //Left turn
        while (SensorValue[Encoder1] < rotations) {
            if (vexRT[Btn7L] && vexRT[Btn7D])
                return -1;
            motor[leftMotor] = -speed;
            motor[rightMotor] = speed;
        }
    }
    else {
        //Right turn
        while (SensorValue[Encoder1] > rotations * 1.25) {
            if (vexRT[Btn7L] && vexRT[Btn7D])
                return -1;
            motor[leftMotor] = speed;
            motor[rightMotor] = -speed;
        }
    }
    return 0;
}


task main()
{
    int exitcode = 0;
    for (int i = 0; i < 3; i++) {
        if ((vexRT[Btn7L] && vexRT[Btn7D]) || exitcode == -1)
          goto end;

        wait(1000);
        exitcode = move(FULL_SPEED / 2, FORWARDS, N);
        if (exitcode == -1)
            goto end;

        wait(1000);
        switch (i) {
            case 0:
                exitcode = turn(90, 50);
                break;
            case 1:
                exitcode = turn(-90, 50);
                break;
            case 2:
                exitcode = turn(-90, 50);
                break;
        }
    }
    wait(1000);
    move(FULL_SPEED / 2, FORWARDS, N);
    end:;
}
