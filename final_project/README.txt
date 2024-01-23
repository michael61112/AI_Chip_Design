===========================
    Compile instruction
===========================
% make
% ./run
or you can use the following instruction to save the terminal output.
% ./run | tee run.log

==========================
    Parameters setting
==========================
users can modify the system architecture by changing the parameters in GlobalParam.h
> GRID_X: number of PEs in one row
> GRID_Y: number of PEs in one column

> I_REG_SIZE: the register size of input data in PE
> W_REG_SIZE: the register size of weight in PE
> P_REG_SIZE: the register size of partial sum (or bias) in PE

> I_MEM_SIZE: the memory size for saving input image
> W_MEM_SIZE: the memory size for saving weight

==========================
    Author information
==========================
> Name:  Tzu-Yun Yen
> Email: yenzu.ee08@nycu.edu.tw
If you have any question, please feel free to contact me ^^