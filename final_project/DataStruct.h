#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
using namespace std;

enum ActFunc { NO_ACT,
               RELU,
               SIGMOID };
enum LayerType { NONE,
                 CONV,
                 MAXPOOL,
                 FC };
enum DataType { WEIGHT,
                INPUT,
                PSUM,
                BIAS,
                FLAG };
enum Direction { EAST,
                 SOUTH,
                 WEST,
                 NORTH,
                 LOCAL };

struct Size {
    int W;
    int H;
    int C;
    friend ostream& operator<<(ostream& os, const Size& I);
    friend bool operator==(const Size& lhs, const Size& rhs);
};

struct Flit {
    DataType type;
    int layer_id;
    int PE_id;
    float data;
    friend ostream& operator<<(ostream& os, const Flit& I);
    friend bool operator==(const Flit& lhs, const Flit& rhs);
};

struct PosInfo {
    int layer_id = -1;
    int PE_id = -1;
    int x = -1;
    int y = -1;
    friend ostream& operator<<(ostream& os, const PosInfo& I);
    friend bool operator==(const PosInfo& lhs, const PosInfo& rhs);
};

struct LayerInfo {
    int layer_id;
    int PE_id;
    int next_PE_size;
    bool is_last_PE;
    LayerType type;
    ActFunc act_func;
    Size w;
    Size i;
    Size o;
    friend ostream& operator<<(ostream& os, const LayerInfo& I);
    friend bool operator==(const LayerInfo& lhs, const LayerInfo& rhs);
};

#endif
