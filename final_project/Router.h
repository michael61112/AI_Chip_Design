#ifndef ROUTER_H
#define ROUTER_H

#include "systemc.h"
#include "GlobalParam.h"
#include <iostream>
#include <vector>

using namespace std;

SC_MODULE(Router)
{
    sc_in_clk clk;
    sc_in<bool> rst_n;

    sc_port<sc_signal_in_if<PosInfo> > i_pos_info; // from CPU
    sc_port<sc_signal_in_if<bool> > i_setup;       // from CPU

    sc_port<sc_fifo_in_if<Flit>, 5> i_data;
    sc_port<sc_fifo_out_if<Flit>, 5> o_data;

    int x;
    int y;
    bool is_valid;
    bool buffer_has_data[5];
    Flit buffer[5];
    PosInfo route_table[GRID_Y][GRID_X];

    void run();
    void write_flit(int src, int dest, Flit flit);
    PosInfo getPos(Flit flit);

    SC_CTOR(Router)
    {
        SC_THREAD(run);
        sensitive << clk.pos() << rst_n.neg();
    }

public:
    void setXY(int x_pos, int y_pos);
};

#endif
