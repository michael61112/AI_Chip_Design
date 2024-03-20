#ifndef CONV_H
#define CONV_H

#include "systemc.h"
#include "define.h"

SC_MODULE( Conv ) {
    sc_in  < bool >  rst;
    sc_in  < bool >  clk;
    sc_in  < bool >  in_valid;
    sc_out < bool >  out_valid;
    sc_in  < DataType > in_data;
    sc_out < DataType > out_data;

    void run();
    void setup_env();

    enum State {IDLE, INPUT, CALC, END};
    State state;
    ConvConfig c;
    DataType ****weights;
    DataType *bias;
    DataType ***input;

    SC_HAS_PROCESS( Conv );
    Conv( sc_module_name name, ConvConfig config):
           sc_module( name ),
           c( config )
    {
        setup_env();
        SC_THREAD( run );
        sensitive << clk.pos() << rst.neg();
    }
};

#endif