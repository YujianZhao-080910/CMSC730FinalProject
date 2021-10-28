#include <Wire.h>
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}

float previousTime, currentTime, elapsedTime;
float distance_x = 0;
float distance_y = 0;
float distance_z = 0;
float g = 6.5;
float threshold = 0.75;

void loop() {
  // put your main code here, to run repeatedly:

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

  if (total_distance >= threshold){
    digitalWrite(4, 100);
  }

  if (abs(gx) < 2 && abs(gy) < 2 && abs(gz) < 2) {
    total_distance = 0;
  }
  
  Serial.print("ACCX: ");
  Serial.print(distance_x);
  
  Serial.print(" ");
  Serial.print("ACCY: ");
  Serial.print(distance_y);

  Serial.print(" ");
  Serial.print("ACCZ: ");
  Serial.print(distance_z);

  Serial.print(" tt");
  Serial.println(total_distance);

//
//  Serial.print("GYROX: ");
//  Serial.print(gyro_x);
//
//  Serial.print(" ");
//  Serial.print("GYROY: ");
//  Serial.print(gyro_y);
//  
//  Serial.print(" ");
//  Serial.print("GYROZ: ");
//  Serial.println(gyro_z);
//  delay(20);
}
