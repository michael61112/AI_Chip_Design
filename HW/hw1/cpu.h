#ifndef CPU_H
#define CPU_H

#include "systemc.h"
#include "define.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

SC_MODULE( CPU ) {
    sc_in  < bool >  rst;
    sc_in  < bool >  clk;
    sc_out < bool >  out_valid; // send image data to conv1
    sc_in  < bool >  in_valid;  // receive result from linear3
    sc_out < DataType > out_data;
    sc_in  < DataType > in_data;

    void run();

    string class_name[1000];
    DataType val;
    DataType total_val;
    DataType top_5_val[5];
    int index;
    int top_5_index[5];

    SC_CTOR( CPU )
    {
        // load class names
        ifstream file("data/imagenet_classes.txt");
        for (int i = 0; i < 1000; i++)
            getline(file, class_name[i]);
        file.close();
        // start simulation
        SC_THREAD( run );
        sensitive << clk.pos() << rst.neg();
    }
};

# endif