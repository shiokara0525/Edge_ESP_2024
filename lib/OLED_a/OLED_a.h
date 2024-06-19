#pragma once

#include<Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <timer.h>
#include<Arduino.h>
#include<myVector.h>

extern int sendtoTeensy(const char*,int);
extern Adafruit_NeoPixel pixels;

#define NUMPIXELS 16
#define PIN 32


/*------------------------------------------------------------------------------------------------------------*/

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C for 128x64, 0x3D for 128x32
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

#define EEPROM_SIZE 12
#define DEF_NUM 3838

#define EEPROM_LINE 0
#define EEPROM_RA 1
#define EEPROM_VAL 2
#define EEPROM_BUTTON 3
#define EEPROM_CHECK 4
#define EEPROM_BALL 10
#define EEPROM_MODE 11

class oled_attack{
    public:
    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    const int bluetooth = 11;
    timer timer_OLED;
    unsigned int address = 0x00;  //EEPROMのアドレス
    int toogle = 0;  //トグルスイッチの値を記録（トグルを引くときに使う）
    int goDir;  //loop関数ないしか使えないangle go_ang.degressの値をぶち込んでグローバルに使うために作った税
    int end_flag = 0;

    char CHECK_str [6][8] = {"CHECK_0","CHECK_1","CHECK_2","CHECK_3","CHECK_4","CHECK_5"};

    void setup();
    void OLED();
    void start();
    void end();
    int first_select();

    void display_main();
    void display_start();
    void select_Mode();
    void display_selectColor();
    void display_waitStart();
    void set_Line_Threshold();
    void display_Line();
    void set_getBall_Threshold();
    void display_Ball();
    void set_MotorVal();
    void set_Avaliable();
    void display_Cam();
    void display_getBall();
    void Kick_test();
    void select_testMode();

    void check_TactSwitch();
    int Button_selectCF = 0;  //コートの方向を決めるときに特殊なことをするので、セレクト変数を変えときますぜよ
    int Robot_Mode = 0; //デフォルトはアタッカー
    int test_flag = 0;
    int Sentor_A = 0;
    int Left_A = 0;
    int Right_A = 0;

    int Target_dir;

    int Left;
    int Sentor;
    int Right;
    timer Left_t;
    timer Sentor_t;
    timer Right_t;

    int A = 0;
    int B = 999;  //ステート初期化のための変数
    int aa = 0;  //タクトスイッチのルーレット状態防止用変数

    int flash_OLED = 0;  //ディスプレイの中で白黒点滅させたいときにつかう
    int OLED_select = 1;  //スイッチが押されたときにどこを選択しているかを示す変数(この数字によって選択画面の表示が変化する)
    int Button_select = 0;  //スイッチが押されたときにどこを選択しているかを示す変数(この数字によってexitかnextかが決まる)


    int check_select = 0;
    int check_flag = 0;
    int check_val[6];

    int testMode = 0;

    const int Tact_Switch[3] = {18,35,19};
    const int Toggle_Switch = 34;  //スイッチのピン番号

    int RA_size;
    int val_max = 200;
    int color;
    int LINE_level = 50;
    int ball_getth = 100;
    int goal_color = 0;
    int Mnone_flag = 0;
    int ac_reset = 0;
    int kick_flag = 0;

    int state;

    int line_on = 0;
    int ball_on = 0;
    int cam_on = 0;

    Vector2D line_vec;
    Vector2D ball_vec;
    Vector2D cam_vec;
    int ac_dir;

    int A_D = 1;
    int addresses[EEPROM_SIZE];
};
/*------------------------------------------------------------------------------------------------------------*/