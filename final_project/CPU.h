#ifndef CPU_H
#define CPU_H

#include <iostream>

#include "GlobalParam.h"
#include "Model.h"
#include "systemc.h"

using namespace std;

SC_MODULE(CPU) {
    sc_in_clk clk;
    sc_in<bool> rst_n;

    // to ROM
    sc_port<sc_fifo_out_if<Size> > o_param_size_info;
    sc_port<sc_signal_out_if<bool> > o_load_param_flg;
    sc_port<sc_signal_out_if<bool> > o_load_input_flg;
    sc_port<sc_signal_in_if<bool> > i_finish_flg;
    // to PEs
    sc_port<sc_signal_out_if<LayerInfo> > o_layer_info;
    sc_port<sc_signal_out_if<bool>, GRID_X * GRID_Y> o_setup_PE;
    // to routers
    sc_port<sc_signal_out_if<PosInfo> > o_pos_info;
    sc_port<sc_signal_out_if<bool> > o_setup_router;

    void run();
    Model model;

    SC_CTOR(CPU) {
        SC_THREAD(run);
        sensitive << clk.pos() << rst_n.neg();
    }

   public:
    int placement_time;
    int load_param_time;
    int load_input_time;
    bool load_model_fail;
};

#endif