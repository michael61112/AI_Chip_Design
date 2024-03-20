#include "conv.h"

void Conv::setup_env()
{
    string file_name;
    ifstream file;
    // Load weights
    file_name = "data/conv" + to_string(c.LAYER_ID) + "_weight.txt";
    file.open(file_name);
    weights = new DataType ***[c.CH_OUT];
    for(int i = 0; i < c.CH_OUT; i++)
    {
        weights[i] = new DataType **[c.CH_IN];
        for(int j = 0; j < c.CH_IN; j++)
        {
            weights[i][j] = new DataType *[c.KER];
            for(int k = 0; k < c.KER; k++)
            {
                weights[i][j][k] = new DataType[c.KER];
                for(int l = 0; l < c.KER; l++)
                    file >> weights[i][j][k][l];
            }
        }
    }
    file.close();

    // Load bias
    file_name = "data/conv" + to_string(c.LAYER_ID) + "_bias.txt";
    file.open(file_name);
    bias = new DataType[c.CH_OUT];
    for(int i = 0; i < c.CH_OUT; i++)
        file >> bias[i];
    file.close();

    // Initialize input and psum
    input = new DataType **[c.CH_IN];
    for(int i = 0; i < c.CH_IN; i++)
    {
        input[i] = new DataType *[c.IN];
        for(int j = 0; j < c.IN; j++)
            input[i][j] = new DataType[c.IN];
    }
}

void Conv::run()
{
    int count;
    int x, y, z;
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
            else if(state == INPUT && count == c.IN * c.IN * c.CH_IN)
            {
                state = CALC;
                count = 0;
            }
            else if(state == CALC && count == c.OUT * c.OUT * c.CH_OUT)
            {
                state = END;
                count = 0;
                out_valid.write(0);
            }
            //  State operation
            if(state == INPUT && in_valid.read())
            {
                z = count / (c.IN * c.IN);
                y = (count - z * c.IN * c.IN) / c.IN;
                x = count % c.IN;
                input[z][y][x] = in_data.read();
                count++;
            }
            else if(state == CALC)
            {
                out_valid.write(1);
                z = count / (c.OUT * c.OUT);
                y = (count - z * c.OUT * c.OUT) / c.OUT;
                x = count % c.OUT;
                // Convolution
                ans = bias[z];
                for(int i = 0; i < c.CH_IN; i++)
                    for(int j = 0; j < c.KER; j++)
                        for(int k = 0; k < c.KER; k++)
                        {
                            int in_y = y*c.STRIDE - c.PAD + j;
                            int in_x = x*c.STRIDE - c.PAD + k;
                            if(in_y < 0 || in_y >= c.IN || in_x < 0 || in_x >= c.IN)
                                continue;
                            else
                                ans += input[i][in_y][in_x] * weights[z][i][j][k];
                        }
                // ReLU
                if(c.ACT == "relu")
                    ans = (ans > 0) ? ans : 0;
                out_data.write(ans);
                count++;
            }
        }
        wait();
    }
}