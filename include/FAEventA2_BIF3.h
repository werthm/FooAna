/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2_BIF3                                                       //
//                                                                      //
// A2 event class (basic, event info, kinfit type 3).                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventA2_BIF3
#define FooAna_FAEventA2_BIF3

#include <iostream>
#include <iomanip>

#include "FAEventT.h"
#include "FAParticleA2KF.h"
#include "FAParticleMCA2.h"

class FAEventA2_BIF3 : public FAEventT<FAParticleA2KF, FAParticleMCA2>
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
    Double32_t pullTheta_Beam;  // beam theta pull
    Double32_t pullPhi_Beam;    // beam phi pull
    Double32_t pullT_Beam;      // beam kinetic energy pull

    FAEventA2_BIF3() : FAEventT(),
                       weight(0), taggCh(-1), cbSum(0),
                       eventID(0), timestamp(0),
                       nComb(0), comb(0), retCode(1),
                       cl(0), nIter(0),
                       pullTheta_Beam(0), pullPhi_Beam(0), pullT_Beam(0) { }
    virtual ~FAEventA2_BIF3() { }

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
        eventID = 0;
        timestamp = 0;
        nComb = 0;
        comb = 0;
        retCode = 1;
        cl = 0;
        nIter = 0;
        pullTheta_Beam = 0;
        pullPhi_Beam = 0;
        pullT_Beam = 0;
    }

    ClassDef(FAEventA2_BIF3, 1)  // A2 event class (basic, event info, kinfit type 3)
};

#endif

