/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2_BF2                                                        //
//                                                                      //
// A2 event class (basic, kinfit type 2).                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventA2_BF2
#define FooAna_FAEventA2_BF2

#include "FAEventT.h"
#include "FAParticleA2KF.h"
#include "FAParticleMCA2.h"

class FAEventA2_BF2 : public FAEventT<FAParticleA2KF, FAParticleMCA2>
{

public:
    Double32_t weight;          // event weight
    Short_t taggCh;             // tagger channel
    Double32_t cbSum;           // CB energy sum
    Short_t nComb;              // number of combinations
    Short_t comb;               // combination index
    Short_t retCode;            // KF return code
    Double32_t cl;              // confidence level
    Short_t nIter;              // number of fit iterations
    Double32_t zVertex;         // z-vertex
    Double32_t pullZVertex;     // z-vertex pull


    FAEventA2_BF2() : FAEventT(),
                      weight(0), taggCh(-1), cbSum(0),
                      nComb(0), comb(0), retCode(1),
                      cl(0), nIter(0),
                      zVertex(0), pullZVertex(0) { }
    virtual ~FAEventA2_BF2() { }

    virtual void Print(Option_t* option = "") const
    {
        FAEventT::Print(option);
        printf("Weight                 : %f\n", weight);
        printf("Tagger channel         : %d\n", taggCh);
        printf("CB energy sum          : %f\n", cbSum);
        printf("Number of combinations : %d\n", nComb);
        printf("Combination index      : %d\n", comb);
        printf("Fit return code        : %d\n", retCode);
        printf("Fit confidence level   : %f\n", cl);
        printf("Fit iterations         : %d\n", nIter);
        printf("z vertex               : %f\n", zVertex);
        printf("z vertex pull          : %f\n", pullZVertex);
    }
    virtual void Clear(Option_t* option = "")
    {
        FAEventT::Clear(option);
        weight = 0;
        taggCh = -1;
        cbSum = 0;
        nComb = 0;
        comb = 0;
        retCode = 1;
        cl = 0;
        nIter = 0;
        zVertex = 0;
        pullZVertex = 0;
    }

    ClassDef(FAEventA2_BF2, 1)  // A2 event class (basic, kinfit type 2)
};

#endif

