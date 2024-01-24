#include "Monitor.h"


void Monitor:: monitor() {
	
	if ( rst ) {
		x = 0;
		y = 0;
		cout << "Result: " << endl;
		return;
	}
	else if ( data_in_signal ) {
		cout << data_in << "\t";
		
		x = x + 1;
		if ( x == 8 ) {
			x = 0;
			y = y + 1;
			cout << endl;
			if ( y == 8 ) {
				sc_stop();
			}
		}		
	}
}


