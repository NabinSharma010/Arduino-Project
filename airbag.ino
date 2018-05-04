#include <Wire.h>
#include <math.h>
#include <Servo.h>
Servo myservo;
long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
float previous;
float now;

void setup() {
  Serial.begin(9600);
  pinMode(7,OUTPUT);
  myservo.attach(8);
  Wire.begin();
  setupMPU();
  myservo.write(50);
}


void loop() {
  setupMPU();
  recordAccelRegisters();
  recordGyroRegisters();
  fall(previous);
  delay(10);
  previous = gForceX;
  printData();
}

void softReset(){
asm volatile ("  jmp 0");
}

void fall(float state){
  /*
  Serial.print("previous = ");
  Serial.println(state);

  Serial.print("NOW = ");
  Serial.println(gForceX);
  */
  if(sqrt(pow(rotZ,2)+pow(rotY,2))>54 and gForceX<5.3 and state>5.3){
    myservo.write(150);
    digitalWrite(7,HIGH);
    delay(1000);
    myservo.write(50);
    delay(3000);
    digitalWrite(7,LOW);
    }
}
void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  Serial.print("a");
  if(Wire.available() < 6);
    Serial.print("b");
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
    Serial.print("c");
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
    Serial.print("d");
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
    Serial.print("e");
  processAccelData();
}

void processAccelData(){
  gForceX = accelX*9.81 / 16384.0;
  gForceY = accelY*9.81 / 16384.0; 
  gForceZ = accelZ*9.81 / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
    Serial.print("1");
  if(Wire.available() < 6);
    Serial.print("2");
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
    Serial.print("3");
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
    Serial.print("4");
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
    Serial.print("5");  
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void printData() {
   /*Serial.print("Formation= ");
   Serial.println(sqrt(pow(rotZ,2)+pow(rotY,2)));
   Serial.print("Previous= ");
   Serial.println(previous);
  */
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
 
  
}
