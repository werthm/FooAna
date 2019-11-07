/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2_BIF1                                                       //
//                                                                      //
// A2 event class (basic, event info, kinfit type 1).                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventA2_BIF1
#define FooAna_FAEventA2_BIF1

#include <iostream>
#include <iomanip>

#include "FAEventT.h"
#include "FAParticleA2_BF1.h"
#include "FAParticleA2MC_B.h"

class FAEventA2_BIF1 : public FAEventT<Double32_t, FAParticleA2_BF1, FAParticleA2MC_B>
{

public:
    Double32_t weight;          // event weight
    Short_t taggCh;             // tagger channel
    Double32_t cbSum;           // CB energy sum
    Double_t eventID;           // event ID
    Int_t timestamp;            // UTC timestamp
    Short_t nComb;              // number of combinations
    Short_t comb;               // combination index
    Short_t retCode;            // KF return code
    Double32_t cl;              // confidence level
    Short_t nIter;              // number of fit iterations

    FAEventA2_BIF1() : FAEventT(),
                       weight(0), taggCh(-1), cbSum(0),
                       eventID(0), timestamp(0),
                       nComb(0), comb(0), retCode(1),
                       cl(0), nIter(0) { }
    virtual ~FAEventA2_BIF1() { }

    virtual void Print(Option_t* option = "") const
    {
        FAEventT::Print(option);
        printf("Weight                 : %f\n", weight);
        printf("Tagger channel         : %d\n", taggCh);
        printf("CB energy sum          : %f\n", cbSum);
        printf("Event ID               : ");
        std::cout << std::setprecision(30) << eventID << std::endl;
        printf("Timestamp              : %d\n", timestamp);
        printf("Number of combinations : %d\n", nComb);
        printf("Combination index      : %d\n", comb);
        printf("Fit return code        : %d\n", retCode);
        printf("Fit confidence level   : %f\n", cl);
        printf("Fit iterations         : %d\n", nIter);
    }
    virtual void Clear(Option_t* option = "")
    {
        FAEventT::Clear(option);
        weight = 0;
        taggCh = -1;
        cbSum = 0;
        eventID = 0;
        timestamp = 0;
        nComb = 0;
        comb = 0;
        retCode = 1;
        cl = 0;
        nIter = 0;
    }

    ClassDef(FAEventA2_BIF1, 1)  // A2 event class (basic, event info, kinfit type 1)
};

#endif

