#include <MadgwickAHRS.h>
#include <IMU_MPU9250.h>

#include <SoftwareSerial.h>
#include <Servo.h>

#define  BPS 38400

//#define DEBUG
#define DEBUG_CAL
#define DEBUG_RX 12
#define DEBUG_TX 13
#define BT_DEBUG
#include <TCS3200.h>

#define IMUDEBUG true


// Bluetooth Data
char BT_Data;
String BT_String;
String status;

long timestamp;

//SoftwareSerial BT(12,13);

//IMU Data
float newyaw;
float roll;
float pitch;
float yaw;
int reference;
int angle_threshold;
int angle;
int bound1;
int bound2;
char serialData;
long microsPerReading;
long last_time = 0; 
String last_move;
String actual_move;

Madgwick filter;
MPU9250 imu;

void updateIMU(float *roll, float *pitch, float *yaw){
 
  if (imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01){    
    imu.readAccelData(imu.accelCount);
    imu.getAres();
 
    imu.ax = (float)imu.accelCount[0]*imu.aRes;
    imu.ay = (float)imu.accelCount[1]*imu.aRes;
    imu.az = (float)imu.accelCount[2]*imu.aRes;
    imu.readGyroData(imu.gyroCount);
    imu.getGres();
 
    imu.gx = (float)imu.gyroCount[0]*imu.gRes;
    imu.gy = (float)imu.gyroCount[1]*imu.gRes;
    imu.gz = (float)imu.gyroCount[2]*imu.gRes;
  }
 
  imu.updateTime();
  imu.delt_t = millis() - imu.count;
  if (imu.delt_t > microsPerReading){
    filter.updateIMU(imu.gx, imu.gy, imu.gz, imu.ax, imu.ay, imu.az);
    *roll = filter.getRoll();
    *pitch = filter.getPitch();
    *yaw = filter.getYaw();
    imu.count = millis();
    
  }
}

//Color Sensor
TCS3200 CS_F(8,9,10);

//Movement Methods
Servo LeftServo;
Servo RightServo;
Servo Claw;

void Servo_attach() {
  LeftServo.attach(5);
  RightServo.attach(6);
}
void Servo_detach() {
  LeftServo.detach();
  RightServo.detach();
}

void forward() {
  Servo_attach();
  LeftServo.write(0);
  RightServo.write(180);
}

void backwards() {
  Servo_attach();
  LeftServo.write(180);
  RightServo.write(0);
}

void turn_left() {
  Servo_attach();
  LeftServo.write(180);
  RightServo.write(180);
}

void turn_right() {
  Servo_attach();
  LeftServo.write(0);
  RightServo.write(0);
}

void stop() {
  LeftServo.write(90);
  RightServo.write(90);
  Servo_detach();
}


void setup() {

  //Bluetooth Initialization
  BT.begin(38400);
  Serial.begin(38400);
  BT.setTimeout(200);
  
  BT_String = (String)("");
  status = (String)("MANUAL");
  
  
  LeftServo.attach(5);
  RightServo.attach(6);
  Claw.attach(10);

  //IMU Initialization
  filter.begin(25);
  Wire.begin();
  microsPerReading = 1000 / 25;
  imu.calibrateMPU9250(imu.gyroBias, imu.accelBias);
  imu.initMPU9250();
  newyaw = (float)(0);
  roll = (int)(0);
  pitch = (int)(0);
  yaw = (int)(0);
  reference = (int)(0);
  angle_threshold = (int)(25);
  angle = (int)(180);
  //last_move = (String)("FORWARD");
  //actual_move = (String)("FORWARD");

  //Color Sensor Initialization
  CS_F.begin();
  CS_F.nSamples(40);
  CS_F.setRefreshTime(200);
  CS_F.setFrequency(TCS3200_FREQ_HI);

  //Load Color EEPROM Calibration
  Serial.println("Clumsee Calibration");
  //CS_F.calibration(0);
  CS_F.loadCal(0);
}

void loop() {

    //-----Receive Data from Bluetooth ---- //
    if (BT.available()) {
      timestamp = millis();
      BT_String = BT.readStringUntil('\n');
      BT_String.replace("\r","");
      //BT_Data = BT_String[0];
      Serial.println(BT_String);
      if (BT_String == ("I")) {
        status = "IMU";
      } else if (BT_String == ("U")) {
        forward();
      } else if (BT_String == ("D")) {
        backwards();
      } else if (BT_String == ("L")) {
        turn_left();
      } else if (BT_String == ("R")) {
        turn_right();
      } else if (BT_String == ("S")) {
        stop();
      } else if (BT_String == ("C")) {
        //ClawValue = (int)(((BT.readString()).toInt()));
        //Claw.write((map(ClawValue,10, 55, ClawMin , ClawMax)));
      }else if(BT_String == ("CAL")){
        CS_F.calibration(0);
      }
      
      Serial.print("Time: ");
      Serial.println( millis() - timestamp );
    }
    if (status == "IMU") {
    }
  
  BT.flush();

  //------------------------------------------//
  //-------Send Data Bluetooth----------------//
  //------------------------------------------//

  updateIMU(&roll, &pitch, &yaw);

  //Change reference on yaw circumference
  if (yaw < 360 && yaw > (360 - reference)) {
    newyaw = yaw - (360 + reference);
  } else {
    newyaw = yaw + reference;
  }
  
   if (Serial.available()){
     String c = Serial.readString();
     BT.print(c);
   }

   // BT Send Data from IMU Sensor and DEBUG
   if (IMUDEBUG){ 
      if ( millis() - last_time > 500){
         
        
        BT.println( IMUDebug() );
        last_time = millis();
      }
   } 

   if (CS_F.onChangeColor()) {
    Serial.println(CS_F._ct[ CS_F.checkColor( &CS_F._rgb) ].name);
    BT.println( "<COLOR|"+ String(CS_F._ct[ CS_F.checkColor( &CS_F._rgb) ].name) + ">" );
    
  }
}

String IMUDebug(){ 
     Serial.print("Orientation ");
     Serial.print(yaw);
     Serial.print(" ");
     Serial.print(pitch);
     Serial.print(" ");
     Serial.print(roll);
     Serial.print(" \tnew Yaw: ");
     Serial.println(newyaw);
     String IMU_str = "<IMU|"+String(roll)+"|"+String(pitch)+"|"+String(yaw)+">"; 
     return IMU_str;
 };
