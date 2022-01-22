#ifndef FM_PAV
#define FM_PAV

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class FM: public upc::Instrument {
    EnvelopeADSR adsr;
    unsigned int index;
    int I;
	float A;
  float f0, fc, fm, N1, N2;
  float alpha1, alpha2;
  float aux1=0;
  float aux2=0;
    std::vector<float> tbl;
  public:
    FM(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif
