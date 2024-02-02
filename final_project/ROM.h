#ifndef ROM_H
#define ROM_H

#include "systemc.h"
#include "GlobalParam.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

SC_MODULE(ROM)
{
    sc_in_clk clk;
    // from CPU
    sc_port<sc_fifo_in_if<Size> > i_param_size_info;
    sc_port<sc_signal_in_if<bool> > i_load_param_flg;
    sc_port<sc_signal_in_if<bool> > i_load_input_flg;
    sc_port<sc_signal_out_if<bool> > o_finish_flg;
    // to PEs
    sc_port<sc_fifo_out_if<Flit> > o_data;

    float rom_I[I_MEM_SIZE];
    float rom_W[W_MEM_SIZE];
    vector<Size> param_sizes;
    vector<int> param_nums;
    int img_size, img_channel;
    int img_num;
    bool finish;

    void run();
    void readWeightFile(string file_name);
    void readInputFile(string file_name);

    SC_CTOR(ROM)
    {
        finish = false;
        load_data_fail = false;
        readWeightFile(WEIGHT_FILE_NAME);
        readInputFile(INPUT_FILE_NAME);
        SC_THREAD(run);
        sensitive << clk.pos();
    }

public:
    bool load_data_fail;
};

#endif
