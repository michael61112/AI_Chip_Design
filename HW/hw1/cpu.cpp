#include "cpu.h"

void CPU::run() 
{
    // reset
    while(!rst.read())
    {
        out_valid.write(0);
        for(int i = 0; i < 5; i++)
        {
            top_5_val[i] = 0;
            top_5_index[i] = 0;
        }
        wait();
    }

    // send image data to conv1
    out_valid.write(1);
    cout << "[Info] Start simulation at " << get_cycle() << " th cycle." << endl;
    ifstream file(INPUT_FILE);
    while(file >> val)
    {
        out_data.write(val);
        wait();
    }

    // wait for result from linear3
    index = 0;
    total_val = 0;
    out_valid.write(0);
    cout << "[Info] Finish sending image data at " << get_cycle() << " th cycle." << endl;
    while(index < 1000)
    {
        if(in_valid.read())
        {
            val = in_data.read();
            for(int i = 0; i < 5; i++)
            {
                if(val > top_5_val[i])
                {
                    for(int j = 4; j > i; j--)
                    {
                        top_5_val[j] = top_5_val[j-1];
                        top_5_index[j] = top_5_index[j-1];
                    }
                    top_5_val[i] = val;
                    top_5_index[i] = index;
                    break;
                }
            }
            index++;
            total_val += exp(val);
        }
        wait();
    }
    cout << "[Info] Receive all datas at " << get_cycle() << " th cycle." << endl;
    cout << "[Info] Top 5 possibility: " << endl;
    cout << "====================================================" << endl;
    cout << " idx |      val |   possibility |  class name       " << endl;
    cout << "----------------------------------------------------" << endl;
    for(int i = 0; i < 5; i++)
        printf("%4d | %8.4f | %13.5f |  %s\n", top_5_index[i], top_5_val[i], exp(top_5_val[i]) / total_val * 100, class_name[top_5_index[i]].c_str());
    cout << "====================================================" << endl;
    sc_stop();
}