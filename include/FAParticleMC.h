/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleMC                                                         //
//                                                                      //
// Base class for presorted analysis MC particles.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticleMC
#define FooAna_FAParticleMC

#include "Rtypes.h"

class FAParticleMC
{

public:
    Int_t pdg;              // PDG ID

    FAParticleMC() : pdg(0) { }
    virtual ~FAParticleMC() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAParticleMC, 1)  // Base class for presorted analysis MC particles
};

#endif

