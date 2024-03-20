#include "clockreset.h"
#include "cpu.h"
#include "conv.h"
#include "maxpool.h"
#include "linear.h"

int sc_main(int argc, char* argv[])
{
    sc_signal < bool > clk;
    sc_signal < bool > rst_n;

    sc_signal < bool > conv1_in_valid;
    sc_signal < bool > maxpool1_in_valid;
    sc_signal < bool > conv2_in_valid;
    sc_signal < bool > maxpool2_in_valid;
    sc_signal < bool > conv3_in_valid;
    sc_signal < bool > conv4_in_valid;
    sc_signal < bool > conv5_in_valid;
    sc_signal < bool > maxpool3_in_valid;
    sc_signal < bool > linear1_in_valid;
    sc_signal < bool > linear2_in_valid;
    sc_signal < bool > linear3_in_valid;
    sc_signal < bool > cpu_in_valid;

    sc_signal < DataType > conv1_in_data;
    sc_signal < DataType > maxpool1_in_data;
    sc_signal < DataType > conv2_in_data;
    sc_signal < DataType > maxpool2_in_data;
    sc_signal < DataType > conv3_in_data;
    sc_signal < DataType > conv4_in_data;
    sc_signal < DataType > conv5_in_data;
    sc_signal < DataType > maxpool3_in_data;
    sc_signal < DataType > linear1_in_data;
    sc_signal < DataType > linear2_in_data;
    sc_signal < DataType > linear3_in_data;
    sc_signal < DataType > cpu_in_data;
    
    Clock m_clock("clk", CLK_PERIOD);
    Reset m_reset("rst", 15);
    CPU m_cpu("cpu");

    // ==================================================================
    // build AlexNet
    // ------------------------------------------------------------------
    // ConvConfig    (layer_id, ch_in, ch_out, in, out, ker, stride, pad)
    // MaxPoolConfig (layer_id, ch_in, in, ker, stride)
    // FcConfig      (layer_id, in, out)
    // ------------------------------------------------------------------
    Conv m_conv1("conv1", ConvConfig(1, 3, 64, 224, 55, 11, 4, 2));
    MaxPool m_maxpool1("maxpool1", MaxPoolConfig(1, 64, 55, 3, 2));
    Conv m_conv2("conv2", ConvConfig(2, 64, 192, 27, 27, 5, 1, 2));
    MaxPool m_maxpool2("maxpool2", MaxPoolConfig(2, 192, 27, 3, 2));
    Conv m_conv3("conv3", ConvConfig(3, 192, 384, 13, 13, 3, 1, 1));
    Conv m_conv4("conv4", ConvConfig(4, 384, 256, 13, 13, 3, 1, 1));
    Conv m_conv5("conv5", ConvConfig(5, 256, 256, 13, 13, 3, 1, 1));
    MaxPool m_maxpool3("maxpool3", MaxPoolConfig(5, 256, 13, 3, 2));

    Linear m_linear1("linear1", FcConfig(6, 9216, 4096));
    Linear m_linear2("linear2", FcConfig(7, 4096, 4096));
    Linear m_linear3("linear3", FcConfig(8, 4096, 1000, "none"));
    // ==================================================================

    // connect modules
    m_clock( clk );
    m_reset( rst_n );
    
    m_cpu      ( rst_n, clk, conv1_in_valid,    cpu_in_valid,      conv1_in_data,    cpu_in_data);
    m_conv1    ( rst_n, clk, conv1_in_valid,    maxpool1_in_valid, conv1_in_data,    maxpool1_in_data);
    m_maxpool1 ( rst_n, clk, maxpool1_in_valid, conv2_in_valid,    maxpool1_in_data, conv2_in_data);
    m_conv2    ( rst_n, clk, conv2_in_valid,    maxpool2_in_valid, conv2_in_data,    maxpool2_in_data);
    m_maxpool2 ( rst_n, clk, maxpool2_in_valid, conv3_in_valid,    maxpool2_in_data, conv3_in_data);
    m_conv3    ( rst_n, clk, conv3_in_valid,    conv4_in_valid,    conv3_in_data,    conv4_in_data);
    m_conv4    ( rst_n, clk, conv4_in_valid,    conv5_in_valid,    conv4_in_data,    conv5_in_data);
    m_conv5    ( rst_n, clk, conv5_in_valid,    maxpool3_in_valid, conv5_in_data,    maxpool3_in_data);
    m_maxpool3 ( rst_n, clk, maxpool3_in_valid, linear1_in_valid,  maxpool3_in_data, linear1_in_data);
    m_linear1  ( rst_n, clk, linear1_in_valid,  linear2_in_valid,  linear1_in_data,  linear2_in_data);
    m_linear2  ( rst_n, clk, linear2_in_valid,  linear3_in_valid,  linear2_in_data,  linear3_in_data);
    m_linear3  ( rst_n, clk, linear3_in_valid,  cpu_in_valid,      linear3_in_data,  cpu_in_data);

    sc_start();
    return 0;
}