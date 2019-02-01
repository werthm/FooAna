/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticle                                                           //
//                                                                      //
// Base class for presorted analysis particles.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticle
#define FooAna_FAParticle

#include "Rtypes.h"

class FAParticle
{

public:
    Int_t detector;         // detector index/pattern
    Short_t detElem;        // detector element index
    FAParticle() : detector(0), detElem(0) { }
    virtual ~FAParticle() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAParticle, 1)  // Base class for presorted analysis particles
};

#endif

