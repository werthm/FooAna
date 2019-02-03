/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleA2                                                         //
//                                                                      //
// Base class for presorted A2 analysis particles.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticleA2
#define FooAna_FAParticleA2

#include "FAParticle.h"

class TLorentzVector;

class FAParticleA2 : public FAParticle
{

public:
    Double32_t theta;       // polar angle [rad]
    Double32_t phi;         // azimuthal angle [rad]
    Double32_t energy;      // energy [MeV]
    Double32_t deltaE;      // deltaE [MeV]
    Double32_t tof;         // time-of-flight [ns]
    Double32_t psa_a;       // PSA angle [deg]
    Double32_t psa_r;       // PSA radius [MeV]

    FAParticleA2() : FAParticle(),
                     theta(0), phi(0), energy(0),
                     deltaE(0), tof(0),
                     psa_a(0), psa_r(0) { }
    virtual ~FAParticleA2() { }

    virtual void Calculate4Vector(TLorentzVector& p4, Double_t mass) const;
    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAParticleA2, 1)  // Class for presorted A2 analysis particles
};

#endif

