#ifndef CLOCKRESET_H
#define CLOCKRESET_H

#include "systemc.h"

SC_MODULE( Clock ) 
{
public:
    sc_out <bool> clk;
    sc_clock clk_intern;
    int count;
    void run();

    SC_HAS_PROCESS( Clock );

    Clock( sc_module_name name, int cycle_time):
           sc_module( name ),
           clk( "clk" ),
           clk_intern( sc_gen_unique_name( name ), cycle_time, SC_NS )
    {
        SC_METHOD( run );
        count = 0;
        sensitive << clk_intern;
    } 
};

SC_MODULE( Reset ) 
{
    sc_out<bool> rst_n;
    int wait_time;
    void run();
    
    SC_HAS_PROCESS( Reset );

    Reset( sc_module_name name, int reset_time): 
           sc_module( name ),
           rst_n( "rst_n" ), 
           wait_time( reset_time )
    {
        SC_THREAD( run );
    }
};

#endif
