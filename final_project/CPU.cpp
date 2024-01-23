#include "CPU.h"

void CPU::run()
{
    int len;
    int PE_num;
    LayerInfo layer;
    PosInfo pos_info;
    Size size;
    
    load_model_fail = 0;
    if(!model.load(MODEL_FILE_NAME))
    {
        load_model_fail = 1;
        sc_stop();
        return;
    }
    while(!rst_n.read())
    {
        o_load_param_flg->write(0);
        o_load_input_flg->write(0);
        for(int k=0; k<GRID_X*GRID_Y; k++)
            o_setup_PE[k]->write(0);
        o_setup_router->write(0);
        wait();
    }
    len = model.layers.size();
    for(int i=0; i<len; i++)
    {
        layer = model.layers[i];
        size.H = layer.w.H * layer.w.W;
        size.W = layer.PE_id;
        size.C = layer.o.C;
        o_param_size_info->write(size);
    }
    wait();
    pos_info.x = 0;
    pos_info.y = 0;
    for(int i=0; i<len; i++)
    {
        layer = model.layers[i];
        
        PE_num = layer.PE_id;
        pos_info.layer_id = layer.layer_id;

        for(int j=0; j<PE_num; j++)
        {
            layer.PE_id = j;
            pos_info.PE_id = j;
            if(j == PE_num-1)
                layer.is_last_PE = 1;
            else
                layer.is_last_PE = 0;
            // to PE
            o_layer_info->write(layer);
            for(int k=0; k<GRID_X*GRID_Y; k++)
            {
                if(k == pos_info.y*GRID_X + pos_info.x)
                    o_setup_PE[k]->write(1);
                else
                    o_setup_PE[k]->write(0);
            }
            // to router
            o_pos_info->write(pos_info);
            o_setup_router->write(1);
            wait();
            // update postition
            if(pos_info.y % 2 == 0)   pos_info.x++;
            else                      pos_info.x--;
            if(pos_info.x == GRID_X)
            {
                pos_info.x--;
                pos_info.y++;
            }
            else if(pos_info.x == -1)
            {
                pos_info.x++;
                pos_info.y++;
            }
        }
    }
    o_setup_router->write(0);
    for(int k=0; k<GRID_X*GRID_Y; k++)
        o_setup_PE[k]->write(0);
    // set PE0 again
    o_setup_PE[0]->write(1);
    layer = model.layers[0];
    if(layer.PE_id == 1)   layer.is_last_PE = 1;
    else                   layer.is_last_PE = 0;
    layer.PE_id = 0;
    o_layer_info->write(layer);
    wait();
    o_setup_PE[0]->write(0);
    wait();
    cout << "<Info> Finish placing all layers to PEs at " << get_cycle() << " th cycle." << endl;
    placement_time = get_cycle();

    // start loading parameters to PEs
    o_load_param_flg->write(1);
    wait();
    o_load_param_flg->write(0);
    while(!i_finish_flg->read())
        wait();
    cout << "<Info> Finish loading all parameters to PEs at " << get_cycle() << " th cycle." << endl;
    load_param_time = get_cycle();

    // start loading input image to 1st PE
    o_load_input_flg->write(1);
    wait();
    o_load_param_flg->write(0);
    while(!i_finish_flg->read())
        wait();
    cout << "<Info> Start computation at " << get_cycle() << " th cycle." << endl;
    load_input_time = get_cycle();
}