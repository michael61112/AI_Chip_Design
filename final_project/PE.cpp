#include "PE.h"

void PE::run()
{
    Flit flit;
    while (1)
    {
        if (!rst_n.read())
        {
            w_cnt = 0;
            i_cnt = 0;
            p_cnt = 0;
            cycle = 0;
            is_valid = 0;
        }
        else if (i_setup->read())
        {
            is_valid = 1;
            layer = i_layer_info->read();
            i_max = layer.i.H * layer.i.W;
            w_max = layer.w.H * layer.w.W * layer.o.C;

            if (layer.type == MAXPOOL)
                p_max = 0;
            else if (layer.type == FC)
                p_max = layer.o.C;
            else if (layer.type == CONV)
            {
                if (layer.PE_id == 0)
                    p_max = layer.o.C; // bias
                else
                    p_max = layer.o.H * layer.o.W;
            }

            channel_max = layer.o.C;
            cut_cycle = ceil(1.0 * layer.o.H * layer.o.W * layer.o.C / layer.next_PE_size);
        }
        if (is_valid && i_cnt >= i_max && w_cnt >= w_max && p_cnt >= p_max)
        {
            if (i_cnt > i_max)
                cout << "<Error> Layer " << layer.layer_id << "[" << setw(2) << layer.PE_id << "] input data overflow." << endl;
            if (w_cnt > w_max)
                cout << "<Error> Layer " << layer.layer_id << "[" << setw(2) << layer.PE_id << "] weight data overflow." << endl;
            if (p_cnt > p_max)
                cout << "<Error> Layer " << layer.layer_id << "[" << setw(2) << layer.PE_id << "] psum data overflow." << endl;

            if (layer.type == CONV)
                runCNN();
            else if (layer.type == MAXPOOL)
                runMaxPool();
            else if (layer.type == FC)
                runFC();

            if (!layer.is_last_PE)
                is_valid = 0;
            if (layer.PE_id != 0)
            {
                flit.type = FLAG;
                o_data->write(flit);
            }
        }
        else if (i_data->num_available())
        {
            flit = i_data->read();
            // read input
            if (flit.type == INPUT)
                i_reg[i_cnt++] = flit.data;
            // read weight
            else if (flit.type == WEIGHT)
                w_reg[w_cnt++] = flit.data;
            // read patial sum
            else if (flit.type == PSUM)
                p_reg[p_cnt++] = flit.data;
            // read bias
            else if (flit.type == BIAS)
                p_reg[p_cnt++] = flit.data;
            else if (flit.type == FLAG)
                is_valid = 1;
        }
        wait();
    }
}

void PE::runCNN()
{
    Flit flit;
    float result;
    int w_idx, i_idx, p_idx;

    p_idx = 0;
    flit.PE_id = 0;

    // cout << "<Info> Layer " << layer.layer_id << "["<< setw(2) << layer.PE_id  << "]" << " start computing CONV    (output channel = " << channel_cnt << ")." << endl;
    for (int i = 0; i < layer.o.H; i++)
    {
        for (int j = 0; j < layer.o.W; j++)
        {
            result = 0;
            for (int ii = 0; ii < layer.w.H; ii++)
            {
                for (int jj = 0; jj < layer.w.W; jj++)
                {
                    w_idx = channel_cnt * layer.w.H * layer.w.W + ii * layer.w.W + jj;
                    i_idx = (i + ii) * layer.i.W + (j + jj);
                    result += w_reg[w_idx] * i_reg[i_idx];
                }
            }
            // add bias or patial sum
            if (layer.PE_id == 0)
                result += p_reg[channel_cnt];
            else
                result += p_reg[p_idx++];

            // pass through activation function
            if (layer.is_last_PE)
            {
                if (layer.act_func == RELU)
                    result = (result > 0) ? result : 0;
                else if (layer.act_func == SIGMOID)
                    result = 1 / (1 + exp(-result));
            }
            // write output flit
            flit.data = result;
            if (layer.is_last_PE)
            {
                flit.type = INPUT;
                flit.layer_id = layer.layer_id + 1;
                flit.PE_id = cycle / cut_cycle;
            }
            else
            {
                flit.type = PSUM;
                flit.layer_id = layer.layer_id;
                flit.PE_id = layer.PE_id + 1;
            }
            o_data->write(flit);
            wait();
            cycle++;
        }
    }
    channel_cnt++;
    if (layer.PE_id != 0)
        p_cnt = 0;
    if (channel_cnt == channel_max)
    {
        i_cnt = 0;
        cycle = 0;
    }
}

void PE::runMaxPool()
{
    Flit flit;
    float result;
    int i_idx;

    flit.PE_id = 0;
    flit.type = INPUT;
    flit.layer_id = layer.layer_id + 1;

    int stride_x, stride_y;
    stride_x = layer.i.W / layer.o.W;
    stride_y = layer.i.H / layer.o.H;

    // cout << "<Info> Layer " << layer.layer_id << "[" << setw(2) << layer.PE_id << "]" << " start computing MAXPOOL (output channel = " << channel_cnt << ")." << endl;
    for (int i = 0; i < layer.i.H; i += stride_y)
    {
        for (int j = 0; j < layer.i.W; j += stride_x)
        {
            result = 0;
            for (int ii = 0; ii < stride_y; ii++)
            {
                for (int jj = 0; jj < stride_x; jj++)
                {
                    i_idx = (i + ii) * layer.i.W + (j + jj);
                    if (i_reg[i_idx] > result)
                        result = i_reg[i_idx];
                }
            }
            // write output flit
            flit.data = result;
            flit.PE_id = cycle / cut_cycle;
            o_data->write(flit);
            wait();
            cycle++;
        }
    }
    channel_cnt++;
    i_cnt = 0;
    if (channel_cnt == channel_max)
        cycle = 0;
}

void PE::runFC()
{
    Flit flit;
    float result;
    int w_idx, i_idx, p_idx;

    p_idx = 0;
    flit.PE_id = 0;

    // cout << "<Info> Layer " << layer.layer_id << "[" << setw(2) << layer.PE_id << "]" << " start computing FC." << endl;
    for (int i = 0; i < layer.o.C; i++)
    {
        result = 0;
        for (int ii = 0; ii < layer.w.H; ii++)
        {
            w_idx = i * layer.w.H + ii;
            i_idx = ii;
            result += w_reg[w_idx] * i_reg[i_idx];
        }
        // add bias or patial sum
        result += p_reg[i];
        // pass through activation function
        if (layer.is_last_PE)
        {
            if (layer.act_func == RELU)
                result = (result > 0) ? result : 0;
            else if (layer.act_func == SIGMOID)
                result = 1 / (1 + exp(-result));
        }
        // write output flit
        flit.data = result;
        if (layer.is_last_PE)
        {
            flit.type = INPUT;
            flit.layer_id = layer.layer_id + 1;
            flit.PE_id = cycle / cut_cycle;
        }
        else
        {
            flit.type = PSUM;
            flit.layer_id = layer.layer_id;
            flit.PE_id = layer.PE_id + 1;
        }
        o_data->write(flit);
        wait();
        cycle++;
    }
    if (layer.PE_id != 0)
        p_cnt = 0;
    i_cnt = 0;
    cycle = 0;
}