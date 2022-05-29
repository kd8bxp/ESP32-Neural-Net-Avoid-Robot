// ultrasonic SR04 sensor pins
#define sensor_A_out P20 //12
#define sensor_A_in  P19 //14
#define sensor_B_out P12 //12
#define sensor_B_in  P11 //14

void setup() {
  Serial.begin(115200);
  
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(sensor_A_out, OUTPUT);
  pinMode(sensor_B_out, OUTPUT);
  pinMode(sensor_A_in, INPUT);
  pinMode(sensor_B_in, INPUT);
}


void loop() {
  sensor();
  delay(1000);
  
}

void sensor() {
  
  digitalWrite(sensor_A_out, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_A_out, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor_A_out, LOW);
  int duration1 = pulseIn(sensor_A_in, HIGH);  
  double distance1 = duration1/58.3; 
  double result1 = distance1;
  Serial.print("A: "); Serial.println(result1, 5);

  digitalWrite(sensor_B_out, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_B_out, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor_B_out, LOW);
  int duration2 = pulseIn(sensor_B_in, HIGH);
  double distance2 = duration2/58.3;
  double result2 = distance2;
  Serial.print("B: "); Serial.println(result2, 5);
}
