#include <Wire.h>
const int MPU = 0x68; //address i2c
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float Temp;

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Wire.begin();   //initialize communication
  Wire.beginTransmission(MPU);    //Start communication with MPU6050
  Wire.write(0x6B); //Talk to the register 6B
  Wire.write(0x00); //Make reset - place a 0 into the 6B register
  Wire.endTransmission(true); //End the transmission
  delay(20);
  Serial.println("Start loop");
}
void loop()
{
  //Read accelerometer data 

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);//Start with register 0x3B ( ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); //Get data from 14 register from 0x3B
  AccX = (Wire.read() <<8 | Wire.read()) / 16384.0; //AFS_SEL 0
  AccY = (Wire.read() <<8 | Wire.read()) / 16384.0;
  AccZ = (Wire.read() <<8 | Wire.read()) / 16384.0;

  Temp = (Wire.read() <<8 | Wire.read()) / 340.0 +36.53;

  GyroX = (Wire.read() <<8 | Wire.read()) / 131.0; //FS_SEL 0
  GyroY = (Wire.read() <<8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() <<8 | Wire.read()) / 131.0;

  Serial.print(AccX); Serial.print(" ");
  Serial.print(AccY); Serial.print(" ");
  Serial.print(AccZ); Serial.println(" ");

  Serial.print(GyroX); Serial.print(" ");
  Serial.print(GyroY); Serial.print(" ");
  Serial.print(GyroZ); Serial.print(" ");

}