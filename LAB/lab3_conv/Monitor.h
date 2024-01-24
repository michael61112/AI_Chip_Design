#include <iostream>

#include "systemc.h"

using namespace std;

SC_MODULE(Monitor) {
    sc_in<bool> rst;
    sc_in<bool> clk;
    sc_in<sc_int<19> > data_in;
    sc_in<bool> data_in_signal;

    void monitor();
    int x, y;

    SC_CTOR(Monitor) {
        SC_METHOD(monitor);
        sensitive << rst.pos() << clk.pos();
    }
};
