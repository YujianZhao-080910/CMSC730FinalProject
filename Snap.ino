int sensorReading = 0; 
const int knockSensor = 12; // the piezo is connected to analog pin 0
const int threshold_knock = 1200;
bool flag_knock = 0;
int count_knock = 0;
int cycles_knock = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Knock
   sensorReading = analogRead(knockSensor);

  if (sensorReading > threshold_knock){
     count_knock += 1;
     if (count_knock == 3){
       digitalWrite(4, 100);
       delay(10000);
       digitalWrite(4, 0);
       count_knock = 0;
     }
     delay(2500);
     
  }
  Serial.println(sensorReading);
//     Serial.println(count_knock);
  delay(100);
  
}
