#include "systemc.h"
#include <iostream>

using namespace std;

SC_MODULE( Monitor ) {	
	sc_in_clk clock;
	sc_in < float > In1, In2, Y;
	void monitor();
	int counter;	
	
	SC_CTOR( Monitor )
	{	
		cout << "In1\t\t\tIn2\t\t\tY\n";
		counter = 0;
		
		SC_METHOD( monitor );
		sensitive << clock.pos();
	}
};


