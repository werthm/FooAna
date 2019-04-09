/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleMCA2                                                       //
//                                                                      //
// Base class for presorted analysis A2 MC particles.                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticleMCA2
#define FooAna_FAParticleMCA2

#include "FAParticleMC.h"

class FAParticleMCA2 : public FAParticleMC
{

public:
    Double32_t theta;       // polar angle
    Double32_t phi;         // azimuthal angle
    Double32_t energy;      // energy

    FAParticleMCA2() : FAParticleMC(),
                       theta(0), phi(0), energy(0) { }
    virtual ~FAParticleMCA2() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAParticleMCA2, 1)  // Base class for presorted analysis A2 MC particles
};

#endif

