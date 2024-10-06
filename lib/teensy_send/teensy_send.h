#pragma once


#include<Arduino.h>

class send_log{
    public:
    send_log(int sort_,int data_);
    send_log(){};
    void set_log(int sort_,int data_);

    int sort = 0;
    int send_data = 3838;
};

class send_teensy{
    private:
    send_log log[100];
    int count = 0;
    void send_onedata(int sort_,int data_);
    void send_sixdata(int sort_,int data_[6]);

    public:
    send_teensy(int sort_,int data_);
    void set_data(int sort_,int data_);
    int send_data();
    void print();
};