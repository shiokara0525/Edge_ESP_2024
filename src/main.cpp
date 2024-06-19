#include<Arduino.h>
#include<BluetoothSerial.h>
#include <PS4Controller.h>
#include<OLED_a.h>
#include<timer.h>

#define DEF_NUM 3838

oled_attack OLED;
BluetoothSerial BTSerial;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

timer timer_main;

int Mode = 0;
int Mode_old = 999;

int sendtoTeensy(const char*,int);

int recieveData();

int neopixel_flag = 0;
int D_A,A_A;
int D_flag,A_flag;

int sendPS4();

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial2.begin(115200);
  BTSerial.begin("ESP32");
  // PS4.begin("A0:A3:B3:2D:17:06");
  Serial.println("Ready.");
  OLED.setup();
  Mode = 99;
  pinMode(35,INPUT);
}

void loop() {
  timer_main.reset();
  neopixel_flag = 0;
  if(7 <= Serial2.available()){
    recieveData();
  }


  if(Mode == 0){
    if(Mode != Mode_old){
      Mode_old = Mode;
      sendtoTeensy("Mode",0);
      OLED.start();
      pixels.clear();
      pixels.show();
    }

    OLED.OLED();

    if(OLED.end_flag == 1){
      if(OLED.Robot_Mode == 0){
        Mode = 1;
      }
      else if(OLED.Robot_Mode == 1){
        Mode = 2;
      }
      else if(OLED.Robot_Mode == 2){
        Mode = 3;
      }
    }
  }
  if(Mode == 1){
    if(Mode != Mode_old){
      Mode_old = Mode;
      sendtoTeensy("Mode",1);
      pixels.clear();
      pixels.show();
    }
    if(digitalRead(OLED.Tact_Switch[1]) == LOW){
      Mode = 0;
    }
  }
  else if(Mode == 2){
    if(Mode != Mode_old){
      Mode_old = Mode;
      sendtoTeensy("Mode",2);
      pixels.clear();
      pixels.show();
    }

    if(neopixel_flag){
      pixels.clear();
      if(D_A == 10){
        if(D_flag == 1){
          pixels.setPixelColor(4,pixels.Color(100,0,0));
        }
        else{
          for(int i = 0; i < 16; i++){
            pixels.setPixelColor(i,pixels.Color(100,0,0));
          }
        }
      }
      else if(D_A == 12){
        pixels.setPixelColor(4,pixels.Color(100,100,0));
      }
      else if(D_A == 15){
        pixels.setPixelColor(4,pixels.Color(0,100,0));
      }
      else if(D_A == 16){
        pixels.setPixelColor(4,pixels.Color(0,100,100));
      }
      else if(D_A == 20){
        pixels.setPixelColor(4,pixels.Color(0,0,100));
      }
      pixels.show();
    }
    if(digitalRead(OLED.Tact_Switch[1]) == LOW){
      Mode = 0;
    }
  }
  else if(Mode == 99){
    if(Mode != Mode_old){
      Mode_old = Mode;
      sendtoTeensy("Mode",99);
    }
    OLED.first_select();
    if(digitalRead(OLED.Tact_Switch[1]) == LOW){
      Mode = 0;
    }
  }

  if (PS4.isConnected()) {
    sendPS4();
  }
}


int sendtoTeensy(const char *message,int val){
  int send;
  int flag = 0;

  if(message == "Mode"){
    send = Mode;
    flag = 1;
  }
  else if(message == "state"){
    send = OLED.state;
    flag = 2;
  }
  else if(message == "val_max"){
    send = OLED.val_max;
    flag = 3;
  }
  else if(message == "color"){
    send = OLED.color;
    flag = 4;
  }
  else if(message == "ball_th"){
    send = OLED.ball_getth;
    flag = 5;
  }
  else if(message == "Mnone_flag"){
    send = OLED.Mnone_flag;
    flag = 6;
  }
  else if(message == "ac_reset"){
    send = OLED.ac_reset;
    flag = 7;
  }
  else if(message == "kick"){
    send = OLED.kick_flag;
    flag = 8;
  }
  else if(message == "PS4"){
    flag = 9;
  }
  else if(message == "LINE_level"){
    send = OLED.LINE_level;
    flag = 10;
  }
  else if(message == "PS4_R"){
    flag = 11;
  }
  else if(message == "CHECK"){
    flag = 12;
  }

  // Serial.print(" message : ");
  // Serial.print(message);
  // Serial.print(" flag : ");
  // Serial.print(flag);
  // Serial.print(" ");
  // Serial.println();

  if(val != DEF_NUM){
    send = val;
  }

  if(flag != 12){
    uint8_t send_byte[5] = {38,0,0,0,37};
    send_byte[1] = flag;
    send_byte[2] = byte( send >> 8 ); //ビットシフトで上位側の８Bitを取り出し、バイト型に型変換をする。
    send_byte[3] = byte( send & 0xFF ); //論理和で下位側の８Bitを取り出し、バイト型に型変換をする。
    Serial2.write(send_byte,5);
  }
  else{
    for(int i = 0; i < 6; i++){
      send = OLED.check_val[i];
      uint8_t send_byte[5] = {38,0,0,0,37};
      send_byte[1] = flag + i;
      send_byte[2] = byte( send >> 8 ); //ビットシフトで上位側の８Bitを取り出し、バイト型に型変換をする。
      send_byte[3] = byte( send & 0xFF ); //論理和で下位側の８Bitを取り出し、バイト型に型変換をする。
      Serial2.write(send_byte,5);
    }
  }

  return 1;
}


int recieveData(){
  uint8_t recieve_byte[7];
  word contain[4];
  while(7 <= Serial2.available()){
    recieve_byte[0] = Serial2.read();
    Serial.print(" recieve_byte : ");
    if(recieve_byte[0] != 38){
      continue;
    }
    for(int i = 1; i < 7; i++){
      recieve_byte[i] = Serial2.read();
      Serial.print(" ");
      Serial.print(recieve_byte[i]);
    }

    contain[0] = recieve_byte[2] << 8;
    contain[1] = recieve_byte[3];
    contain[2] = recieve_byte[4] << 8;
    contain[3] = recieve_byte[5];
    int16_t recieve_int[2];
    recieve_int[0] = contain[0] | contain[1];
    recieve_int[1] = contain[2] | contain[3];

    // for(int i = 0; i < 7; i++){
    //   Serial.print(" ");
    //   Serial.print(recieve_byte[i]);
    // }
    Serial.print(" 0 : ");
    Serial.print(recieve_int[0]);
    Serial.print(" 1 : ");
    Serial.print(recieve_int[1]);

    if(recieve_byte[1] == 1){
      sendtoTeensy("Mode",DEF_NUM);
      sendtoTeensy("val_max",DEF_NUM);
      sendtoTeensy("color",DEF_NUM);
      sendtoTeensy("ball_th",DEF_NUM);
      sendtoTeensy("LINE_level",DEF_NUM);
      sendtoTeensy("CHECK",DEF_NUM);
    }
    else if(recieve_byte[1] == 2){
      OLED.ball_vec.setX(recieve_int[0]);
      OLED.ball_vec.setY(recieve_int[1]);
    }
    else if(recieve_byte[1] == 3){
      OLED.line_vec.setX(recieve_int[0] * 0.01);
      OLED.line_vec.setY(recieve_int[1] * 0.01);
      OLED.line_vec.print();
    }
    else if(recieve_byte[1] == 4){
      OLED.cam_vec.setX(recieve_int[0]);
      OLED.cam_vec.setY(recieve_int[1]);
    }
    else if(recieve_byte[1] == 5){
      OLED.ac_dir = recieve_int[0];
    }
    else if(recieve_byte[1] == 6){
      BTSerial.print(" cam_ang : ");
      BTSerial.print(recieve_int[0]);
      BTSerial.print(" go_ang : ");
      BTSerial.println(recieve_int[1]);
    }
    else if(recieve_byte[1] == 7){
      neopixel_flag = 1;
      D_A = recieve_int[0];
      D_flag = recieve_int[1];
    }
    Serial.println();
    return 1;
    break;
  }
  return 0;
}


int sendPS4(){
  int x,y;

  y = PS4.LStickX();
  x = PS4.LStickY();
  unsigned int contain[2];
  int send_int;
  contain[0] = uint16_t(y + 128) << 8;
  contain[1] = uint16_t(x + 128);
  send_int = (contain[0] | contain[1]);
  sendtoTeensy("PS4",send_int);
  y = PS4.RStickX();
  x = PS4.RStickY();
  contain[0] = uint16_t(y + 128) << 8;
  contain[1] = uint16_t(x + 128);
  send_int = contain[0] | contain[1];
  sendtoTeensy("PS4_R",send_int);
  if(PS4.Circle()){
    sendtoTeensy("kick",1);
  }
  return 1;
}