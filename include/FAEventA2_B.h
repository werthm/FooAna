/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2                                                            //
//                                                                      //
// Class for A2 events.                                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventA2
#define FooAna_FAEventA2

#include "FAEventT.h"
#include "FAEventBaseA2.h"
#include "FAParticleA2.h"
#include "FAParticleMCA2.h"

class FAEventA2 :
    public FAEventT<FAParticleA2, FAParticleMCA2>,
    public FAEventBaseA2
{

public:
    FAEventA2() : FAEventT(), FAEventBaseA2() { }
    virtual ~FAEventA2() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAEventA2, 1)  // Class for A2 events
};

#endif

