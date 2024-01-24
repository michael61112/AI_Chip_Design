#include "ROM.h"


void ROM::read_data() {	
	if ( rom_rd ) 
		data = rom_M[rom_addr.read()];
}



