#include <iostream>
#include <math.h>
#include "FM.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

FM::FM(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);

  if (!kv.to_int("I",I))
    I = 1; //default value
  else if (!kv.to_float("N1",N1))
    N1 = 5;
  else if (!kv.to_float("N2",N2))
    N2 = 1;

}


void FM::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();

    A = vel / 127.;
    f0 = 440.0 * pow(2.0, (note-69.0)/12.0);
    fc = N1*f0;
    fm = N2*f0;
    alpha1 = 2*M_PI *fc/SamplingRate;
    alpha2 = 2*M_PI*fm/SamplingRate;

  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & FM::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = A * sin(aux1 + I*sin(aux2));
    aux1 = aux1 + alpha1;
    aux2 = aux2 + alpha2;
  
    if (aux1>=2*M_PI)
      aux1=0;
    else if (aux2>=2*M_PI)
      aux2=0;
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
