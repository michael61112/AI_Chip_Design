#include "systemc.h"
#include "Neuron.h"

SC_MODULE( Neural_Network ) {
	
	sc_in < float > input1, input2;
	sc_out < float > output;
	
	sc_signal < float > c1, c2;
	
	Neuron *N1;
	Neuron *N2;
	Neuron *N3;
	
    SC_CTOR( Neural_Network ) {
		
		// vvvvv put your code here vvvvv
		
		// ^^^^^ put your code here ^^^^^

		N1->w1 = 10;
		N1->w2 = -10;
		N1->b = -5;
		N2->w1 = -10;
		N2->w2 = 10;
		N2->b = -5;
		N3->w1 = 10;
		N3->w2 = 10;
		N3->b = -5;
    }
};

