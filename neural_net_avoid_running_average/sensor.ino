void sensor()   // make a measurement
{  
  remeasure:  
  result1.clear();
  result2.clear();
  for (int ra=0; ra<10; ra++) {
  digitalWrite(sensor_A_out, LOW); delayMicroseconds(2); digitalWrite(sensor_A_out, HIGH);  delayMicroseconds(10); digitalWrite(sensor_A_out, LOW); int duration1 = pulseIn(sensor_A_in, HIGH); 
  delay(5); 
  digitalWrite(sensor_B_out, LOW); delayMicroseconds(2); digitalWrite(sensor_B_out, HIGH);  delayMicroseconds(10); digitalWrite(sensor_B_out, LOW); int duration2 = pulseIn(sensor_B_in, HIGH);
  // convert ToF to distance in cm
  double distance1 = duration1/58.3; 
  double distance2 = duration2/58.3;  
  
  result1.addValue(distance1);
  result2.addValue(distance2);
    
  if(result1.getAverage() < 250 || result2.getAverage() < 250)
  {
    cells_in[input_A_toCell] = result1.getAverage()/1000;  Serial.print(" A= "); Serial.print(cells_in[input_A_toCell],5);
    cells_in[input_B_toCell] = result2.getAverage()/1000;  Serial.print(" B= "); Serial.println(cells_in[input_B_toCell],5);
  }
  else
  {
    goto remeasure;   
  }
  }   
}
