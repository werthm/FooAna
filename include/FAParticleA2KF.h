/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleA2KF                                                       //
//                                                                      //
// Base class for presorted A2 analysis particles.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticleA2KF
#define FooAna_FAParticleA2KF

#include "FAParticleA2.h"

class FAParticleA2KF : public FAParticleA2
{

public:
    Double32_t pullTheta;       // theta pull
    Double32_t pullPhi;         // phi pull
    Double32_t pullT;           // kinetic energy pull

    FAParticleA2KF() : FAParticleA2(),
                       pullTheta(0), pullPhi(0), pullT(0) { }
    virtual ~FAParticleA2KF() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAParticleA2KF, 1)  // Class for presorted A2 analysis particles (kinfit)
};

#endif

