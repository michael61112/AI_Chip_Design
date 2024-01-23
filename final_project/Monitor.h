#ifndef MONITOR_H
#define MONITOR_H

#include "systemc.h"
#include "GlobalParam.h"
#include <iostream>
#include <vector>

using namespace std;

SC_MODULE( Monitor )
{
    sc_in_clk clk;
    sc_in <bool> rst_n;

    sc_port < sc_fifo_in_if <Flit>, GRID_Y > i_data;

    void run();
    int idx;

    SC_CTOR( Monitor )
	{	
		SC_THREAD( run );
		sensitive << clk.pos();
	}
public:
    int compute_time;
};

#endif