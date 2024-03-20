#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <fstream>
using namespace std;

#define INPUT_FILE "data/cat.txt"

#define DataType float
#define CLK_PERIOD 10 // unit: ns
#define get_cycle() (sc_time_stamp().to_seconds() * 1e9 / CLK_PERIOD)

struct ConvConfig {
    int LAYER_ID;
    int CH_IN;
    int CH_OUT;
    int IN;
    int OUT;
    int KER;
    int STRIDE;
    int PAD;
    string ACT;
    ConvConfig(int layer_id, int ch_in, int ch_out, int in, int out, int ker, int stride = 1, int pad = 0, string act = "relu")
    {
        LAYER_ID = layer_id;
        CH_IN = ch_in;
        CH_OUT = ch_out;
        IN = in;
        OUT = out;
        KER = ker;
        STRIDE = stride;
        PAD = pad;
        ACT = act;
    }
};

struct MaxPoolConfig {
    int LAYER_ID;
    int CH_IN;
    int IN;
    int KER;
    int STRIDE;
    MaxPoolConfig(int layer_id, int ch_in, int in, int ker, int stride)
    {
        LAYER_ID = layer_id;
        CH_IN = ch_in;
        IN = in;
        KER = ker;
        STRIDE = stride;
    }
};

struct FcConfig {
    int LAYER_ID;
    int IN;
    int OUT;
    string ACT;
    FcConfig(int layer_id, int in, int out, string act = "relu")
    {
        LAYER_ID = layer_id;
        IN = in;
        OUT = out;
        ACT = act;
    }
};

#endif