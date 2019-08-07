/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2_BF3                                                        //
//                                                                      //
// A2 event class (basic, kinfit type 3).                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventA2_BF3
#define FooAna_FAEventA2_BF3

#include "FAEventT.h"
#include "FAParticleA2_BF1.h"
#include "FAParticleA2MC_B.h"

class FAEventA2_BF3 : public FAEventT<Double32_t, FAParticleA2_BF1, FAParticleA2MC_B>
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
    Double32_t pullTheta_Beam;  // beam theta pull
    Double32_t pullPhi_Beam;    // beam phi pull
    Double32_t pullT_Beam;      // beam kinetic energy pull

    FAEventA2_BF3() : FAEventT(),
                      weight(0), taggCh(-1), cbSum(0),
                      nComb(0), comb(0), retCode(1),
                      cl(0), nIter(0),
                      pullTheta_Beam(0), pullPhi_Beam(0), pullT_Beam(0) { }
    virtual ~FAEventA2_BF3() { }

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
        printf("Beam theta pull        : %f\n", pullTheta_Beam);
        printf("Beam phi pull          : %f\n", pullPhi_Beam);
        printf("Beam T pull            : %f\n", pullT_Beam);
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
        pullTheta_Beam = 0;
        pullPhi_Beam = 0;
        pullT_Beam = 0;
    }

    ClassDef(FAEventA2_BF3, 1)  // A2 event class (basic, kinfit type 3)
};

#endif

