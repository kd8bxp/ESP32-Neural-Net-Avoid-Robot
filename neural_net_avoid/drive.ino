void drive()  //  start driving
{
  int cal = 5;
  set_leds();
  switch(out)
  {
    case 0: cw(255, 0); /*cw(150, 50);*/ delay(350); break; //left - go to right backward //cw(50, 150); delay(350);  break; //left wide backward
    case 1: cw(150,50); /*cw(255, 0);*/ delay(50); break; //left - go a little to right backward//cw(0,255); delay(50);   break;  //left soft
    case 2: ccw(150, 150);   break; //forward
    case 3: cw(0,255); /*cw(50,150);*/ delay(350); break; //right got to left backward //cw(150,50); delay(350); break; //right wide backward
    case 4: cw(50,150); /*cw(0, 255);*/ delay(50); break; //right goto a little left backward //cw(255,0); delay(50); break; //right soft
    //case 5: digitalWrite(motA_1, LOW); digitalWrite(motA_2, HIGH);  digitalWrite(motB_1, LOW); digitalWrite(motB_2, HIGH);ledcWrite(Channel_0, 170); ledcWrite(Channel_1, 170);   break;   
  }
}

void cw(int lft, int rgt) { 
  //This causes the car to move backward
  digitalWrite(motA_1, 1);
  digitalWrite(motB_1, 1);
  ledcWrite(Channel_0, map(lft, 50, 255, 50, 255));
  ledcWrite(Channel_1, map(rgt, 50, 255, 50, 255));
 // analogWrite(left2, map(lft,50,255,50,255));
 // analogWrite(right2, map(rgt,50,255,50,255));  
}

void ccw(int lft, int rgt) {
  //This causes the car to move forward
  digitalWrite(motA_1, 0);
  digitalWrite(motB_1, 0);
  ledcWrite(Channel_0, lft);
  ledcWrite(Channel_1, rgt);
  //analogWrite(left2, lft);
  //analogWrite(right2, rgt);
}

void stop() {
  digitalWrite(motA_1, 0);
  digitalWrite(motB_1, 0);
  ledcWrite(Channel_0, 0);
  ledcWrite(Channel_1, 0);
  //analogWrite(left2, 0);
  //analogWrite(right2, 0);
}
