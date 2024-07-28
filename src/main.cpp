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
int D_v;
int D_A_15_flag;

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

      if(OLED.test_flag){
        Mode = 3;
      }
    }
  }
  if(Mode == 1){
    if(Mode != Mode_old){
      Mode_old = Mode;
      sendtoTeensy("SETPLAY",DEF_NUM);
      sendtoTeensy("Mode",1);
      pixels.clear();
      pixels.show();
    }
    sendtoTeensy("A_NEO",DEF_NUM);
    OLED.OLED_moving();

    pixels.clear();
    if(OLED.option_on[3]){
      if(A_A == 10){
        pixels.setPixelColor(4,pixels.Color(0,0,200));
      }
      else if(A_A == 12){
        pixels.setPixelColor(4,pixels.Color(0,200,0));
      }
      else if(A_A == 20){
        pixels.setPixelColor(4,pixels.Color(200,0,0));
      }
      else if(A_A == 21){
        pixels.setPixelColor(4,pixels.Color(200,200,0));
      }
      else if(A_A == 22){
        pixels.setPixelColor(4,pixels.Color(0,200,200));
      }
      else if(A_A == 23){
        pixels.setPixelColor(4,pixels.Color(200,0,200));
      }
      if(OLED.cam_on){
        int ball_pos = 0;
        ball_pos = (degrees(OLED.cam_vec.getAngle()) + 180) / 22.5 - 4;
        if(ball_pos < 0){
          ball_pos += 16;
        }
        Serial.print(" b_p : ");
        Serial.print(ball_pos);
        if(OLED.goal_color == BLUE){
          pixels.setPixelColor(ball_pos,pixels.Color(0,0,100));
        }
        else if(OLED.goal_color == YELLOW){
          pixels.setPixelColor(ball_pos,pixels.Color(100,100,0));
        }
      }
      if(OLED.cam_back_on){
        int ball_pos = 0;
        ball_pos = -(degrees(OLED.cam_back_vec.getAngle())) / 22.5 + 12;
        if(ball_pos < 0){
          ball_pos += 16;
        }
        Serial.print(" b_p : ");
        Serial.print(ball_pos);
        if(OLED.goal_color == BLUE){
          pixels.setPixelColor(ball_pos,pixels.Color(100,100,0));
        }
        else if(OLED.goal_color == YELLOW){
          pixels.setPixelColor(ball_pos,pixels.Color(0,0,100));
        }
      }
    }

    pixels.show();

    if(digitalRead(OLED.Tact_Switch[1]) == LOW){
      Mode = 0;
    }

    if(digitalRead(OLED.Bluetooth_pin) == LOW && OLED.option_on[4]){
      Mode = 10;
    }
  }
  else if(Mode == 2){
    if(Mode != Mode_old){
      Mode_old = Mode;
      sendtoTeensy("Mode",2);
      pixels.clear();
      pixels.show();
    }
    OLED.OLED_moving();
    sendtoTeensy("D_neo",999);
    if(OLED.option_on[3]){
      if(neopixel_flag){
        pixels.clear();
        if(D_A == 0){
          pixels.setPixelColor(5,pixels.Color(100,100,0));
        }
        else if(D_A == 5){
          pixels.setPixelColor(5,pixels.Color(100,0,0));
        }
        else if(D_A == 10){
          for(int i = 0; i < 15; i++){
            if(D_v < 5 * i){
              if(D_flag == 0){
                for(int j = 0; j <= i; j++){
                  pixels.setPixelColor(j,pixels.Color(100,0,0));
                }
              }
              else if(D_flag == 1){
                for(int j = 0; j <= i; j++){
                  pixels.setPixelColor(j,pixels.Color(100,100,0));
                }
              }
              else if(D_flag == 2){
                for(int j = 0; j <= i; j++){
                  pixels.setPixelColor(j,pixels.Color(100,0,100));
                }
              }
              break;
            }
          }
        }
        else if(D_A == 12){
          pixels.setPixelColor(4,pixels.Color(100,100,0));
        }
        else if(D_A == 13){
          pixels.setPixelColor(4,pixels.Color(100,0,100));
        }
        else if(D_A == 15){
          pixels.setPixelColor(3 + D_A_15_flag,pixels.Color(0,100,0));
        }
        else if(D_A == 16){
          pixels.setPixelColor(4,pixels.Color(0,100,100));
        }
        else if(D_A == 20){
          pixels.setPixelColor(4,pixels.Color(0,0,100));
        }
        pixels.show();
      }
    }
    if(digitalRead(OLED.Tact_Switch[1]) == LOW){
      Mode = 0;
    }

    if(digitalRead(OLED.Bluetooth_pin) == LOW && OLED.option_on[4]){
      Mode = 10;
    }
  }
  else if(Mode == 3){
    if(Mode != Mode_old){
      Mode_old = Mode;
      sendtoTeensy("Mode",3);
      sendtoTeensy("TEST",OLED.testMode);
    }
    if(digitalRead(OLED.Tact_Switch[1]) == LOW){
      Mode = 0;
    }
  }
  else if(Mode == 10){
    if(Mode != Mode_old){
      Mode_old = Mode;
      sendtoTeensy("Mode",10);
    }
    if(digitalRead(OLED.Bluetooth_pin) == HIGH){
      if(OLED.Robot_Mode == 0){
        Mode = 1;
      }
      else if(OLED.Robot_Mode == 1){
        Mode = 2;
      }
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
    if(OLED.first_select()){
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
  byte send_2[2];

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
    send = OLED.goal_color;
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
    flag = 100;
  }
  else if(message == "D_neo"){
    flag = 13;
  }
  else if(message == "TEST"){
    flag = 14;
    send = OLED.testMode;
  }
  else if(message == "OPTION"){
    flag = 15;
    send = OLED.setplay_flag;
  }
  else if(message == "A_NEO"){
    flag = 16;
  }
  else if(message == "SETPLAY"){
    flag = 17;
    send = OLED.setplay_flag;
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

  if(flag != 100){
    uint8_t send_byte[5] = {38,0,0,0,37};
    send_byte[1] = flag;
    send_byte[2] = byte( send >> 8 ); //ビットシフトで上位側の８Bitを取り出し、バイト型に型変換をする。
    send_byte[3] = byte( send & 0xFF ); //論理和で下位側の８Bitを取り出し、バイト型に型変換をする。
    Serial2.write(send_byte,5);
  }
  else if(flag == 15){
    uint8_t send_byte[5] = {38,0,0,0,37};
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
    if(recieve_byte[0] != 38){
      continue;
    }
    for(int i = 1; i < 7; i++){
      recieve_byte[i] = Serial2.read();
      // Serial.print(" ");
      // Serial.print(recieve_byte[i]);
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
    // Serial.print(" byte : ");
    // Serial.print(recieve_byte[1]);
    // Serial.print(" 0 : ");
    // Serial.print(recieve_int[0]);
    // Serial.print(" 1 : ");
    // Serial.print(recieve_int[1]);

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
      // OLED.line_vec.print();
    }
    else if(recieve_byte[1] == 4){
      if(recieve_int[1] == 0){
        OLED.cam_on = 0;
      }
      else{
        OLED.cam_on = 1;
        OLED.cam_vec.setPolarCoordinates(radians(recieve_int[0]),recieve_int[1]);
      }
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
      D_v = recieve_byte[2];
      D_flag = recieve_byte[3];
      D_A = recieve_byte[4];
      D_A_15_flag = recieve_byte[5];
    }
    else if(recieve_byte[1] == 8){
      if(recieve_int[1] == 0){
        OLED.cam_back_on = 0;
      }
      else{
        OLED.cam_back_on = 1;
        OLED.cam_back_vec.setPolarCoordinates(radians(recieve_int[0]),recieve_int[1]);
      }
      // OLED.cam_back_vec.print();
    }
    else if(recieve_byte[1] == 9){
      OLED.line_on = 1;
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++){
          OLED.line_on_all[i * 8 + j] = recieve_byte[i + 2] % 2;
          recieve_byte[i + 2] /= 2;
          if(OLED.line_on_all[i] != 0){
            OLED.line_on_all[i] = 1;
          }
          if(i == 3 && 2 <= j){
            break;
          }
        }
      }
      // for(int i = 0; i < 27; i++){
      //   Serial.print(" ");
      //   Serial.print(OLED.line_on_all[i]);
      // }
      if(OLED.line_on_all[24] == 1 && OLED.line_on_all[25] == 0){
        OLED.line_side_flag = 1;
      }
      else if(OLED.line_on_all[24] == 0 && OLED.line_on_all[25] == 1){
        OLED.line_side_flag = 2;
      }
      else if(OLED.line_on_all[24] == 1 && OLED.line_on_all[25] == 1 && OLED.line_on_all[26] == 0){
        OLED.line_side_flag = 3;
      }
      else if(OLED.line_on_all[26] == 1 && OLED.line_on_all[25] == 0 && OLED.line_on_all[24] == 0){
        OLED.line_side_flag = 4;
      }
      else if(OLED.line_on_all[24] == 1 && OLED.line_on_all[25] == 1 && OLED.line_on_all[26] == 1){
        OLED.line_side_flag = 4;
      }
      else{
        OLED.line_side_flag = 0;
      }
      Serial.println();
    }
    else if(recieve_byte[1] == 10){
      OLED.ball_catch_val = recieve_int[0];
    }
    else if(recieve_byte[1] == 11){
      A_A = recieve_byte[2];
      if(recieve_byte[2] == 11){
        if(recieve_byte[3] != 240){
          OLED.cam_on = 1;
          OLED.cam_vec.setPolarCoordinates(radians(recieve_byte[3] - 60),20);
        }
        else{
          OLED.cam_on = 0;
        }
        if(recieve_byte[4] != 240){
          OLED.cam_back_on = 1;
          OLED.cam_back_vec.setPolarCoordinates(radians(recieve_byte[4] - 60),20);
        }
        else{
          OLED.cam_back_on = 0;
        }
      }
      else{
        OLED.cam_on = 0;
        OLED.cam_back_on = 0;
      }
    }

    // Serial.println();
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