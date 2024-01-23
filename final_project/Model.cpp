#include "Model.h"

bool Model::load(string file_name)
{
    if(!readModelFile(file_name))
        return false;
    printLayers();
    if(!partition())
        return false;
    return true;   
}

bool Model::readModelFile(string file_name)
{
    ifstream f;
    string word;
    LayerInfo layer;
    int num_H, num_W, num_C;

    layers.clear();
    cout << "<Info> Read model file ... ";
    f.open(file_name);
    if(!f)
    {
        cout << "<Error> " << file_name << " does not exist." << endl;
        return false;
    }
    f >> name;
    layer.layer_id = 0;
    while(f >> word)
    {
        if(word == "Input")
        {
            if(layer.layer_id != 0)
            {
                cout << "<Error> Input should be placed in front of all layers!!" << endl;
                return false;
            }
            f >> num_H >> num_W >> num_C;
            layer.i.H = num_H;
            layer.i.W = num_W;
            layer.i.C = num_C;
        }
        else
        {
            if(word == "Conv")
            {
                layer.type = CONV;
                f >> num_H >> num_W >> num_C;
                layer.w.H = num_H;
                layer.w.W = num_W;
                layer.w.C = layer.i.C;
                layer.o.H = layer.i.H - layer.w.H + 1;
                layer.o.W = layer.i.W - layer.w.W + 1;
                layer.o.C = num_C;
                f >> word;
                layer.act_func = getActFunc(word);
            }   
            else if(word == "MaxPool")
            {
                layer.type = MAXPOOL;
                f >> num_H >> num_W;
                layer.w.H = 0;
                layer.w.W = 0;
                layer.w.C = 0;
                layer.o.H = layer.i.H / num_H;
                layer.o.W = layer.i.W / num_W;
                layer.o.C = layer.i.C;
                layer.act_func = getActFunc("-");
            }
            else if(word == "FC")
            {
                layer.type = FC;
                f >> num_H;
                // flatten
                layer.i.H = layer.i.H * layer.i.W * layer.i.C;
                layer.i.W = 1;
                layer.i.C = 1;
                layer.w.H = layer.i.H;
                layer.w.W = 1;
                layer.w.C = 1;
                layer.o.H = 1;
                layer.o.W = 1;
                layer.o.C = num_H;
                f >> word;
                layer.act_func = getActFunc(word);
            }
            layers.push_back(layer);
            layer.i.H = layer.o.H;
            layer.i.W = layer.o.W;
            layer.i.C = layer.o.C;
            layer.layer_id++;
        }
    }
    f.close();
    cout << "Done." << endl;
    return true;
}

void Model::printLayers()
{
    LayerInfo layer;
    int len = layers.size();
    char word[30];
    cout << "<Info> Print layers infomation." << endl;
    
    cout << "============================================================================" << endl;
    cout << " Layer |     Type     Input Size    Output Size     Weight Size   Act Func. " << endl;
    cout << "----------------------------------------------------------------------------" << endl;
    for(int i=0; i<len; i++)
    {
        layer = layers[i];
        cout << setw(6) << layer.layer_id << " | " << setw(8) << getLayerStr(layer.type);
        if(layer.type == FC)
        {
            sprintf(word, "%d", layer.i.H*layer.i.W*layer.i.C);
            cout << setw(15) << word;
            sprintf(word, "%d", layer.o.H*layer.o.W*layer.o.C);
            cout << setw(15) << word;
            sprintf(word, "%dx%d", layer.w.H*layer.w.W*layer.w.C, layer.o.C);
            cout << setw(16) << word;
        }
        else
        {
            sprintf(word, "%dx%dx%d", layer.i.H, layer.i.W, layer.i.C);
            cout << setw(15) << word;
            sprintf(word, "%dx%dx%d", layer.o.H, layer.o.W, layer.o.C);
            cout << setw(15) << word;
            if(layer.type == CONV)
                sprintf(word, "%dx%dx%dx%d", layer.w.H, layer.w.W, layer.w.C, layer.o.C);
            else if(layer.type == MAXPOOL)
                sprintf(word, "0");
            cout << setw(16) << word;
        }
        if(layer.act_func == RELU)
            cout << setw(11) << "relu" << endl;
        else if(layer.act_func == SIGMOID)
            cout << setw(11) << "sigmoid" << endl;
        else if(layer.act_func == NO_ACT)
            cout << setw(11) << "-" << endl;
    }
    cout << "============================================================================" << endl;
}

bool Model::partition()
{
    LayerInfo layer;
    int len = layers.size();
    int PE_num;
    int i_reg_size, w_reg_size, p_reg_size, b_reg_size;

    cout << "<Info> Partition layers to PEs ... " << endl;
     
    cout << "============================================================" << endl;
    cout << " Layer |     Type    Input    Weight    PSum    Bias    PE  " << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << setw(6) << "max" << " | "
         << setw(17) << I_REG_SIZE
         << setw(10) << W_REG_SIZE
         << setw(8)  << P_REG_SIZE 
         << setw(8) << P_REG_SIZE 
         << setw(6) << "-" << endl;
    cout << "------------------------------------------------------------" << endl;

    for(int i=0; i<len; i++)
    {
        layer = layers[i];
        i_reg_size = layer.i.H * layer.i.W;
        w_reg_size = layer.w.H * layer.w.W * layer.o.C;
        p_reg_size = layer.o.H * layer.o.W;
        b_reg_size = layer.o.C;
        if(layer.type == CONV)
        {
            cout << setw(6)  << layer.layer_id << " | " 
                 << setw(8)  << getLayerStr(layer.type)
                 << setw(9)  << i_reg_size
                 << setw(10) << w_reg_size
                 << setw(8)  << p_reg_size
                 << setw(8)  << b_reg_size
                 << setw(6)  << layer.i.C << endl;
            if(layer.i.C == 1)
                p_reg_size = 0;
            if(i_reg_size > I_REG_SIZE)
            {
                cout << "<Error> I_REG_SIZE should set larger." << endl;
                return false;
            }
            if(w_reg_size > W_REG_SIZE)
            {
                cout << "<Error> W_REG_SIZE should set larger." << endl;
                return false;
            }
            if(p_reg_size > P_REG_SIZE)
            {
                cout << "<Error> P_REG_SIZE should set larger." << endl;
                return false;
            }
            if(b_reg_size > P_REG_SIZE)
            {
                cout << "<Error> P_REG_SIZE should set larger." << endl;
                return false;
            }
            layers[i].PE_id = layer.i.C;
        }
        else if(layer.type == MAXPOOL)
        {
            cout << setw(6)  << layer.layer_id << " | " 
                 << setw(8)  << getLayerStr(layer.type)
                 << setw(9)  << i_reg_size
                 << setw(10) << w_reg_size
                 << setw(8)  << 0
                 << setw(8)  << 0
                 << setw(6)  << 1 << endl;
            layers[i].PE_id = 1;
        }
        else if(layer.type == FC)
        {
            p_reg_size = layer.o.C;
            if(p_reg_size > P_REG_SIZE)
            {
                cout << "<Error> P_REG_SIZE should set larger." << endl;
                return false;
            }
            PE_num = W_REG_SIZE / p_reg_size;
            PE_num = ceil(1.0 * i_reg_size / PE_num);
            
            cout << setw(6)  << layer.layer_id << " | " 
                 << setw(8)  << getLayerStr(layer.type)
                 << setw(9)  << i_reg_size / PE_num
                 << setw(10) << w_reg_size / PE_num
                 << setw(8)  << p_reg_size
                 << setw(8)  << b_reg_size
                 << setw(6)  << PE_num << endl;
            layers[i].PE_id = PE_num;
            layers[i].i.H = layers[i].i.H / PE_num;
            layers[i].w.H = layers[i].i.H;
        }
    }
    cout << "============================================================" << endl;
    PE_num = layers[0].PE_id;
    for(int i=1; i<len; i++)
    {
        PE_num += layers[i].PE_id;
        layers[i-1].next_PE_size = layers[i].PE_id;
    }
    cout << "PE utilization = " << PE_num << " / " << GRID_X * GRID_Y << endl;
     
    if(PE_num > GRID_X * GRID_Y)
    {
        cout << "<Error> GRID_X or GRID_Y should set larger." << endl;
        return false;
    }
    cout << "<Info> Partition successfully." << endl;
    return true;
}

string Model::getLayerStr(LayerType type)
{
    switch(type)
    {
        case CONV:    return "CONV";
        case MAXPOOL: return "MAXPOOL";
        case FC:      return "FC";
        default:      return "-";
    }
}

ActFunc Model::getActFunc(string str)
{
    if(str == "relu")
        return RELU;
    else if(str == "sigmoid")
        return SIGMOID;
    else
        return NO_ACT;
}