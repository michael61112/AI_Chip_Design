#include "maxpool.h"

void MaxPool::setup_env()
{
    input = new DataType*[c.IN];
    for(int i = 0; i < c.IN; i++)
        input[i] = new DataType[c.IN];
}

void MaxPool::run()
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
                input[y][x] = in_data.read();
                if((x >= c.KER - 1) && 
                   (y >= c.KER - 1) &&
                   (x + 1 - c.KER) % c.STRIDE == 0 &&
                   (y + 1 - c.KER) % c.STRIDE == 0)
                {
                    ans = input[y][x];
                    for(int i = 0; i < c.KER; i++)
                        for(int j = 0; j < c.KER; j++)
                            ans = (ans > input[y - i][x - j]) ? ans : input[y - i][x - j];
                    out_data.write(ans);
                    out_valid.write(1);
                }
                else
                    out_valid.write(0);
                count++;
            }
        }
        wait();
    }
}