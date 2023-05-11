#include "Monitor.h"

void Monitor:: monitor() {
	if ( counter > 2 )
		cout<< In1 << "\t\t\t" << In2 << "\t\t\t" << Y << "\n";
	else
		counter++;
}



