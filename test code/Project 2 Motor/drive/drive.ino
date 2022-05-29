#define left1 19 //14
#define left2 5 //16 //pwm
#define right1 18 //13
#define right2 23 //15 //pwm

// setting PWM properties motor
const int freq = 8000;
const int Channel_0 = 0;
const int Channel_1 = 1;
const int Channel_2 = 2;
const int Channel_3 = 3;
const int resolution = 8;   //Resolution 8, 10, 12, 15
int battery = 20;   //calibration for battery voltage/speed. 

void setup() {
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  ledcSetup(Channel_0, freq, resolution);
  ledcAttachPin(left2, Channel_0);
  ledcSetup(Channel_1, freq, resolution);
  ledcAttachPin(right2, Channel_1);
stop(); //make sure motors are stopped

}

void loop() {
  //Arduino PWM is from 0 to 255
  //ccw (counter clockwise) will move the car forward
  //in the python examples this is positive numbers.
  //cw (clockwise) will move the car backward
  //in the python examples this is the negitive numbers.
  //PWM for left, right motor
 // ccw(100,100);
 // delay(2000);
 // cw(100,100);
 // delay(2000);
  ccw(50,255); //turn wide left forward
  delay(500);
  //cw(255,50); //trun wide right backward
  //delay(2000);
  ccw(255,50); //turn wide right forward
  delay(500);
  //cw(50,255); //turn wide left backward
  //delay(2000);
  ccw(0, 255); //turn tight left forward
  delay(500);
  ccw(0,255); //turn tight right forward
  delay(500);
  stop();
  delay(10000); //wait 10 seconds then start over

}

void cw(int lft, int rgt) { 
  //This causes the car to move backward
  digitalWrite(left1, 1);
  digitalWrite(right1, 1);
  ledcWrite(Channel_0, map(lft, 50, 255, 50, 255));
  ledcWrite(Channel_1, map(rgt, 50, 255, 50, 255));
 // analogWrite(left2, map(lft,50,255,50,255));
 // analogWrite(right2, map(rgt,50,255,50,255));  
}

void ccw(int lft, int rgt) {
  //This causes the car to move forward
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  ledcWrite(Channel_0, lft);
  ledcWrite(Channel_1, rgt);
  //analogWrite(left2, lft);
  //analogWrite(right2, rgt);
}

void stop() {
  digitalWrite(left1, 0);
  digitalWrite(right1, 0);
  ledcWrite(Channel_0, 0);
  ledcWrite(Channel_1, 0);
  //analogWrite(left2, 0);
  //analogWrite(right2, 0);
}
