/*
#ifndef ONLY_DATATYPES
#define SC_INCLUDE_FX // enable the fixed-point data types
#include <systemc>
#else
#include "datatypes.h"
#endif
*/
#include <bitset>
#include <iomanip>
#include <string>
#include <cstdint> // for fixed width integers

using std::bitset;

using std::setw;
using std::string;


#include <systemc.h>
#define SC_INCLUDE_FX // enable the fixed-point data types
#include "sysc/datatypes/fx/fx.h"

using sc_dt::fixed;
//using sc_dt::sc_fixed;

int sc_main(int argc, char *argv[])
{
    // 5. SystemC fixed point data types can handle decimal data type easily.
    cout << "SystemC fixed point data types" << endl;
    // Note: In NR, many types of number representation are defined
    // which will be used in the to_string function later.
    sc_numrep NR[] = {SC_NOBASE, SC_DEC, SC_BIN, SC_BIN_US, SC_BIN_SM, SC_OCT,
                      SC_OCT_US, SC_OCT_SM, SC_HEX, SC_HEX_US, SC_HEX_SM, SC_CSD};

    sc_int<5> INT[] = {-13, -3, -1, 0, 1, 3, 13};

    sc_fixed<7, 3> FXD[] = {-13.3, -2.7, -1.0, 0.0, 1.0, 3.3, 13.7};

    sc_fixed<7, 3> FSD[] = {"-13.3", "-2.7", "-1.0", "0.0", "1.0", "3.3", "13.7"};

    string q("'");
    for (unsigned iTC = 0; iTC != 7; iTC++)
    {
        for (unsigned iNR = 1; iNR != 12; iNR++)
        {
            cout << "sc_int       " << std::setw(7) << INT[iTC] << " output as "
                 << std::setw(9) << NR[iNR] << " = " << std::setw(15)
                 << q + INT[iTC].to_string(NR[iNR], true) + q << endl

                 << "sc_fixed     " << std::setw(7) << FXD[iTC] << " output as "
                 << std::setw(9) << NR[iNR] << " = " << std::setw(15)
                 << q + FXD[iTC].to_string(NR[iNR], true) + q << endl

                 << "sc_fixed str " << std::setw(7) << FSD[iTC] << " output as "
                 << std::setw(9) << NR[iNR] << " = " << std::setw(15)
                 << q + FSD[iTC].to_string(NR[iNR], true) + q << endl;
        } // endfor
    }     // endfor

    return 0;

} // end main()
