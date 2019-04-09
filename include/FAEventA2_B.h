/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2_B                                                          //
//                                                                      //
// A2 event class (basic).                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventA2_B
#define FooAna_FAEventA2_B

#include "FAEventT.h"
#include "FAParticleA2.h"
#include "FAParticleMCA2.h"

class FAEventA2_B : public FAEventT<FAParticleA2, FAParticleMCA2>
{

public:
    Double32_t weight;          // event weight
    Short_t taggCh;             // tagger channel
    Double32_t cbSum;           // CB energy sum

    FAEventA2_B() : FAEventT(),
                    weight(0), taggCh(-1), cbSum(0) { }
    virtual ~FAEventA2_B() { }

    virtual void Print(Option_t* option = "") const
    {
        FAEventT::Print(option);
        printf("Weight                 : %f\n", weight);
        printf("Tagger channel         : %d\n", taggCh);
        printf("CB energy sum          : %f\n", cbSum);
    }
    virtual void Clear(Option_t* option = "")
    {
        FAEventT::Clear(option);
        weight = 0;
        taggCh = -1;
        cbSum = 0;
    }

    ClassDef(FAEventA2_B, 1)  // A2 event class (basic)
};

#endif

