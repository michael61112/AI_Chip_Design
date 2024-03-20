#include "linear.h"

void Linear::setup_env()
{
    string file_name;
    ifstream file;
    // Load weights
    file_name = "data/fc" + to_string(c.LAYER_ID) + "_weight.txt";
    file.open(file_name);
    weight = new DataType*[c.OUT];
    for(int i = 0; i < c.OUT; i++)
    {
        weight[i] = new DataType[c.IN];
        for(int j = 0; j < c.IN; j++)
            file >> weight[i][j];
    }
    file.close();
    // Load bias
    file_name = "data/fc" + to_string(c.LAYER_ID) + "_bias.txt";
    file.open(file_name);
    bias = new DataType[c.OUT];
    for(int i = 0; i < c.OUT; i++)
        file >> bias[i];
    file.close();
    // Initialize input
    input = new DataType[c.IN];
}

void Linear::run()
{
    int count;
    DataType ans;
    while (true)
    {
        if(!rst.read())
        {
            out_valid.write(0);
            state = IDLE;
            count = 0;
        }
        else
        {
            //  State transition
            if(state == IDLE && in_valid.read())
            {
                state = INPUT;
                count = 0;
            }
            else if(state == INPUT && count == c.IN)
            {
                state = CALC;
                count = 0;
            }
            else if(state == CALC && count == c.OUT)
            {
                state = END;
                count = 0;
                out_valid.write(0);
            }
            //  State operation
            if(state == INPUT && in_valid.read())
            {
                input[count] = in_data.read();
                count++;
            }
            else if(state == CALC)
            {
                // Fully connected
                ans = bias[count];
                for(int i = 0; i < c.IN; i++)
                    ans += input[i] * weight[count][i];
                // ReLU
                if (c.ACT == "relu")
                    ans = (ans > 0) ? ans : 0;
                out_data.write(ans);
                out_valid.write(1);
                count++;
            }  
        }
        wait();
    }
}