#include <SPI.h>             // SPI communications library
#include <Esplora.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

// These definitions map display functions to the Adduino Esplora display header pins
#define sclk 15
#define mosi 16
#define cs   7
#define dc   0
#define rst  1

#define WHITE ST7735_WHITE
#define BLACK ST7735_BLACK
#define GREEN ST7735_GREEN
#define YELLOW ST7735_YELLOW
#define BLUE ST7735_BLUE
#define CYAN ST7735_CYAN
#define RED ST7735_RED
#define BROWN ST7735_MAGENTA
#define MAGENTA ST7735_MAGENTA

long bps = 38400;
String BT_Data;
String msg;
String msg_color;
float msg_yaw;
float msg_roll;
float msg_pitch;

SoftwareSerial BT(11,3);
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

//Screens parameters
int width;
int height;

//Interface and control buttons

Adafruit_GFX_Button roll_btn;
Adafruit_GFX_Button pitch_btn;
Adafruit_GFX_Button yaw_btn;

int up_btn;
int down_btn;
int left_btn;
int right_btn;
int joystick_btn;
int IMUMode_btn;
int CALMode_btn;

void TFTColor(String color) {
  int margin_rect = 30;
  if (color == "WHITE") {
    //Esplora.writeRGB(0, 0, 0) ;
    tft.fillRect(0,margin_rect, width/2, height - margin_rect,WHITE);
    //tft.fillScreen(WHITE);
  } else if (color == "BLACK") {
    //Esplora.writeRGB(255, 255, 255) ;
    //tft.fillScreen(BLACK);
    tft.fillRect(0,margin_rect, width/2, height - margin_rect,BLACK);
  } else if (color == "RED") {
    //Esplora.writeRGB(255, 0, 0) ;
    //tft.fillScreen(RED);
    tft.fillRect(0,margin_rect, width/2, height - margin_rect,RED);
  } else if (color == "GREEN") {
    //Esplora.writeRGB(0, 255, 0) ;
    //tft.fillScreen(GREEN);
    tft.fillRect(0,margin_rect, width/2, height - margin_rect,GREEN);
  } else if (color == "BLUE") {
    //Esplora.writeRGB(0, 0, 255) ;
    //tft.fillScreen(BLUE);
    tft.fillRect(0,margin_rect, width/2, height - margin_rect,BLUE);
  } else if (color == "YELLOW") {
    //Esplora.writeRGB(255, 255, 0) ;
    //tft.fillScreen(YELLOW);
    tft.fillRect(0,margin_rect, width/2, height - margin_rect,YELLOW);
  } else if (color == "ORANGE") {
    //Esplora.writeRGB(0, 0, 0) ;
  } else if (color == "BROWN") {
    //Esplora.writeRGB(0, 0, 0) ;
    //tft.fillScreen(BROWN);
    tft.fillRect(0,margin_rect, width/2, height - margin_rect,BROWN);
  } else if (color == "MAGENTA") {
    //Esplora.writeRGB(255, 0, 255) ;
    //tft.fillScreen(MAGENTA);
    tft.fillRect(0,margin_rect, width/2, height - margin_rect,MAGENTA);
  }
}

// Draw Orientation magnitudes on TFT Screen
void TFTIMU(float r, float p, float y, uint8_t R = 250, uint8_t G = 250, uint8_t B = 0) {
  char buf[5];
  int margin_x=10;
  int margin_y=5;
  int textSize = 1;
  int msg_w = sizeof(buf)*textSize*5;
  int msg_h = textSize*7;
  
  //Tamanio real del boton
  int btn_w = msg_w+2*margin_x;
  int btn_h = msg_h+2*margin_y;
  String s = dtostrf(r, 1, 3, buf);
  
  roll_btn.initButton(&tft,width/6,20,btn_w,btn_h,BLACK,BLACK,tft.Color565(B,G,R), s.c_str(), textSize);
  roll_btn.drawButton(true);
  
  s = dtostrf(p, 1, 3, buf);
  pitch_btn.initButton(&tft,width/2,20,btn_w,btn_h,BLACK,BLACK,tft.Color565(B,G,R), s.c_str(), textSize);
  pitch_btn.drawButton(true);
  
  s = dtostrf(y, 1, 3, buf);
  yaw_btn.initButton(&tft,5*width/6,20,btn_w,btn_h,BLACK,BLACK,tft.Color565(B,G,R), s.c_str(), textSize);
  yaw_btn.drawButton(true);
}

void setup() {
  BT.begin(bps);
  Serial.begin(bps);

  //Initialize Data Bluetooth
  BT_Data = (String)("");
  msg = (String)("");
  msg_color = (String)("");
  msg_yaw = (float)(0);

  //Initialize TFT Screen
  tft.initR(INITR_GREENTAB); 
  tft.setRotation(1);  
  tft.setTextWrap(false);
  tft.fillScreen(GREEN);
  width = tft.width();
  height = tft.height();
}

void loop() {
  
  if (BT.available()) {
    BT_Data = (BT.readStringUntil('\n'));
    BT_Data.replace("\r","");
    Serial.println(BT_Data);
    if (BT_Data.startsWith("<")) {
      //Serial.println("Begin with <");
    }
    BT_Data.replace("<","");
    if (BT_Data.endsWith(">")) {
      //Serial.println("Ends with >");
    }
    msg = (BT_Data.substring( 0, (BT_Data.indexOf("|")) ));
    //Serial.println(msg);
    BT_Data.remove(0,((BT_Data.indexOf("|")) + 1));
    
    //Manage Data from sensors
    if (msg == "IMU") {
      msg_roll = (BT_Data.toFloat());
      BT_Data.remove(0,((BT_Data.indexOf("|")) + 1));
      msg_pitch = (BT_Data.toFloat());
      BT_Data.remove(0,((BT_Data.indexOf("|")) + 1));
      msg_yaw = (BT_Data.toFloat());
      TFTIMU(msg_roll , msg_pitch, msg_yaw);
      Serial.println(msg_yaw);
    }
    if (msg == "COLOR") {
      Serial.println("COLOR Detection");
      msg_color = (BT_Data.substring( 0, (BT_Data.indexOf(">")) ));
      Serial.println(msg_color);
      TFTColor( msg_color );
      Serial.println();
    }
    
  }
  

  if (Serial.available()){
   char c = Serial.read();
   BT.print(c);
 }
  //BT.flush();
  if( Esplora.readButton(SWITCH_UP) != up_btn){
    up_btn = Esplora.readButton(SWITCH_UP);
      if ((Esplora.readButton(SWITCH_UP)) != true) {
        BT.println('U');
        Serial.println("UP");
      } else {
        BT.println('S');
        Serial.println("STOP");
      }
  }

  if(Esplora.readButton(SWITCH_DOWN) != down_btn){
    down_btn = Esplora.readButton(SWITCH_DOWN);
    if ((Esplora.readButton(SWITCH_DOWN)) != true) {
      BT.println('D');
      Serial.println("DOWN");
    } else {
      BT.println('S');
      Serial.println("STOP");
    }

  }
  
  if(Esplora.readButton(SWITCH_LEFT) != left_btn){
    left_btn = Esplora.readButton(SWITCH_LEFT);
    if ((Esplora.readButton(SWITCH_LEFT)) != true) {
      BT.println('L');
      Serial.println("LEFT");
    } else {
      BT.println('S');
      Serial.println("STOP");
    }

  }
  
  if(Esplora.readButton(SWITCH_RIGHT) != right_btn){
    right_btn = Esplora.readButton(SWITCH_RIGHT);
      if ((Esplora.readButton(SWITCH_RIGHT)) != true) {
      BT.println('R');
      Serial.println("RIGHT");
    } else {
      BT.println('S');
      Serial.println("STOP");
    }

  }
  
  if(Esplora.readJoystickSwitch()  != joystick_btn){
    joystick_btn = Esplora.readJoystickSwitch() ;
    if ((Esplora.readJoystickSwitch() ) != true) {
      BT.println("Y");
      Serial.println("Yes");
    } /*else {
      BT.println('N');
      Serial.println("No");
    }*/

  }

  //Calibration and mode configuration
  if( Esplora.readSlider() < 10 ){  
    if ( (Esplora.readButton(SWITCH_UP) && Esplora.readButton(SWITCH_DOWN) ) != IMUMode_btn ) {
      IMUMode_btn = Esplora.readButton(SWITCH_UP) && Esplora.readButton(SWITCH_DOWN) ;
      if ( IMUMode_btn != true ){
        Esplora.writeRGB(0, 0, 255) ;
        BT.println("IMU");
        Serial.println("IMU Mode");
      }
        
    }
   
  }
  
  if( Esplora.readSlider() < 10 ){
    if ( (Esplora.readButton(SWITCH_LEFT) && Esplora.readButton(SWITCH_RIGHT) ) != CALMode_btn ) {
      CALMode_btn = Esplora.readButton(SWITCH_LEFT) && Esplora.readButton(SWITCH_RIGHT) ;
      
      if ( CALMode_btn != true ){
        Esplora.writeRGB(0, 255, 0) ;
        BT.println("CAL");
        Serial.println("Color Calibration");
      }
        
    }
  }
}


