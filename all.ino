#include <Wire.h>

float previousTime, currentTime, elapsedTime;
float distance_x = 0;
float distance_y = 0;
float distance_z = 0;
float g = 7;
float threshold_IMU = 0.6;
float total_distance = 0;
float gx = 0, gy = 0, gz = 0;

int count = 0;
boolean flag = 0;
int cycles = 0;
int threshold_tap = 1500;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

void loop() {
  
  // Force sensor
  int data = analogRead(25);
    
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
//    Serial.print("data:");
//    Serial.println(data);

  
  // IMU
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000;
  
  // acc x
  
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 1);
  byte acc_x_h = Wire.read();

  Wire.beginTransmission(0x68);
  Wire.write(0x3C);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 1);
  byte acc_x_l = Wire.read();

  int16_t acc_x_combined = acc_x_h << 8 | acc_x_l;

  float gx = acc_x_combined / 16384.0 * 4;

  if (gx >= g){
    distance_x += gx * elapsedTime; 
  }
  
  // acc y
  Wire.beginTransmission(0x68);
  Wire.write(0x3D);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 1);
  byte acc_y_h = Wire.read();

  Wire.beginTransmission(0x68);
  Wire.write(0x3E);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 1);

  byte acc_y_l = Wire.read();

  int16_t acc_y_combined = acc_y_h << 8 | acc_y_l;

  float gy = acc_y_combined / 16384.0 * 4;

  if (gy >= g){
    distance_y += gy * elapsedTime; 
  }

  // acc z
  Wire.beginTransmission(0x68);
  Wire.write(0x3F);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 1);
  byte acc_z_h = Wire.read();

  Wire.beginTransmission(0x68);
  Wire.write(0x40);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 1);

  byte acc_z_l = Wire.read();

  int16_t acc_z_combined = acc_z_h << 8 | acc_z_l;

  float gz = acc_z_combined / 16384.0 * 4;

  if (gz >= g){
    distance_z += gz * elapsedTime; 
  }
  
  float total_distance = sqrt(distance_x * distance_x + distance_y * distance_y + distance_z * distance_z);

  if (total_distance >= threshold_IMU){
    digitalWrite(4, 100);
    delay(10000);
    digitalWrite(4, 0);
    total_distance = 0;
    distance_x = 0;
    distance_y = 0;
    distance_z = 0;
  }

  if (abs(gx) < 2 && abs(gy) < 2 && abs(gz) < 3.5) {
    
    total_distance = 0;
    distance_x = 0;
    distance_y = 0;
    distance_z = 0;
    
  }

//  Serial.print("ACCX: ");
//  Serial.print(gx);
//
//  Serial.print(" ");
//  Serial.print("ACCY: ");
//  Serial.print(gy);
//
//  Serial.print(" ");
//  Serial.print("ACCZ: ");
//  Serial.print(gz);
//  
//  Serial.print("DX: ");
//  Serial.print(distance_x);
//  
//  Serial.print(" ");
//  Serial.print("DY: ");
//  Serial.print(distance_y);
//
//  Serial.print(" ");
//  Serial.print("DZ: ");
//  Serial.print(distance_z);
//
//  Serial.print(" tt");
//  Serial.println(total_distance);
}

void reset() {
  total_distance = 0;
  distance_x = 0;
  distance_y = 0;
  distance_z = 0;
}

float update_distance(int addr, float *axis_g, float *axis_distance, float elapsedTime) {
  Wire.beginTransmission(0x68);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 2);
  byte acc_h = Wire.read();
  byte acc_l = Wire.read();

  int16_t acc_combined = acc_h << 8 | acc_l;

  *axis_g = acc_combined / 16384.0 * 4;
  
  if (*axis_g >= g) {
    *axis_distance += *axis_g * elapsedTime * elapsedTime; 
  }
  //Serial.println(axis_g);
  return *axis_distance;
}
