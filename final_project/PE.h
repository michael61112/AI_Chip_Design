#ifndef PE_H
#define PE_H

#include "systemc.h"
#include "GlobalParam.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

SC_MODULE(PE)
{
    sc_in_clk clk;
    sc_in<bool> rst_n;

    sc_port<sc_signal_in_if<LayerInfo> > i_layer_info;
    sc_port<sc_signal_in_if<bool> > i_setup;

    sc_port<sc_fifo_in_if<Flit> > i_data;
    sc_port<sc_fifo_out_if<Flit> > o_data;

    LayerInfo layer;
    bool is_valid;

    // total computation cycle
    int cycle;
    int cut_cycle;

    int w_max;
    int i_max;
    int p_max;
    int channel_max;
    int w_cnt;
    int i_cnt;
    int p_cnt;
    int channel_cnt;

    float i_reg[I_REG_SIZE];
    float w_reg[W_REG_SIZE];
    float p_reg[P_REG_SIZE];

    void run();
    void runCNN();
    void runMaxPool();
    void runFC();

    SC_CTOR(PE)
    {
        is_valid = 0;
        SC_THREAD(run);
        sensitive << clk.pos() << rst_n.neg();
    }
};

#endif
