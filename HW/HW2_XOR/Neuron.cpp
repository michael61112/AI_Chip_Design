#include "Neuron.h"

void Neuron::neuron() {
 
	// vvvvv put your code here vvvvv
	output_temp = (input1.read() * w1 + input2.read() * w2 + b);
	if (output_temp > 0.5) {
		y = 1;
	}
	else {
		y = 0;
	}
	output.write(y);
	// ^^^^^ put your code here ^^^^^
}
