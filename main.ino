#include <Arduino.h>

#include <Servo.h>
/* L9110s 모터드라이버
  오른쪽모터
  L9110s A_1A D6
  L9110s A_1B D11
  왼쪽모터
  L9110s B_1A D3
  L9110s B_1B D5
  float getDistance() {
  digitalWrite(echoPin, LOW);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // 거리값을 저장합니다.
  float distance = pulseIn(echoPin, HIGH) / 29.0 / 2.0;
  return distance;
}
*/

#define irLeft analogRead(A0)
#define irRight analogRead(A1)

int A_1A = 3;
int A_1B = 4;
int B_1A = 11;
int B_1B = 12;
/*
  초음파센서
  초음파센서 TRIG - D8
  초음파센서 ECHO - D9
*/
int trigPin = 7;

int echoPin = 6;
long leftDistance = 0;
long rightDistance = 0;
int distance = 0;
boolean object;
/*
  서보모터
  서보모터 OUT - D10
*/
int servoPin = 8;
Servo servo;
int speed = 120;
long duration = 0;

/**
  왼쪽,오른쪽 모터를 정회전하여 전진합니다.
*/
void forward() {
  //모터A 정회전
  analogWrite(A_1A, speed);
  analogWrite(A_1B, 0);
  //모터B 정회전
  analogWrite(B_1A, speed);
  analogWrite(B_1B, 0);
}
/**
  왼쪽,오른쪽 모터를 역회전하여 후진합니다.
*/
void back() {
  //모터A 역회전
  analogWrite(A_1A, 0);
  analogWrite(A_1B, speed+150);
  //모터B 역회전
  analogWrite(B_1A, 0);
  analogWrite(B_1B, speed+150);
}
/**
  오른쪽 모터를 정지, 왼쪽 모터를 정회전하여 우회전합니다.
*/
void right() {
  //모터A 정회전
  analogWrite(A_1A, speed+150);
  analogWrite(A_1B, 0);
  //모터B 역회전
  analogWrite(B_1A, 0);
  analogWrite(B_1B, speed+150);
}
/**
  오른쪽 모터를 정회전, 왼쪽모터를 정지하여 좌회전합니다.
*/
void left() {
  //모터A 역회전
  analogWrite(A_1A, 0);
  analogWrite(A_1B, speed+150);
  //모터B 정회전
  analogWrite(B_1A, speed+150);
  analogWrite(B_1B, 0);
}
/**
  오른쪽,왼쪽모터를 모두 정지합니다.
*/
void stoop() {
  //모터A 정지
  analogWrite(A_1A, 0);
  analogWrite(A_1B, 0);
  //모터B 정지
  analogWrite(B_1A, 0);
  analogWrite(B_1B, 0);
}

int lookLeft () {
  //lock left
  servo.write(90);
  delay(500);
  leftDistance = getDistanceCM();
  delay(100);
  servo.write(90);
  Serial.print("Left:");
  Serial.print(leftDistance);
  return leftDistance;
  delay(100);
}

float getDistanceCM() {

  digitalWrite(echoPin, LOW);

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // 거리값을 저장합니다.

  float distance = pulseIn(echoPin, HIGH) / 29.0 / 2.0;

  return distance;

}

int lookRight() {
  //lock right
  servo.write(90);
  delay(500);
  rightDistance = getDistanceCM();
  delay(100);
  servo.write(90);
  Serial.print("   ");
  Serial.print("Right:");
  Serial.println(rightDistance);
  return rightDistance;
  delay(100);
/**
  초음파센서를 이용해서 벽까지의 거리(mm)를 구합니다.
*/
}
void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  //핀을 초기화합니다.
  //L298 모터드라이버의 핀들을 출력으로 변경합니다.
  pinMode(A_1A, OUTPUT);
  pinMode(A_1B, OUTPUT);
  pinMode(B_1A, OUTPUT);
  pinMode(B_1B, OUTPUT);
  digitalWrite(A_1A, LOW);
  digitalWrite(A_1B, LOW);
  digitalWrite(B_1A, LOW);
  digitalWrite(B_1B, LOW);
  //초음파센서를 초기화합니다.
  pinMode(trigPin, OUTPUT); // Trigger is an output pin
  pinMode(echoPin, INPUT); // Echo is an input pin
  // 서보모터를 연결합니다.
  // 주의: 서보모터를 사용하게되면, 자동으로 9,10번핀의 analogWrite기능이 비활성화 됩니다.
  servo.attach(servoPin);
  //초기에 서보를 정면을 봅니다
  servo.write(90);
  delay(100);
}

void objectAvoid() {
  distance =getDistanceCM();
  if (distance <= 15) {
    //stop
    stoop();
    Serial.println("Stop1");

    lookLeft();
    lookRight();
    delay(100);
    if (rightDistance <= leftDistance) {
      //left
      object = true;
      left();
      Serial.println("moveLeft");
    } else {
      //right
      object = false;
      right();
      Serial.println("moveRight");
    }
    delay(100);
  }
  else {
    //forword
    Serial.println("moveforword");
    forward();
  }
}


void objectAvoid2() {
   forward();     

    // 서보모터를 좌회전하여 왼쪽 벽과의 거리를 변수에 저장합니다.

    servo.write(145);

    delay(100);

    leftDistance = getDistanceCM();

    delay(500);

    // 서보를 우회전하여 오른쪽 벽과의 거리를 변수에 저장합니다.

    servo.write(45);

    delay(100);

    rightDistance = getDistanceCM();

    delay(500);

    // 서보모터를 중앙으로 움직입니다.

    //servo.write(90);

    //delay(100);

    // 0.3초간 후진합니다.

    

     if (getDistanceCM() < 30 ) {

    //0.5초간 정지합니다.

    stoop();

    delay(500);

    // 서보모터를 좌회전하여 왼쪽 벽과의 거리를 변수에 저장합니다.

    servo.write(145);

    delay(500);

    leftDistance = getDistanceCM();

    delay(500);

    // 서보를 우회전하여 오른쪽 벽과의 거리를 변수에 저장합니다.

    servo.write(45);

    delay(500);

    rightDistance = getDistanceCM();

    delay(500);

    // 서보모터를 중앙으로 움직입니다.

    servo.write(90);

    delay(1000);

    // 0.3초간 후진합니다.

    back();

    delay(500);

    // 이전에 저장한 왼쪽거리가 오른쪽거리 보다 크다면 0.5초간 좌회전하며, 작다면 0.5초간 우회전합니다.

    if (leftDistance > rightDistance) {

      left();

    } else {

      right();

    }

    delay(1000);
    
  }
forward();
}
 

void loop() {
  //20cm이내에 벽이 있다면 아래의 코드를 실행합니다.
  if (getDistanceCM() >= 20 ){
    
   if (irLeft >=400  && irRight >= 300 ) {
     Serial.println("Forward");
      Serial.println(irLeft);
      Serial.println(irRight);
      forward();
  }

   
   else if (irLeft >= 400 && irRight < 300 ) {
   Serial.println("TL");
    Serial.println(irLeft);
      Serial.println(irRight);
      stoop();
      delay(100);
      back();
      delay(700);
      left();
      delay(1000);
 }
 else if (irLeft < 400 && irRight >= 300 ) {

   Serial.println("TR");
    Serial.println(irLeft);
      Serial.println(irRight);
      stoop();
      delay(100);
      back();
      delay(700);
      right();
      delay(1000);
  
 }
 else if (irLeft < 400 && irRight < 300 ) {
   //Stop
     Serial.println("stop");
     Serial.println(irLeft);
      Serial.println(irRight);
      stoop();
      delay(500);
       forward();
      delay(500);
      stoop();
      delay(500);

      }}
   
