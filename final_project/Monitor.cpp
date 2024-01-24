#include "Monitor.h"

void Monitor::run() {
    Flit flit;
    int max_idx;
    float max_num;
    while (1) {
        if (!rst_n.read())
            idx = 0;
        else {
            for (int i = 0; i < GRID_Y; i++) {
                if (i_data[i]->num_available()) {
                    flit = i_data[i]->read();
                    if (idx == 0) {
                        cout << "<Info> Monitor detect first output at " << get_cycle() << " th cycle." << endl;
                        max_idx = 0;
                        max_num = flit.data;
                    } else {
                        if (flit.data > max_num) {
                            max_idx = idx;
                            max_num = flit.data;
                        }
                    }
                    cout << idx << ": result = " << flit.data << endl;
                    idx++;
                    if (idx == CLASS_NUM) {
                        cout << "<Info> Monitor detect last output at " << get_cycle() << " th cycle." << endl;
                        cout << "<Info> The answer is " << max_idx << "." << endl;
                        compute_time = get_cycle();
                        sc_stop();
                    }
                }
            }
        }
        wait();
    }
}