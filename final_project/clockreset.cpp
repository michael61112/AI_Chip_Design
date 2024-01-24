#include "clockreset.h"

void Clock::run() {
    clk.write(clk_intern);
    count++;
}

void Reset::run() {
    rst_n.write(0);
    wait(wait_time, SC_NS);
    rst_n.write(1);
}
