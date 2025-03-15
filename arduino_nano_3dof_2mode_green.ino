#include <PS2X_lib.h>
#include <Servo.h>

PS2X ps2x;  // Create an instance of the PS2 controller library
Servo servo1;
Servo servo2;
Servo servo3;

const int MODE_PIN = 11;  // connect jumper wire to pin S11 and V11 for controller mode
const int MODE_REF = 10;  // connect jumper wire to pin S11 and S10 for normal mode

int angle1 = 90;  // default angle
int angle2 = 90;  // default angle
int angle3 = 90;  // default angle

constexpr int maxAngle = 110;
constexpr int minAngle = 70;

int anglePerStep = 2;         // max is 4, set the angle to move the servo at each interval
int holdDelay = 20;           // Time interval for continuous movement when holding a button, make sure interval is smooth
int joystickThreshold = 100;  // Sensitivity threshold for joystick movement, to make sure angle input is less than certain amount

void setup() {                // run only once when the program start or microcontroller first power up
  pinMode(MODE_PIN, INPUT);   // setup pin for jumper
  pinMode(MODE_REF, OUTPUT);  // setup pin for jumper to connect to for default LOW
  Serial.begin(9600);
  delay(500);  // Small delay for stability

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  int error = ps2x.config_gamepad(A4, A2, A3, A1, false, false);

  // check for connection with controller DO NOT CHANGE
  if (error == 0) {
    Serial.println("✅ PS2 Controller connected successfully!");
  } else if (error == 1) {
    Serial.println("❌ No controller found.");
  } else if (error == 2) {
    Serial.println("⚠ Controller found but not accepting commands.");
  } else if (error == 3) {
    Serial.println("⚠ Controller refusing to enter Pressures mode.");
  } else {
    Serial.println("❓ Unknown error.");
  }

  // set up pin for each servo
  servo1.attach(4);
  servo2.attach(5);
  servo3.attach(6);

  // write default angle to servos
  servo1.write(angle1);
  servo2.write(angle2);
}

void loop() {
  if (digitalRead(MODE_PIN) == HIGH) {  // jumper connected to the mode pin
    controllerMode();
  } else {
    Serial.println("Normal mode...");
    normalMode();
  }
}

void controllerMode() {  // use ps2 controller to move servo
  ps2x.read_gamepad(false, false);


  // RESET both angles when "Select" is pressed
  if (ps2x.Button(PSB_SELECT)) {
    angle1 = 90;
    angle2 = 90;
    angle3 = 90;
    Serial.println("SELECT button pressed - Resetting both angles to 90");
  }

  // -----Notes-----
  //ps2x.Analog(PSS_LX) read value of left joystick, default in between 0 and 255

  // PSB_SQUARE

  // D-pad UP or Joystick UP - Increase angle1
  if (ps2x.Button(PSB_PAD_UP) || ps2x.Analog(PSS_LY) < 128 - joystickThreshold || ps2x.Button(PSB_TRIANGLE) || ps2x.Analog(PSS_RY) < 128 - joystickThreshold) {
    // if angle still under our servo limit of 135 degree
    if (angle1 < maxAngle && angle2 > minAngle && angle3 > minAngle) {
      angle1 += anglePerStep;
      angle2 -= anglePerStep;
      angle3 -= anglePerStep;
    }
  }

  // D-pad DOWN or Joystick DOWN - Decrease angle1
  if (ps2x.Button(PSB_PAD_DOWN) || ps2x.Analog(PSS_LY) > 128 + joystickThreshold || ps2x.Button(PSB_CROSS) || ps2x.Analog(PSS_RY) > 128 + joystickThreshold) {
    if (angle1 > minAngle && angle2 < maxAngle && angle3 < maxAngle) {
      angle1 -= anglePerStep;
      angle2 += anglePerStep;
      angle3 += anglePerStep;
    }
  }


  // D-pad LEFT or Joystick LEFT - Increase angle2
  if (ps2x.Button(PSB_PAD_LEFT) || ps2x.Analog(PSS_LX) < 128 - joystickThreshold || ps2x.Button(PSB_SQUARE) || ps2x.Analog(PSS_RX) < 128 - joystickThreshold) {
    if (angle2 > minAngle && angle3 < maxAngle) {
      angle2 -= anglePerStep;
      angle3 += anglePerStep;
    }
  }

  // D-pad RIGHT or Joystick RIGHT - Decrease angle2
  if (ps2x.Button(PSB_PAD_RIGHT) || ps2x.Analog(PSS_LX) > 128 + joystickThreshold || ps2x.Button(PSB_CIRCLE) || ps2x.Analog(PSS_RX) > 128 + joystickThreshold) {
    if (angle3 > minAngle && angle2 < maxAngle) {
      angle2 += anglePerStep;
      angle3 -= anglePerStep;
    }
  }

  // L1
  if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) {
    if (angle1 < maxAngle && angle2 < maxAngle && angle3 < maxAngle) {
      angle1 += anglePerStep;
      angle2 += anglePerStep;
      angle3 += anglePerStep;
    }
  }

  // L2
  if (ps2x.Button(PSB_L2) || ps2x.Button(PSB_R2)) {
    if (angle1 > minAngle && angle2 > minAngle && angle3 >  minAngle) {
      angle1 -= anglePerStep;
      angle2 -= anglePerStep;
      angle3 -= anglePerStep;
    }
  }


  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);

  delay(holdDelay);  // Add delay to allow smooth movement on long press
}

// pattern mode, set hard coded angle
void normalMode() {
  int delayTime = 800;  // delay time for new pattern cycle in milisecond

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  delay(delayTime);


  servo1.write(130);
  servo2.write(130);
  servo3.write(130);
  delay(delayTime);

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  delay(delayTime);

  servo1.write(50);
  servo2.write(50);
  servo3.write(50);
  delay(delayTime);

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  delay(delayTime);

  servo1.write(50);
  servo2.write(130);
  servo3.write(130);
  delay(delayTime);

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  delay(delayTime);

  servo1.write(130);
  servo2.write(50);
  servo3.write(50);
  delay(delayTime);

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  delay(delayTime);

  servo1.write(90);
  servo2.write(130);
  servo3.write(50);
  delay(delayTime);

  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  delay(delayTime);

  servo1.write(90);
  servo2.write(50);
  servo3.write(130);
  delay(delayTime);
}