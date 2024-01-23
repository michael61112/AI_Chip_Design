#include "systemc.h"
#include<iostream>
#include<iomanip>
#include<string>
using namespace std;

SC_MODULE(DT){
	sc_in_clk clk;
	sc_in<bool> reset;
	sc_out<bool> sti_rd;
	sc_out<sc_uint<10> > sti_addr;
	sc_in<sc_uint<16> > sti_di;
	sc_out<bool> res_wr;
	sc_out<bool> res_rd;
	sc_out<sc_uint<14> > res_addr;
	sc_out<sc_uint<8> > res_do;
	sc_in<sc_uint<8> > res_di;
	sc_out<bool> done,fwpass_finish;
		
	SC_CTOR(DT)
	{
	
	}
	~DT(){};
	
};
