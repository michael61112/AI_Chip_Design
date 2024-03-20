#ifndef LINEAR_H
#define LINEAR_H

#include "systemc.h"
#include "define.h"

SC_MODULE( Linear ) {
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
    FcConfig c;
    DataType **weight;
    DataType *bias;
    DataType *input;

    SC_HAS_PROCESS( Linear );
    Linear( sc_module_name name, FcConfig config):
           sc_module( name ),
           c( config )
    {
        setup_env();
        SC_THREAD( run );
        sensitive << clk.pos() << rst.neg();
    }
};

#endif