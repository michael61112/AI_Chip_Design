#include "CPU.h"
#include "Monitor.h"
#include "PE.h"
#include "ROM.h"
#include "Router.h"
#include "clockreset.h"

using namespace std;

int sc_main(int argc, char** argv) {
    char* str;
    // =======================
    //   signals declaration
    // =======================
    sc_signal<bool> clk;
    sc_signal<bool> rst_n;
    // ROM
    sc_fifo<Size> param_size_info;
    sc_signal<bool> load_param_flg;
    sc_signal<bool> load_input_flg;
    sc_signal<bool> finish_flg;
    // routers
    sc_fifo<Flit> to_east[GRID_Y + 1][GRID_X + 1];
    sc_fifo<Flit> from_east[GRID_Y + 1][GRID_X + 1];
    sc_fifo<Flit> to_south[GRID_Y + 1][GRID_X + 1];
    sc_fifo<Flit> from_south[GRID_Y + 1][GRID_X + 1];
    sc_fifo<Flit> to_local[GRID_Y][GRID_X];
    sc_fifo<Flit> from_local[GRID_Y][GRID_X];
    // CPU
    sc_signal<LayerInfo> layer_info;
    sc_signal<PosInfo> pos_info;
    sc_signal<bool> setup_PE[GRID_Y][GRID_X];
    sc_signal<bool> setup_router;

    // =======================
    //   modules declaration
    // =======================
    Reset m_reset("m_reset", 15);
    Clock m_clock("m_clock", CYCLE_TIME);
    CPU m_CPU("m_CPU");
    ROM m_ROM("m_ROM");
    Monitor m_monitor("m_monitor");

    sc_vector<sc_vector<PE> > m_PE("m_PE");
    sc_vector<sc_vector<Router> > m_router("m_router");

    if (m_ROM.load_data_fail)
        return 1;

    m_PE.init(GRID_Y);
    m_router.init(GRID_Y);

    for (int i = 0; i < GRID_Y; i++) {
        m_PE[i].init(GRID_X);
        m_router[i].init(GRID_X);
    }
    for (int i = 0; i < GRID_Y; i++)
        for (int j = 0; j < GRID_X; j++)
            m_router[i][j].setXY(j, i);

    // =======================
    //   modules connection
    // =======================
    m_clock(clk);
    m_reset(rst_n);
    // CPU
    m_CPU.clk(clk);
    m_CPU.rst_n(rst_n);
    m_CPU.o_param_size_info(param_size_info);
    m_CPU.o_load_param_flg(load_param_flg);
    m_CPU.o_load_input_flg(load_input_flg);
    m_CPU.i_finish_flg(finish_flg);
    m_CPU.o_layer_info(layer_info);
    m_CPU.o_pos_info(pos_info);
    m_CPU.o_setup_router(setup_router);
    for (int i = 0; i < GRID_Y; i++)
        for (int j = 0; j < GRID_X; j++)
            m_CPU.o_setup_PE(setup_PE[i][j]);
    // ROM
    m_ROM(clk, param_size_info, load_param_flg, load_input_flg, finish_flg, to_east[1][0]);
    // monitor
    m_monitor.clk(clk);
    m_monitor.rst_n(rst_n);
    for (int i = 0; i < GRID_Y; i++)
        m_monitor.i_data(to_east[i + 1][GRID_X]);
    // PEs
    for (int i = 0; i < GRID_Y; i++)
        for (int j = 0; j < GRID_X; j++)
            m_PE[i][j](clk, rst_n, layer_info, setup_PE[i][j], to_local[i][j], from_local[i][j]);
    // routers
    for (int i = 0; i < GRID_Y; i++) {
        for (int j = 0; j < GRID_X; j++) {
            m_router[i][j].clk(clk);
            m_router[i][j].rst_n(rst_n);
            m_router[i][j].i_pos_info(pos_info);
            m_router[i][j].i_setup(setup_router);
            //
            m_router[i][j].i_data(from_east[i + 1][j + 1]);
            m_router[i][j].i_data(from_south[i + 1][j + 1]);
            m_router[i][j].i_data(to_east[i + 1][j]);
            m_router[i][j].i_data(to_south[i][j + 1]);
            m_router[i][j].i_data(from_local[i][j]);
            //
            m_router[i][j].o_data(to_east[i + 1][j + 1]);
            m_router[i][j].o_data(to_south[i + 1][j + 1]);
            m_router[i][j].o_data(from_east[i + 1][j]);
            m_router[i][j].o_data(from_south[i][j + 1]);
            m_router[i][j].o_data(to_local[i][j]);
        }
    }
    cout << "<Info> Execution start!!" << endl;
    sc_start();
    if (!m_CPU.load_model_fail) {
        cout << "<Info> Execution complete!!" << endl;
        cout << "<Info> Total execution " << get_cycle() << " cycles." << endl;

        // print timing report
        cout << "<Info> Print timing report." << endl;
        cout << "=================================" << endl;
        cout << "  items             |    cycles  " << endl;
        cout << "---------------------------------" << endl;
        cout << "  placement         |  " << setw(8) << m_CPU.placement_time << endl;
        cout << "  load parameters   |  " << setw(8) << m_CPU.load_param_time - m_CPU.placement_time << endl;
        cout << "  load inputs       |  " << setw(8) << m_CPU.load_input_time - m_CPU.load_param_time << endl;
        cout << "  computation       |  " << setw(8) << m_monitor.compute_time - m_CPU.load_input_time << endl;
        cout << "---------------------------------" << endl;
        cout << "  total             |  " << setw(8) << get_cycle() << endl;
        cout << "=================================" << endl;
    }
    return 0;
}
