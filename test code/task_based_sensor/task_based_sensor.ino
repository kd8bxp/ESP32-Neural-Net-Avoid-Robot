#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif


// ultrasonic SR04 sensor pins
#define sensor_A_out P20 
#define sensor_A_in  P19 
#define sensor_B_out P12 
#define sensor_B_in  P11 


void TaskSensors( void *pvParameters );

void setup() {
 Serial.begin(115200);
  Serial.println("Start");   
pinMode(5, OUTPUT);
digitalWrite(5, LOW);
pinMode(sensor_A_out, OUTPUT);
  pinMode(sensor_B_out, OUTPUT);
  pinMode(sensor_A_in, INPUT);
  pinMode(sensor_B_in, INPUT);
 // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskSensors
    ,  "TaskSensors"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);


}

void loop() {
  // put your main code here, to run repeatedly:

}

void TaskSensors(void *pvParameters) {
  for (;;) {
    digitalWrite(sensor_A_out, LOW); delayMicroseconds(2); digitalWrite(sensor_A_out, HIGH);  delayMicroseconds(10); digitalWrite(sensor_A_out, LOW); int duration1 = pulseIn(sensor_A_in, HIGH); 
  delay(5); 
  digitalWrite(sensor_B_out, LOW); delayMicroseconds(2); digitalWrite(sensor_B_out, HIGH);  delayMicroseconds(10); digitalWrite(sensor_B_out, LOW); int duration2 = pulseIn(sensor_B_in, HIGH);
  // convert ToF to distance in cm
  double distance1 = duration1/58.3; 
  double distance2 = duration2/58.3;  
  double result1 = distance1;
  double result2 = distance2;
 // if(distance1 < 250 || distance2 < 250)
 // {
    Serial.print(" A= "); Serial.print(result1);
    Serial.print(" B= "); Serial.println(result2);
  delay(500);
  //}
  }
}
