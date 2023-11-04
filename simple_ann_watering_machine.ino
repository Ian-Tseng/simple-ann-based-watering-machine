int active_count= 0;
int hour_count= 0;
int duration_time= 300000; // 300000= 5 min
void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(7, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  float sensorA0Value = analogRead(A0);
  float sensorA1Value = analogRead(A1);
  float sensorA2Value = analogRead(A2);
  float sensorA3Value = analogRead(A3);
  float norSensorA0Value = 1- sensorA0Value / 1000;
  float norSensorA1Value = 1 - sensorA1Value / 1000;
  float norSensorA2Value = 1 - sensorA2Value / 1000;
  float norSensorA3Value = 1 - sensorA3Value / 1000;
  Serial.println("Current sensor value");
  Serial.print(norSensorA0Value);
  Serial.print(norSensorA1Value);
  Serial.print(norSensorA2Value);
  Serial.print(norSensorA3Value);
  digitalWrite(7, HIGH);

}
void update_sensors_value() {
   // put your main code here, to run repeatedly:
  float sensorA0Value = analogRead(A0);
  float sensorA1Value = analogRead(A1);
  float sensorA2Value = analogRead(A2);
  float sensorA3Value = analogRead(A3);

  float norSensorA0Value = 1 - sensorA0Value / 1000;
  float norSensorA1Value = 1 - sensorA1Value / 1000;
  float norSensorA2Value = 1 - sensorA2Value / 1000;
  float norSensorA3Value = 1 - sensorA3Value / 1000;

 // float activate_thre_value = 0.25;
  float activate_thre_value = (norSensorA0Value+ norSensorA1Value+ norSensorA2Value+ norSensorA3Value) /4;
  float inactivate_thre_value = 0.6;
  float err_A0 = activate_thre_value - norSensorA0Value;
  float err_A1 = activate_thre_value - norSensorA1Value;
  float err_A2 = activate_thre_value - norSensorA2Value;
  float err_A3 = activate_thre_value - norSensorA3Value;

  float weight_A0 = norSensorA0Value * activate_thre_value;
  float weight_A1 = norSensorA1Value * activate_thre_value;
  float weight_A2 = norSensorA2Value * activate_thre_value;
  float weight_A3 = norSensorA3Value * activate_thre_value;
  float value = weight_A0 * norSensorA0Value + weight_A1 * norSensorA1Value + weight_A2 * norSensorA2Value+ weight_A3 * norSensorA3Value- err_A0 - err_A1 - err_A2- err_A3;
  
  float sig_val = 1 / (1 + pow(2.71828182846, -value));
  
  
  Serial.println("sig_val");
  Serial.print(sig_val);
  bool active=false;
  int active_count= 0;
  while (sig_val <= inactivate_thre_value) {
    digitalWrite(7, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    active_count+= 5;
    float sensorA0Value = analogRead(A0);
    float sensorA1Value = analogRead(A1);
    float sensorA2Value = analogRead(A2);
    float sensorA3Value = analogRead(A3);
    float norSensorA0Value = 1- sensorA0Value / 1000;
    float norSensorA1Value = 1 - sensorA1Value / 1000;
    float norSensorA2Value = 1 - sensorA2Value / 1000;
    float norSensorA3Value = 1 - sensorA3Value / 1000;
    float sum_val= norSensorA0Value+ norSensorA1Value+ norSensorA2Value+ norSensorA3Value;
    if (sum_val== 4 || norSensorA0Value<0 || norSensorA1Value< 0 || norSensorA2Value< 0 || norSensorA3Value< 0){
      continue;
    };


    float activate_thre_value = (norSensorA0Value+ norSensorA1Value+ norSensorA2Value+ norSensorA3Value) /4;
    float inactivate_thre_value = 0.6;
    float err_A0 = activate_thre_value - norSensorA0Value;
    float err_A1 = activate_thre_value - norSensorA1Value;
    float err_A2 = activate_thre_value - norSensorA2Value;
    float err_A3 = activate_thre_value - norSensorA3Value;

    float weight_A0 = norSensorA0Value * activate_thre_value;
    float weight_A1 = norSensorA1Value * activate_thre_value;
    float weight_A2 = norSensorA2Value * activate_thre_value;
    float weight_A3 = norSensorA3Value * activate_thre_value;
    float value = weight_A0 * norSensorA0Value + weight_A1 * norSensorA1Value + weight_A2 * norSensorA2Value+ weight_A3 * norSensorA3Value- err_A0 - err_A1 - err_A2- err_A3;
  
    float sig_val = 1 / (1 + pow(2.71828182846, -value));
    Serial.println("");
    Serial.println("Active");
    Serial.print(norSensorA0Value);  
    Serial.print(norSensorA1Value);
    Serial.print(norSensorA2Value);
    Serial.print(norSensorA3Value);
    active= true;
    digitalWrite(7, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    if (sig_val>inactivate_thre_value || active_count> 600 ){ // Inactivate after 600 sec
      sig_val= 1;
      break;
    }
  }
  Serial.println("");
  Serial.println("Inctive");
  digitalWrite(7, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  if (active) {
    active_count+= 1;
  }
}


void loop() {
  update_sensors_value();
  delay(3600000); // 3600000= 1 hr
  if (hour_count%12== 0 && hour_count!= 0){
    if (active_count== 0){
      digitalWrite(7, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(duration_time);
      Serial.println("");
      Serial.println("Inctive");
      digitalWrite(7, HIGH);
      digitalWrite(LED_BUILTIN, LOW);

    }
    hour_count= 0;
  }
  hour_count+= 1;
}
