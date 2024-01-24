#include "Router.h"

int temp = 0;
void Router::run()
{
    Flit flit;
    PosInfo pos_info;
    while (1)
    {
        if (!rst_n.read())
        {
            is_valid = 0;
            for (int i = 0; i < 5; i++)
                buffer_has_data[i] = 0;

            for (int i = 0; i < GRID_Y; i++)
                for (int j = 0; j < GRID_X; j++)
                {
                    route_table[i][j].x = -1;
                    route_table[i][j].y = -1;
                    route_table[i][j].PE_id = -1;
                    route_table[i][j].layer_id = -1;
                }
        }
        else if (i_setup->read())
        {
            is_valid = 1;
            pos_info = i_pos_info->read();
            route_table[pos_info.y][pos_info.x] = pos_info;
        }
        else if (is_valid)
        {
            for (int i = 0; i < 5; i++)
            {
                if (i_data[i]->num_available())
                {
                    if (buffer_has_data[i]) // overflow
                        flit = buffer[i];
                    else
                        flit = i_data[i]->read();
                    pos_info = getPos(flit);
                    if (flit.type == FLAG && i != LOCAL)
                        write_flit(i, LOCAL, flit);
                    else if (pos_info.layer_id == -1)
                        write_flit(i, EAST, flit);
                    else if (pos_info.x == x && pos_info.y == y)
                        write_flit(i, LOCAL, flit);
                    else if (pos_info.y == y)
                    {
                        if (pos_info.x > x)
                            write_flit(i, EAST, flit);
                        else
                            write_flit(i, WEST, flit);
                    }
                    else
                    {
                        if (pos_info.y > y)
                            write_flit(i, SOUTH, flit);
                        else
                            write_flit(i, NORTH, flit);
                    }
                }
            }
        }
        wait();
    }
}

PosInfo Router::getPos(Flit flit)
{
    PosInfo pos_info;
    if (flit.type == FLAG)
    {
        pos_info.layer_id = 0;
        pos_info.PE_id = 0;
        pos_info.y = y;
        if (y % 2 == 0)
            pos_info.x = x - 1;
        else
            pos_info.x = x + 1;
        if (pos_info.x == -1 || pos_info.x == GRID_X)
        {
            pos_info.x = x;
            pos_info.y = y - 1;
        }
        return pos_info;
    }
    for (int i = 0; i < GRID_Y; i++)
        for (int j = 0; j < GRID_X; j++)
        {
            if (route_table[i][j].layer_id == flit.layer_id && route_table[i][j].PE_id == flit.PE_id)
                return route_table[i][j];
        }
    return pos_info;
}

void Router::setXY(int x_pos, int y_pos)
{
    x = x_pos;
    y = y_pos;
}

void Router::write_flit(int src, int dest, Flit flit)
{
    if (o_data[dest]->num_free())
    {
        o_data[dest]->write(flit);
        buffer_has_data[src] = 0;
    }
    else
    {
        buffer[src] = flit;
        buffer_has_data[src] = 1;
    }
}