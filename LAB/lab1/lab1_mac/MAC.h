#include "systemc.h"

SC_MODULE( Adder ) {
	sc_in < sc_uint<8> > in1,in2;
	sc_out < sc_uint<9> > out;
	
	void run() {
		// vvvvv put your code here vvvvv
		
		// ^^^^^ put your code here ^^^^^
	}

	SC_CTOR( Adder ) {
		SC_METHOD( run );
		sensitive << in1 << in2;
	}

};

SC_MODULE( Multiplier ) {
	sc_in < sc_uint<4> > in1,in2;
	sc_out < sc_uint<8> > out;
	
	void run() {
		// vvvvv put your code here vvvvv
		
		// ^^^^^ put your code here ^^^^^
	}

	SC_CTOR( Multiplier ) {
		SC_METHOD( run );
		sensitive << in1 << in2;
	}

};

SC_MODULE( MAC ) {
	sc_in < sc_uint<4> > in1, in2;
	sc_in < sc_uint<8> > in3;
	sc_out < sc_uint<9> > out;
    
	Adder *ADD_1;
	Multiplier *MUL_1;
    
	// vvvvv put your code here vvvvv
	
	// ^^^^^ put your code here ^^^^^
		
	SC_CTOR( MAC )
	{
		// vvvvv put your code here vvvvv
		
		
		
		
		
		// ^^^^^ put your code here ^^^^^
	}
};
