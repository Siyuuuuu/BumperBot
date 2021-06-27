// =======================
//       Bumper Bot
// =======================

// Code for the bumper bot that is going forward until it hits 
// on an object. Indicating LED will be turn on while the bot 
// is backing up and turning towards another direction.

#include <Servo.h>

Servo servoLeft;      // Left motor 
Servo servoRight;      // Right motor

int rightLED = 2;     // Right LED connect to pin 2
int rightBump = 4;      // Right bumper connect to pin 4
int leftBump = 7;     // Left bumper connect to pin 7
int leftLED = 8;      // Left LED connect to pin 8
int rightMotor = 9;     // Right motor PWM connect to pin 9
int leftMotor = 10;     // Left motor PWM connect to pin 10
int sl = -1, sr = -1;     // Speed counter variable
int d = -1;     // Direction variable
static int flag = 0;    // Flag to void instant movement

//================================
//   some different motor speed
//================================
int ccw_100 = 1000;   // 100% speed counter-clockwise
int ccw_75 = 1125;    // 75% speed counter-clockwise
int ccw_50 = 1250;    // 50% speed counter-clockwise
int ccw_25 = 1375;    // 25% speed counter-clockwise
int ccw_5 = 1475;   // 5% speed counter-clockwise
int mid = 1500;     // Stop (middle position)
int clw_5 = 1525;     // 5% speed clockwise
int clw_25 = 1625;    // 25% speed clockwise
int clw_50 = 1750;    // 50% speed clockwise
int clw_75 = 1875;    // 75% speed clockwise
int clw_100 = 2000;   // Full speed clockwise


void setup()
{
  // Assign digital pins leftBump(8) and rightBump(2) as input
  pinMode(leftBump, INPUT);
  pinMode(rightBump, INPUT);

  // Assign digital pins leftLED(4) and rightLED(7) as output
  pinMode(leftLED, OUTPUT); 
  pinMode(rightLED, OUTPUT);

  // Connect digital pins leftMotor(9) and rightMotor(10) with L/R motors
  servoLeft.attach(leftMotor);
  servoRight.attach(rightMotor);
}


// Bot going forward
void go()
{
  // Both motors rotate full speed clockwise
  servoLeft.writeMicroseconds(clw_100);
  servoRight.writeMicroseconds(ccw_100);
}


// Given parameter time t, bot stop for t milisecond(ms)
void freeze(int t)
{
  // Both motors set to mid-point
  servoLeft.writeMicroseconds(mid);
  servoRight.writeMicroseconds(mid);

  // Keep current state for t ms
  delay(t);
}


// Given parameter time t, bot move forward to the left for t milisecond(ms)
void forLeft(int t)
{
  // Left motor rotates 5% speed clockwise
  servoLeft.writeMicroseconds(clw_5);
  // Right motor rotates 50% speed clockwise
  servoRight.writeMicroseconds(ccw_50);

  // Keep current state for t ms
  delay(t);

  sl = clw_5; 
  sr = ccw_50;
  // Left motor slowly speeds up to full power while
  // right motor remains full speed for a smooth speed change
  while (sl < clw_100 || sr > ccw_100)
      {
        servoLeft.writeMicroseconds(sl += 10);
        servoRight.writeMicroseconds(sr -= 5);
      }
}


// Given parameter time t, bot move forward to the right for t milisecond(ms)
void forRight(int t)
{
  // Left motor rotates 50% speed clockwise
  servoLeft.writeMicroseconds(clw_50);
  // Right motor rotates 5% speed clockwise 
  servoRight.writeMicroseconds(ccw_5);

  // Keep current state for t ms
  delay(t);
  
  sl = clw_50; 
  sr = ccw_5;
  // Right motor slowly speeds up to full power while
  // left motor remains full speed for a smooth speed change
  while (sl < clw_100 || sr > ccw_100)
      {
        servoLeft.writeMicroseconds(sl += 5);
        servoRight.writeMicroseconds(sr -= 10);
      }
}

// Given parameter time t, bot move backward for t milisecond(ms)
void backward(int t)
{
  // Both motors rotate 75% speed counter-clockwise
  servoLeft.writeMicroseconds(ccw_75);
  servoRight.writeMicroseconds(clw_75);

  // Keep current state for t ms
  delay(t);
}


// Main function that loops over and over
void loop()
{  
  // Set the bot to wait for 0.5s when it just start
  if (flag == 0)
  {
    delay(500);
    flag++;
  }
  
  // Initial car movement
  go();

  // It's hardly possible to have two 
  // When signal from any bumper were detected
  if (digitalRead(leftBump) == LOW || digitalRead(rightBump) == LOW)
  {
    // Stop and wait for 0.1s then do another signal check
    freeze(100);

    // Bot backup and turn to random direction if bumped on both sides
    if (digitalRead(leftBump) == LOW && digitalRead(rightBump) == LOW)
    {    
      // Both LEDs on
      digitalWrite(leftLED, HIGH);
      digitalWrite(rightLED, HIGH);
    
      //==============
      // Bot respond
      //==============
    
      // Stop for 300ms
      freeze(300);
      
      // Backup for 1.5s
      backward(1500);
    
      // Stop for 300ms
      freeze(300);
      
      // Generate a random number 0(left) or 1(right)
      d = random(0, 2);
    
      // Case of turning left
      if (d == 0)
        // Going forward to the left for 1s
        forLeft(1000);
    
      // Case of turning right
      else if (d == 1)
        // Going forward to the right for 1s
        forRight(1000);
      
      
      // Both LEDs off
      digitalWrite(leftLED, LOW);
      digitalWrite(rightLED, LOW);
    }

    // Bot backup and turn right if bumped on the left side
    else if (digitalRead(leftBump) == LOW)
    {
      // Left LED on
      digitalWrite(leftLED, HIGH);
  
      //==============
      // Bot respond
      //==============
      
      // Stop for 300ms
      freeze(300);
      
      // Backup for 1.5s
      backward(1500);
  
      // Stop for 300ms
      freeze(300);
      
      // Going forward to the right for 1s
      forRight(1000);
  
      // Turn off LED
      digitalWrite(leftLED, LOW);
    }
  
  
    // Bot backup and turn left if bumped on the right side
    else
    {
      // Right LED on
      digitalWrite(rightLED, HIGH);
  
      //==============
      // Bot respond
      //==============
      
      // Stop for 300ms
      freeze(300);
  
      // Backup for 1.5s
      backward(1500);
  
      // Stop for 300ms
      freeze(300);
  
      // Going forward to the left for 1s
      forLeft(1000);
  
      // Turn off LED
      digitalWrite(rightLED, LOW);
    }
  }
}
