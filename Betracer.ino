#include <SoftwareSerial.h>
//#include <Wire.h>
//#include <AFMotor.h>
//SoftwareSerial Bluetooth(0, 1); // Arduino(RX, TX) - HC-05 Bluetooth (TX, RX)

//**********5 Channel IR Sensor Connection**********//
#define ir1 A0
#define ir2 A1
#define ir3 A2
#define ir4 A3
#define ir5 A4

char data;


//*************Variables for Line Following***************//
int s1;  //Left Most Sensor
int s2;  //Left Sensor
int s3;  //Middle Sensor
int s4;  //Right Sensor
int s5;  //Right Most Sensor

//***********Distances for moving to avoid obstacle****************//
int horizontal = 20;
int vertical = 20;

//***********Sonar*************//
#define TRIG_PIN 52  //attach pin D3 Arduino to pin Trig of HC-SR04 (TBD)
#define ECHO_PIN 53  //attach pin D3 Arduino to pin Trig of HC-SR04 (TBD)

#define TRIG_PIN_LEFT 50  //attach pin D3 Arduino to pin Trig of HC-SR04 (TBD)
#define ECHO_PIN_LEFT 51  //attach pin D3 Arduino to pin Trig of HC-SR04 (TBD)

#define TRIG_PIN_RIGHT 48  //attach pin D3 Arduino to pin Trig of HC-SR04 (TBD)
#define ECHO_PIN_RIGHT 49  //attach pin D3 Arduino to pin Trig of HC-SR04 (TBD)

//*************Sonar*****************//
// HCSR04 hcsr04(TRIG_PIN, ECHO_PIN);
// HCSR04 hcsr04_left(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
// HCSR04 hcsr04_right(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT);

//*************Variables for Sonar*****************//
int sonar_center;
int sonar_left;
int sonar_right;
#define NEAR 20
#define FAR 40
int obstacle = 0;

//**********Front wheels***********//
#define IN11 7  // 0
#define IN12 8  // 0
#define IN13 6  // 1
#define IN14 4  // 1
#define ENA1 3  // 0
#define ENB1 5  // 1

//***********Back wheels*************//
#define IN21 9   //3
#define IN22 10   //3
#define IN23 11   //2
#define IN24 12   //2
#define ENA2 2   //3
#define ENB2 13  //2

#define C 18.85  // circumference of wheel
#define P 20     // pulses per revolution

//*******encoder pins and variables**********//
int encoder0 = 2;
int encoder1 = 3;
int pulseCount0 = 0;
int pulseCount1 = 0;

//*********variables for rpm calculation*******//
unsigned long thoigian;
unsigned long hientai;
int rpm;


void pulseUp0() {
  pulseCount0++;  // interrupt encoder 0
}
void pulseUp1() {
  pulseCount1++;  // interrupt encoder 1
}
//-------------------Calculating obstacle distance of Center, Left, Right-------------------
int ultra_center() {
  // Clears the trigPin condition
  long duration;  // variable for the duration of sound wave travel
  int distance;   // variable for the distance measurement
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.print(distance);
  // Serial.println(" cm");
  // delay(100);
  return distance;
}

int ultra_left() {
  // Clears the trigPin condition
  long duration;  // variable for the duration of sound wave travel
  int distance;   // variable for the distance measurement
  digitalWrite(TRIG_PIN_LEFT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_LEFT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN_LEFT, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  // Serial.print("Distance LEFT: ");
  // Serial.print(distance);
  // Serial.println(" cm");
  // delay(100);
  return distance;
}

int ultra_right() {
  long duration;  // variable for the duration of sound wave travel
  int distance;   // variable for the distance measurement
  // Clears the trigPin condition3
  digitalWrite(TRIG_PIN_RIGHT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_RIGHT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN_RIGHT, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  // Serial.print("Distance RIGHT: ");
  // Serial.print(distance);
  // Serial.println(" cm");
  // delay(100);
  return distance;
}

//DIRECTIONS
void moveBackward() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, HIGH);
  digitalWrite(IN13, HIGH);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN21, HIGH);
  digitalWrite(IN22, LOW);
  digitalWrite(IN23, HIGH);
  digitalWrite(IN24, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void moveSidewaysRight() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, HIGH);
  digitalWrite(IN13, HIGH);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, HIGH);
  digitalWrite(IN21, LOW);
  digitalWrite(IN22, HIGH);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void moveSidewaysLeft() {
  digitalWrite(IN11, HIGH);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, LOW);
  digitalWrite(IN14, HIGH);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, HIGH);
  digitalWrite(IN24, LOW);
  digitalWrite(IN21, HIGH);
  digitalWrite(IN22, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void rotateLeft() {
  digitalWrite(IN11, HIGH);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, LOW);
  digitalWrite(IN14, HIGH);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, HIGH);
  digitalWrite(IN21, LOW);
  digitalWrite(IN22, HIGH);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void rotateRight() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, HIGH);
  digitalWrite(IN13, HIGH);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, HIGH);
  digitalWrite(IN24, LOW);
  digitalWrite(IN21, HIGH);
  digitalWrite(IN22, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void moveRightForward() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, HIGH);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, HIGH);
  digitalWrite(IN21, LOW);
  digitalWrite(IN22, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void moveRightBackward() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, HIGH);
  digitalWrite(IN13, LOW);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, LOW);
  digitalWrite(IN21, LOW);
  digitalWrite(IN22, HIGH);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void moveLeftForward() {
  digitalWrite(IN11, HIGH);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, LOW);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, LOW);
  digitalWrite(IN21, HIGH);
  digitalWrite(IN22, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void moveLeftBackward() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, LOW);
  digitalWrite(IN14, HIGH);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, HIGH);
  digitalWrite(IN24, LOW);
  digitalWrite(IN21, LOW);
  digitalWrite(IN22, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void turnLeft() {
  digitalWrite(IN11, HIGH);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, LOW);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, HIGH);
  digitalWrite(IN21, LOW);
  digitalWrite(IN22, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void moveForward() {
  digitalWrite(IN11, HIGH);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, HIGH);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, HIGH);
  digitalWrite(IN21, HIGH);
  digitalWrite(IN22, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void turnRight() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, HIGH);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, LOW);
  digitalWrite(IN21, HIGH);
  digitalWrite(IN22, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void turnLeftFrontCenterICC() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, LOW);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, HIGH);
  digitalWrite(IN21, LOW);
  digitalWrite(IN22, HIGH);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
void stopMoving() {
  digitalWrite(IN11, LOW);
  digitalWrite(IN12, LOW);
  digitalWrite(IN13, LOW);
  digitalWrite(IN14, LOW);
  // analogWrite(ENA1, wheelSpeed);
  // analogWrite(ENB1, wheelSpeed);
  digitalWrite(IN21, LOW);
  digitalWrite(IN22, LOW);
  digitalWrite(IN23, LOW);
  digitalWrite(IN24, LOW);
  // analogWrite(ENA2, wheelSpeed);
  // analogWrite(ENB2, wheelSpeed);
}
//**needs refinement
void changeSpeed(int wheelSpeed) {
  analogWrite(ENA1, wheelSpeed - 10);  // compensate for misdirection
  analogWrite(ENB1, wheelSpeed);
  analogWrite(ENA2, wheelSpeed);
  analogWrite(ENB2, wheelSpeed);
}
void calculateRPM(int time) {

  thoigian = millis();

  if (thoigian - hientai >= 1000) {
    hientai = thoigian;
    rpm = (((pulseCount0 + pulseCount1) / 2) / 20) * 60;
    /*
         * Đĩa encoder có 20 xung, chúng ta đo được 120 xung/s
         * vậy lấy 120/20 = 6 vòng/s
         * ta được: 6*60 = số vòng quay / phút (RPM)
         */
    Serial.print("\t\t\t\t");
    Serial.print("Số xung/s: ");
    Serial.println((pulseCount0 + pulseCount1) / 2);
    pulseCount0 = 0;
    pulseCount1 = 0;
    Serial.print("\t\t\t\t");
    Serial.print("RPM: ");
    Serial.println(rpm);
  }
}
//-------------------Code do line -------------------------//
void followLine(int s1, int s2, int s3, int s4, int s5) {
  //if only middle sensor detects black line

  // state = 0
  // only 3 sensors in the middle
  if ((s1 == 0) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 0)) {
    //going forward with full speed
    moveForward();
  }

  // state = 1
  if ((s1 == 0) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 0)) {
    //going forward with full speed
    turnRight();
  }

  // state = 2
  if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 1)) {
    //going slightly left
    turnRight();
  }

  if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 1)) {
    //going slightly left
    rotateRight();
  }

  // state = -1
  if ((s1 == 0) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 0)) {
    //going left with full speed
    turnLeft();
  }

  // // state = -2
  if ((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 0)) {
    //going left with full speed
    turnLeft();
  }

  // state = -3
  if ((s1 == 1) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0)) {
    //going left with full speed
    rotateLeft();
  }
  // nothing
  if ((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0)) {
    stopMoving();
  }
  // if ((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1))
  // {
  //   changeSpeed(128);
  //   moveForDistance(200, "leftForward");
  //   delay(100);
  //   moveForDistance(35, "turnLeftFrontCenterICC");
  //   delay(100);
  //   moveForDistance(150, "forward");
  // }
}

void moveForDistance(int distance, String direct) {
  pulseCount0 = 0;
  pulseCount1 = 0;
  int pulses = distance / C * P;

  if (direct == "moveForward") {
    while (((pulseCount0 + pulseCount1) / 2) < pulses) {  // if average pulse of 2 encoder is < 20
      moveForward();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "moveBackward") {
    while (((pulseCount0 + pulseCount1) / 2) < pulses) {  // if average pulse of 2 encoder is < 20
      moveBackward();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "moveSidewaysRight") {
    while (((pulseCount0 + pulseCount1) / 2) < pulses) {  // if average pulse of 2 encoder is < 20
      moveSidewaysRight();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "moveSidewaysLeft") {
    while (((pulseCount0 + pulseCount1) / 2) < pulses) {  // if average pulse of 2 encoder is < 20
      moveSidewaysLeft();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "rotateLeft") {
    while (((pulseCount0 + pulseCount1) / 2) < pulses) {  // if average pulse of 2 encoder is < 20
      rotateLeft();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "rotateRight") {
    while (((pulseCount0 + pulseCount1) / 2) < pulses) {  // if average pulse of 2 encoder is < 20
      rotateRight();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "moveRightForward") {
    while (pulseCount1 < pulses) {  // if average pulse of 2 encoder is < 20
      moveRightForward();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "moveRightBackward") {
    while (pulseCount0 < pulses) {  // if average pulse of 2 encoder is < 20
      moveRightBackward();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "moveLeftForward") {
    while (pulseCount0 < pulses) {  // if average pulse of 2 encoder is < 20
      moveLeftForward();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "moveLeftBackward") {
    while (pulseCount1 < pulses) {  // if average pulse of 2 encoder is < 20
      moveLeftBackward();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "turnLeft") {
    while (((pulseCount0 + pulseCount1) / 2) < pulses) {  // if average pulse of 2 encoder is < 20
      turnLeft();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "turnRight") {
    while (pulseCount1 < pulses) {  // if average pulse of 2 encoder is < 20
      turnRight();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else if (direct == "turnLeftFrontCenterICC") {
    while (pulseCount1 < pulses) {  // if average pulse of 2 encoder is < 20
      turnLeftFrontCenterICC();
      delay(5);
      Serial.println();
    }
    stopMoving();
  } else {
    Serial.println("No/Wrong direction provided!");
    stopMoving();
  }
}
// AVOID*****
void avoidRight() {
  sonar_left = ultra_left();
  if (sonar_left < NEAR) {  // too close to Obstacle
    Serial.println("Too close");
    moveForDistance(horizontal, "moveSidewaysRight");

    // moveRightForward();

  } else if (sonar_left > FAR) {  // too FAR from Obstacle
    Serial.println("Too far");
    moveForDistance(horizontal, "moveSidewaysLeft");
    // moveLeftForward();

  } else {  // Obstacle within range
    moveForDistance(vertical, "moveForward");
    // moveForward();
  }
  //Truong hop di ve dung line thi thoat che do obstacle, bat dau do line
  s1 = digitalRead(ir1);  //Left Most Sensor
  s2 = digitalRead(ir2);  //Left Sensor
  s3 = digitalRead(ir3);  //Middle Sensor
  s4 = digitalRead(ir4);  //Right Sensor
  s5 = digitalRead(ir5);  //Right Most Sensor
  if (s1 || s2 || s3 || s4 || s5) {
    Serial.println("LINE FOUND!!!");
    obstacle = 0;
  }
}

//-----Ne ve ben trai truong hop right<left --------//
void avoidLeft() {
  sonar_left = ultra_left();
  sonar_right = ultra_right();
  if (sonar_right < NEAR) {  // too close to Obstacle
    Serial.println("Too close");
    moveForDistance(horizontal, "moveSidewaysLeft");
    if (sonar_center < 40) {
      moveForDistance(vertical, "moveForward");
    }
    // moveLeftForward();

  } else if (sonar_right > FAR) {  // too FAR from Obstacle
    Serial.println("Too far");
    moveForDistance(horizontal, "moveSidewaysRight");
    // moveRightForward();

  } else {  // Obstacle within range
    moveForDistance(vertical, "moveForward");
    // moveForward();
  }
  //Truong hop di ve dung line thi thoat che do obstacle, bat dau do line
  s1 = digitalRead(ir1);  //Left Most Sensor
  s2 = digitalRead(ir2);  //Left Sensor
  s3 = digitalRead(ir3);  //Middle Sensor
  s4 = digitalRead(ir4);  //Right Sensor
  s5 = digitalRead(ir5);  //Right Most Sensor
  if (s1 || s2 || s3 || s4 || s5) {
    Serial.println("LINE FOUND!!!");
    obstacle = 0;
  }
}
//-------Do line -----------
void checkForLine() {
  // check for line
  s1 = digitalRead(ir1);  //Left Most Sensor
  s2 = digitalRead(ir2);  //Left Sensor
  s3 = digitalRead(ir3);  //Middle Sensor
  s4 = digitalRead(ir4);  //Right Sensor
  s5 = digitalRead(ir5);  //Right Most Sensor
  // Serial.println("Gia tri cua cac do line: ");
  Serial.print("s1: ");
  Serial.println(s1);
  Serial.print("s2: ");
  Serial.println(s2);
  Serial.print("s3: ");
  Serial.println(s3);
  Serial.print("s4: ");
  Serial.println(s4);
  Serial.print("s5: ");
  Serial.println(s5);
  // if (s1!=0 || s2!=0 || s3!=0 || s4!=0 || s5!=0)
  // {
  //   obstacle=0;
  //    followLine(s1,s2,s3,s4,s5);
  // }
  //if (s1 || s2 || s3 || s4 || s5) {  //if này lỗi vì nếu cả 5 chân đều off thì nó ko vào vòng if này và xe sẽ ko chạy cái movestop
  //obstacle = 0;
  followLine(s1, s2, s3, s4, s5);
  //break;
  //}
}

void stopsign(){
  stopMoving();  
}

void TurnLeftSign(){ 
  moveForward(); 
  delay(500);
  rotateLeft();
  delay(600);
  moveForward();
  delay(5000);        
}

void TurnRightSign(){ 
  moveForward(); 
  delay(500);
  rotateRight();
  delay(600);
  moveForward();
  delay(5000);        
}

void setup() {
  Serial.begin(9600);

  // Set up bluetooth
  //Bluetooth.begin(9600);
  //Bluetooth.begin(9600);
  //Bluetooth.setTimeout(1);

  // ultrasonic
  //Center
  // pinMode(TRIG_PIN, OUTPUT);  // Sets the TRIG_PIN of as an OUTPUT
  // pinMode(ECHO_PIN, INPUT);   // Sets the ECHO_PIN as an INPUT
  // //Left
  // pinMode(TRIG_PIN_LEFT, OUTPUT);  // Sets the TRIG_PIN of as an OUTPUT
  // pinMode(ECHO_PIN_LEFT, INPUT);   // Sets the ECHO_PIN as an INPUT
  // //Right
  // pinMode(TRIG_PIN_RIGHT, OUTPUT);  // Sets the TRIG_PIN of as an OUTPUT
  // pinMode(ECHO_PIN_RIGHT, INPUT);   // Sets the ECHO_PIN as an INPUT

  // Set up Encoder
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(encoder0, INPUT);
  pinMode(encoder1, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0), pulseUp0, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder1), pulseUp1, RISING);

  // Wheel 0 and 1 pins
  pinMode(IN11, OUTPUT);
  pinMode(IN12, OUTPUT);
  pinMode(IN13, OUTPUT);
  pinMode(IN14, OUTPUT);
  pinMode(ENA1, OUTPUT);
  pinMode(ENB1, OUTPUT);

  // Wheel 2 and 3 pins
  pinMode(IN21, OUTPUT);
  pinMode(IN22, OUTPUT);
  pinMode(IN23, OUTPUT);
  pinMode(IN24, OUTPUT);
  pinMode(ENA2, OUTPUT);
  pinMode(ENB2, OUTPUT);

  // Line follower sensors
  // pinMode(ir1, INPUT);
  // pinMode(ir2, INPUT);
  // pinMode(ir, INPUT);
  // pinMode(ir4, INPUT);
  // pinMode(ir5, INPUT);
  changeSpeed(130);
  // changeSpeed(150);
  // moveForward();
  // delay(500);
  // //Rotate 90 degree to the right
  // rotateRight();
  // delay(430);
  // moveForward();
  // delay(500);
  // Roate 90 degree to the left
  // rotateLeft();
  // delay(590);
  // moveForward();
  // delay(1000);
  // stopMoving();
  // rotateRight();
  // delay(430);
}

void followLine() {
  // calculateRPM(1000); // print pulse count and speed every 1 second
  // delay(5);

  // // // Reading Sensor Values
  sonar_center = ultra_center();
  sonar_left = ultra_left();
  sonar_right = ultra_right();
  // //-----DEBUG-----//
  // // Serial.println(sonar_left);
  // // Serial.println(sonar_center);
  // // Serial.println(sonar_right);
  // // Serial.println();

  // //------Detecting obstacle -------//
  if (sonar_center < 40) {  // yes obstacle
    obstacle = 1;

    if (sonar_left < sonar_right) {  // obstacle on left => move right
      Serial.println("Avoid to right");
      moveForDistance(10, "moveSidewaysRight");
      while (obstacle == 1) {
        delay(5);
        avoidRight();
      }

    } else {  // obstacle on right => move left + check right
      Serial.println("Avoid to left");
      moveForDistance(10, "moveSidewaysLeft");
      while (obstacle == 1) {
        delay(5);
        avoidLeft();
      }
    }
  } else {  // no obstacle
    // Serial.println("BACK ON LINE BABYYYYY");
    checkForLine();
  }
}



void loop() {
// moveForward();
// delay(10000);

  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.print(data);

    if (data == 'a') {
      moveForward();
      delay(80);
      stopMoving();
    } else if (data == 'b') {
      turnLeft();
      delay(80);
      stopMoving();
    } else if (data == 'c') {
      Serial.println(data);
      rotateLeft();
      delay(80);
      stopMoving();
    } else if (data == 'd') {
      Serial.println(data);
      rotateLeft();
      delay(80);
      stopMoving();
    } else if (data == 'e') {
      Serial.println(data);
      turnRight();
      delay(80);
      stopMoving();
    } else if (data == 'f') {
      Serial.println(data);
      rotateRight();
      delay(80);
      stopMoving();
    } else if (data == 'g') {
      Serial.println(data);
      rotateRight();
      delay(80);
      stopMoving();
    } else if (data == 'h') {
      Serial.println(data);
      stopMoving();
      delay(80);
      stopMoving();
    } else if (data == 'l') {
      Serial.println(data);
      TurnLeftSign();            
    } else if (data == 'r'){
      Serial.println(data);
      TurnRightSign();            
    } else if (data == 's'){
      Serial.println(data);
      stopMoving();       
    }
  }
}
