/*
  Hluti 1

  -- Psuedocode --
  n = time_to_move_half_meter

	while True:
		do 3 times:
		  i = iteration number
		  move_forwards(n * i milliseconds)
		  wait(1 second)
		  move_backwards(n * i milliseconds)
		  wait(1 second)

	-- Psuedocode --

  Distance moved in one second on full power = 0.61m
  n = 0,81967213114754098360655737704918
  The robot isn't perfect so I rounded it up to 900 milliseconds
*/

//Couple of constants defined
static const int FULL_SPEED = 127;
static const int FORWARDS = 1;
static const int BACKWARDS = -1;
static const int N = 900;

void move(int speed, int direction, int milliseconds) {
  //This function handles time based movement of the robot
  motor[port2] = speed * direction;
  motor[port3] = -speed * direction;
  wait1Msec(milliseconds);
}

void wait(int milliseconds) {
  //This function resets the motors to 0 and waits a specified time
  motor[port2] = 0;
  motor[port3] = 0;
  wait1Msec(milliseconds);
}

task main()
{
	//Short wait before the robot goes nuts
  wait(1000);

	for (int i = 1; i < 4; i++) {
		move(FULL_SPEED, FORWARDS, N * i);
    wait(1000); // Reset the motors and wait for a second

		move(FULL_SPEED, BACKWARDS, N * i);
    wait(1000);
	}
}
