//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DESCRIPTION
//   Examples of various data types in use.
// 1. Efficient native C++ data types. NOT SystemC specialty.
// 2. Enumeration and bitset; NOT SystemC specialty.
// 3. SystemC integer data types.
// 4. SystemC boolean and multi-value data types.
// 5. SystemC fixed point data types.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//#define ONLY_DATATYPES
/* If we define the ONLY_DATATYPES,
 * only the required datatype related headers are included
 * (the required headers are listed in "datatype.h");
 * otherwise, all SystemC headers are included by <systemc>.
 */
#ifndef ONLY_DATATYPES
#define SC_INCLUDE_FX // enable the fixed-point data types
#include <systemc>
#else
#include "datatypes.h"
#endif

#include <bitset>
#include <iomanip>
#include <string>
#include <cstdint> // for fixed width integers

using std::bitset;
using std::cout;
using std::endl;
using std::setw;
using std::string;

using namespace sc_dt;
using namespace sc_core;

#ifndef ONLY_DATATYPES
int sc_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
  // 1. Efficient native C++ data types. NOT SystemC specialty.
  cout << "Native C++ data types" << endl;
  cout << "  sizeof( char ) = " << sizeof(char) << endl;
  cout << "  sizeof( short int ) = " << sizeof(short int) << endl;
  cout << "  sizeof( int ) = " << sizeof(int) << endl;
  cout << "  sizeof( long int ) = " << sizeof(long int) << endl;
  cout << "  sizeof( long long int ) = " << sizeof(long long int) << endl;
  cout << "  sizeof( float ) = " << sizeof(float) << endl;
  cout << "  sizeof( double ) = " << sizeof(double) << endl;
  cout << "Native fixed width C++ data types. These integer types are preferred." << endl;
  cout << "  sizeof( int8_t ) = " << sizeof(int8_t) << endl;
  cout << "  sizeof( int16_t ) = " << sizeof(int16_t) << endl;
  cout << "  sizeof( int32_t ) = " << sizeof(int32_t) << endl;
  cout << "  sizeof( int64_t ) = " << sizeof(int64_t) << endl;
  cout << "  sizeof( uint8_t ) = " << sizeof(uint8_t) << endl;
  cout << "  sizeof( uint16_t ) = " << sizeof(uint16_t) << endl;
  cout << "  sizeof( uint32_t ) = " << sizeof(uint32_t) << endl;
  cout << "  sizeof( uint64_t ) = " << sizeof(uint64_t) << endl;
  cout << endl;

  // 2. Enumeration and bitset; NOT SystemC specialty.
  cout << "Enumeration and bitset in C++" << endl;
  enum Compass { SW, W, NW, N, NE, E, SE, S, nDir }; // Direction of travel
  bitset<nDir> bitset_obstacle;                      // Indication of obstacles
  for (int heading = SW; heading < nDir; heading++) {
    cout << "  Heading " << heading << " ";
    switch (heading) {
    case SW:
      cout << "SW";
      break;
    case W:
      cout << " W";
      break;
    case NW:
      cout << "NW";
      break;
    case N:
      cout << " N";
      break;
    case NE:
      cout << "NE";
      break;
    case E:
      cout << " E";
      break;
    case SE:
      cout << "SE";
      break;
    case S:
      cout << " S";
      break;
    }
    cout << endl;
  } // endfor
  bitset_obstacle.set(NW);
  bitset_obstacle.set(E);
  // Output all values
  cout << "  bitset_obstacle =" << setw(10) << bitset_obstacle << endl;
  cout << endl;

  // 3. SystemC integer data types
  // sc_int, sc_uint, sc_bigint, sc_biguint
  // Note: sc_(u)int cannot represent number with more than 64 bits,
  // but sc_big(u)int can.
  cout << "SystemC integer data types" << endl;
  {
    cout << std::string(40, '-') << endl;
    cout << "  INFO: Adding 3 shorts to produce an int" << endl;
    short a = 32767;
    short b = 32767;
    short c = 32767;
    cout << "  INFO: short a = " << a << endl;
    cout << "  INFO: short b = " << b << endl;
    cout << "  INFO: short c = " << c << endl;
    cout << "  INFO: a + b + c = " << a + b + c << " OK" << endl;
    short ssum = a + b + c;
    cout << "  INFO: a + b + c = ssum = " << ssum << " OOPS" << endl;
    int csum = a + b + c;
    cout << "  INFO: a + b + c = csum = " << csum << " OK" << endl;
  }
  {
    cout << std::string(40, '-') << endl;
    cout << "  INFO: Adding 3 sc_ints to produce a bigger sc_int" << endl;
    sc_int<3> d(3);
    sc_int<5> e(15);
    sc_int<5> f(14);
    cout << "  INFO: sc_int<3> d = " << d << endl;
    cout << "  INFO: sc_int<5> e = " << e << endl;
    cout << "  INFO: sc_int<5> f = " << f << endl;
    cout << "  INFO: d + e + f = " << d + e + f << " OK" << endl;
    sc_int<7> sum = d + e + f;
    cout << "  INFO: d + e + f = sum = " << sum << " OK" << endl;
  }
  {
    cout << std::string(40, '-') << endl;
    cout << "  INFO: Adding 3 sc_ints to produce an sc_bigint" << endl;
    sc_int<64> g("0x7000000000000000");
    sc_int<64> h("0x7000000000000000");
    sc_int<64> i("0x7000000000000000");
    sc_bigint<70> bigsum;
    cout << "  INFO: sc_int<3> g = " << g << endl;
    cout << "  INFO: sc_int<5> h = " << h << endl;
    cout << "  INFO: sc_int<5> i = " << i << endl;
    cout << "  INFO: g + h + i = " << g + h + i << " OOPS" << endl;
    bigsum = g + h + i;
    cout << "  INFO: g + h + i = bigsum = " << bigsum << " OOPS" << endl;
    bigsum = sc_bigint<70>(g) + h + i;
    cout << "  INFO: g + h + i = bigsum = " << bigsum << " OK" << endl;
  }
  cout << endl;

  // 4. SystemC boolean and multi-value data types
  // NOTE: sc_bit & sc_bv are probably better implemented with bool & bitset.
  // sc_logic & sc_lv provide four-value logic: 0, 1, X (unknow), and Z
  // (high-z).
  cout << "SystemC boolean and multi-value data types" << endl;
  bool flag(true); // more efficient to use bool
  sc_bv<5> positions = "01101";
  sc_bv<6> mask = "100111";
  sc_bv<5> active = positions & mask;     // 00101
  sc_bv<1> and_all = active.and_reduce(); // SC_LOGIC_0

  positions.range(3, 2) = "00"; // 00001
  positions[2] = active[0] ^ flag;
  cout << "  INFO: positions is '" << positions << "'" << endl;
  cout << "  INFO:      mask is '" << mask << "'" << endl;
  cout << "  INFO:    active is '" << active << "'" << endl;
  cout << "  INFO:      flag is '" << flag << "'" << endl;
  cout << "  INFO:   and_all is '" << and_all << "'" << endl;

  sc_logic buf(sc_dt::Log_Z);
  sc_lv<8> data_drive("zz01XZ1Z");
  data_drive.range(5, 4) = "ZZ"; // ZZZZXZ1Z
  buf = '1';
  buf = SC_LOGIC_1;
  sc_logic or_all = data_drive.or_reduce();
  cout << "  INFO: buf is '" << buf << "'" << endl;
  cout << "  INFO: data_drive is '" << data_drive << "'" << endl;
  cout << "  INFO: or_all is '" << or_all << "'" << endl;
  cout << endl;

  // 5. SystemC fixed point data types can handle decimal data type easily.
  cout << "SystemC fixed point data types" << endl;
  // Note: In NR, many types of number representation are defined
  // which will be used in the to_string function later.
  sc_numrep NR[] = {SC_NOBASE, SC_DEC,    SC_BIN, SC_BIN_US, SC_BIN_SM, SC_OCT,
                    SC_OCT_US, SC_OCT_SM, SC_HEX, SC_HEX_US, SC_HEX_SM, SC_CSD};

  sc_int<5> INT[] = {-13, -3, -1, 0, 1, 3, 13};

  sc_fixed<7, 3> FXD[] = {-13.3, -2.7, -1.0, 0.0, 1.0, 3.3, 13.7};

  sc_fixed<7, 3> FSD[] = {"-13.3", "-2.7", "-1.0", "0.0", "1.0", "3.3", "13.7"};

  string q("'");
  for (unsigned iTC = 0; iTC != 7; iTC++) {
    for (unsigned iNR = 1; iNR != 12; iNR++) {
      cout << "sc_int       " << std::setw(7) << INT[iTC] << " output as "
           << std::setw(9) << NR[iNR] << " = " << std::setw(15)
           << q + INT[iTC].to_string(NR[iNR], true) + q << std::endl
           << "sc_fixed     " << std::setw(7) << FXD[iTC] << " output as "
           << std::setw(9) << NR[iNR] << " = " << std::setw(15)
           << q + FXD[iTC].to_string(NR[iNR], true) + q << std::endl
           << "sc_fixed str " << std::setw(7) << FSD[iTC] << " output as "
           << std::setw(9) << NR[iNR] << " = " << std::setw(15)
           << q + FSD[iTC].to_string(NR[iNR], true) + q << std::endl;
    } // endfor
  }   // endfor

  return 0;

} // end main()

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
