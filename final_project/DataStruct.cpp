#include "DataStruct.h"

ostream& operator << (ostream& os, const Size& I)
{
    os << "(" << I.H << ", " << I.W << ", " << I.C << ")";
    return os;
}
bool operator==(const Size& lhs, const Size& rhs)
{
    return (lhs.W == rhs.W && lhs.H == rhs.H  && lhs.C == rhs.C);
}

ostream& operator << (ostream& os, const Flit& I)
{
    os << "flit: dest = Layer " << I.layer_id << "[" << I.PE_id << "], data = "<< I.data;
    return os;
}
bool operator==(const Flit& lhs, const Flit& rhs)
{
    return (lhs.layer_id == rhs.layer_id && lhs.PE_id == rhs.PE_id  && lhs.type == rhs.type && lhs.data == rhs.data);
}

ostream& operator << (ostream& os, const PosInfo& I)
{
    os << "Layer " << I.layer_id << "[" << I.PE_id << "] is at (" << I.y << ", " << I.x << ")";
    return os;
}
bool operator==(const PosInfo& lhs, const PosInfo& rhs)
{
    return (lhs.layer_id == rhs.layer_id && lhs.PE_id == rhs.PE_id);
}

ostream& operator << (ostream& os, const LayerInfo& I)
{
    os << "Layer " << I.layer_id << "[" << I.PE_id << "]" << endl;
    os << " > input size: " << I.i << endl;
    os << " > weight size: " << I.w << endl;
    os << " > output size: " << I.o << endl;
    return os;
}
bool operator==(const LayerInfo& lhs, const LayerInfo& rhs)
{
    return (lhs.layer_id == rhs.layer_id && lhs.PE_id == rhs.PE_id);
}