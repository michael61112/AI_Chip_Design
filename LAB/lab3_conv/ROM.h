#include "systemc.h"
#include <iostream>
#include <fstream>

using namespace std;

SC_MODULE( ROM ) {	
	sc_in_clk clock;
	sc_in < bool > rom_rd;
	sc_in < sc_uint<7> > rom_addr;
	sc_out < float > data;
	
	float rom_M[110];
	ifstream fin;
	void read_data();
	int temp;
	
	SC_CTOR( ROM )
	{
		// vvvvv change the path of input file here vvvvv
		fin.open( "input.txt", ios::in );
		for ( int i = 0 ; i < 110 ; i++ )
			fin >> rom_M[i];
			
		fin.close();
		
		SC_METHOD( read_data );
		sensitive << clock.pos();
	}
};


