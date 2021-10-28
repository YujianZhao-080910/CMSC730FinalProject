void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
}
int count = 0;
boolean flag = 0;
int cycles = 0;
int threshold = 1500;
void loop() {
  // put your main code here, to run repeatedly:
    int data = analogRead(13);
    
    if (data >= threshold){
      flag = 1;
    } 
    if (flag && data < threshold) {
      flag = 0;
      count += 1;
      if (count == 3) {
        digitalWrite(4, 100);
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
    Serial.print("cycle:");
    Serial.println(cycles);
    Serial.print("count:");
    Serial.println(count);
    
}
