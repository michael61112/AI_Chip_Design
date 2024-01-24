#ifndef GLOBAL_PARAM_H
#define GLOBAL_PARAM_H
// network size
#define GRID_X 7
#define GRID_Y 6
// PE register size
#define I_REG_SIZE 2000
#define W_REG_SIZE 2000
#define P_REG_SIZE 200  // save partial sum or bias
// ROM size
#define I_MEM_SIZE 1024
#define W_MEM_SIZE 65536
#define WEIGHT_FILE_NAME "./model/weight.txt"
#define INPUT_FILE_NAME "./model/input.txt"
#define MODEL_FILE_NAME "./model/model.txt"

#define CYCLE_TIME 10  // unit: ns
#define get_cycle() (sc_time_stamp().to_seconds() * 1e9 / CYCLE_TIME)

#define CLASS_NUM 10

#include "DataStruct.h"

#endif