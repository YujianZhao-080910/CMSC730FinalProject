#include <Wire.h>

float previousTime, currentTime, elapsedTime;
float distance_x = 0;
float distance_y = 0;
float distance_z = 0;
float g = 6.5;
float threshold_IMU = 0.5;
float total_distance = 0;
float gx = 0, gy = 0, gz = 0;

int count = 0;
boolean flag = 0;
int cycles = 0;
int threshold_tap = 1500;

int sensorReading = 0; 
const int knockSensor = 12; // the piezo is connected to analog pin 0
const int threshold_knock = 800;
bool flag_knock = 0;
int count_knock = 0;
int cycles_knock = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() {
  
  // Force sensor
  int data = analogRead(13);
    
    if (data >= threshold_tap){
      flag = 1;
    } 
    // Reset
    if (flag && data < threshold_tap) {
      flag = 0;
      count += 1;
      if (count == 3) {
        digitalWrite(4, 500);
        delay(10000);
        digitalWrite(4, 0);
      }
    }
    //digitalWrite(4, 100);
    delay(100);
    cycles += 1;
    if (cycles > 100) {
      // reset
      flag = 0;
      count = 0;
      cycles = 0;
    }
//    Serial.print("cycle:");
//    Serial.println(cycles);
//    Serial.print("count:");
//    Serial.println(count);

  // Knock
   sensorReading = analogRead(knockSensor);

  if (sensorReading > threshold_knock){
      flag_knock = 1;
  }
  if (flag_knock && sensorReading < threshold_knock){
     flag_knock = 0;
     count_knock += 1;
     if (count_knock == 5){
       digitalWrite(4, 100);
       delay(10000);
       digitalWrite(4, 0);
     }
  }

  Serial.println(sensorReading);
  Serial.println(count_knock);
  delay(100);
  
  // IMU

  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000;

  // acc x
  distance_x = update_distance(0x3B, &gx, distance_x, elapsedTime);
  
  // acc y
  distance_y = update_distance(0x3D, &gy, distance_y, elapsedTime);

  // acc z
  distance_z = update_distance(0x3F, &gz, distance_z, elapsedTime);
  
  total_distance = sqrt(distance_x * distance_x + distance_y * distance_y + distance_z * distance_z);
  
//  Serial.print("distanceX: ");
//  Serial.print(distance_x);
//  
//  Serial.print(" ");
//  Serial.print("distanceY: ");
//  Serial.print(distance_y);
//
//  Serial.print(" ");
//  Serial.print("distanceZ: ");
//  Serial.print(distance_z);
//
//  Serial.print(" tt");
//  Serial.println(total_distance);

  if (total_distance >= threshold_IMU){
    digitalWrite(4, 100);
    delay(10000);
    digitalWrite(4, 0);
    reset();
  }
  
  if (abs(gx) < 2 && abs(gy) < 2 && abs(gz) < 2) {
    total_distance = 0;
  }
}

void reset() {
  total_distance = 0;
  distance_x = 0;
  distance_y = 0;
  distance_z = 0;
}

float update_distance(int addr, float *axis_g, float axis_distance, float elapsedTime) {
  Wire.beginTransmission(0x68);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 2);
  byte acc_h = Wire.read();
  byte acc_l = Wire.read();

  int16_t acc_combined = acc_h << 8 | acc_l;

  *axis_g = acc_combined / 16384.0 * 4;
  
  if (*axis_g >= g) {
    axis_distance += *axis_g * elapsedTime; 
  }
  //Serial.println(axis_g);
  return axis_distance;
}
