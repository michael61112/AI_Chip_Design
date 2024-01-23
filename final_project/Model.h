#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include "GlobalParam.h"

using namespace std;

class Model
{
public:
    bool load(string file_name);
    bool readModelFile(string file_name);
    void printLayers();
    bool partition();
    string getLayerStr(LayerType type);
    ActFunc getActFunc(string str);

    string name;
    vector<LayerInfo> layers;
};

#endif