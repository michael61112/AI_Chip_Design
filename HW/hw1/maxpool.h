#ifndef MAXPOOL_H
#define MAXPOOL_H

#include "systemc.h"
#include "define.h"

SC_MODULE( MaxPool ) {
    sc_in  < bool >  rst;
    sc_in  < bool >  clk;
    sc_in  < bool >  in_valid;
    sc_out < bool >  out_valid;
    sc_in  < DataType > in_data;
    sc_out < DataType > out_data;

    void run();
    void setup_env();

    enum State {IDLE, INPUT, END};
    State state;
    MaxPoolConfig c;
    DataType **input;

    SC_HAS_PROCESS( MaxPool );
    MaxPool( sc_module_name name, MaxPoolConfig config):
           sc_module( name ),
           c( config )
    {
        setup_env();
        SC_THREAD( run );
        sensitive << clk.pos() << rst.neg();
    }
};

#endif