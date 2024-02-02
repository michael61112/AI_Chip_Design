#ifndef CLOCKRESET_H
#define CLOCKRESET_H

#include <systemc.h>

SC_MODULE(Clock) {
    sc_out<bool> clk;

   public:
    sc_clock clk_intern;
    int count;
    void run();

    SC_HAS_PROCESS(Clock);

    Clock(sc_module_name name, int clock_cycle) : sc_module(name), clk("clk"), clk_intern(sc_gen_unique_name(name), clock_cycle, SC_NS) {
        count = 0;
        SC_METHOD(run);
        sensitive << clk_intern;
    }
};

SC_MODULE(Reset) {
    sc_out<bool> rst_n;

    int wait_time;
    void run();

    SC_HAS_PROCESS(Reset);

    Reset(sc_module_name name, int reset_time) : sc_module(name), rst_n("rst_n"), wait_time(reset_time) {
        SC_THREAD(run);
    }
};

#endif
