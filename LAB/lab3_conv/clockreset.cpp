#include "clockreset.h"

void Clock::do_it() {
    clk = clk_intern;
    count++;

    if (count == number * 2)
        sc_stop();
}

void Reset::do_it() {
    rst = 1;
    wait(ticks, SC_NS);
    rst = 0;
}
