//모터A 컨트롤
int IN1=3;
int IN2=4;
int IN3=11;
int IN4=12;
int speed = 120;
 
void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
}
 
void loop() {
  StartA();
  delay(3000);
  
}
 
//모터A,B 정회전
void StartA()
{
   analogWrite(IN1, speed);
  analogWrite(IN2, 0);
   analogWrite(IN3, speed);
  analogWrite(IN4, 0);
}
 
//모터A,B Stop
void StopA()
{
    digitalWrite(IN1,0);
    digitalWrite(IN2,0);
}
