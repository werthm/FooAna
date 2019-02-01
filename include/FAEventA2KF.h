/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2KF                                                          //
//                                                                      //
// Class for A2 (kinfit) events.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventA2KF
#define FooAna_FAEventA2KF

#include "FAEventT.h"
#include "FAEventBaseA2.h"
#include "FAParticleA2KF.h"
#include "FAParticleMCA2.h"

class FAEventA2KF :
    public FAEventT<FAParticleA2KF, FAParticleMCA2>,
    public FAEventBaseA2
{

public:
    Short_t nComb;          // number of combinations
    Short_t comb;           // combination index
    Short_t retCode;        // KF return code
    Double32_t cl;          // confidence level
    Short_t nIter;          // number of fit iterations

    FAEventA2KF() : FAEventT(), FAEventBaseA2(),
                    nComb(0), comb(0), retCode(1),
                    cl(0), nIter(0) { }
    virtual ~FAEventA2KF() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    ClassDef(FAEventA2KF, 1)  // Class for A2 (kinfit) events
};

#endif

