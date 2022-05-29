/*
Copyright (c) 2018 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/

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
  ccw(100,100);
  delay(2000);
  cw(100,100);
  delay(2000);
  ccw(50,255); //turn wide left forward
  delay(2000);
  cw(255,50); //trun wide right backward
  delay(2000);
  ccw(255,50); //turn wide right forward
  delay(2000);
  cw(50,255); //turn wide left backward
  delay(2000);
  ccw(255,0); //turn tight right forward
  delay(500);
  ccw(0,255);
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
