/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventCLAS12_BI                                                     //
//                                                                      //
// CLAS12 event class (basic, event info).                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventCLAS12_BI
#define FooAna_FAEventCLAS12_BI

#include <iostream>
#include <iomanip>

#include "FAEventT.h"
#include "FAParticleCLAS12_B.h"
#include "FAParticleCLAS12MC_B.h"

class FAEventCLAS12_BI : public FAEventT<Double32_t, FAParticleCLAS12_B, FAParticleCLAS12MC_B>
{

public:
    Double32_t weight;          // event weight
    Double32_t startTime;       // event start time
    Double32_t rfTime;          // accelerator RF time
    Double_t eventID;           // event ID
    Int_t timestamp;            // UTC timestamp
    Short_t nComb;              // number of combinations
    Short_t comb;               // combination index

    FAEventCLAS12_BI() : FAEventT(),
                         weight(0),
                         startTime(0), rfTime(0),
                         eventID(0), timestamp(0),
                         nComb(0), comb(0) { }
    virtual ~FAEventCLAS12_BI() { }

    virtual void Print(Option_t* option = "") const
    {
        FAEventT::Print(option);
        printf("Weight                 : %f\n", weight);
        printf("Start time             : %f\n", startTime);
        printf("RF time                : %f\n", rfTime);
        printf("Event ID               : ");
        std::cout << std::setprecision(30) << eventID << std::endl;
        printf("Timestamp              : %d\n", timestamp);
        printf("Number of combinations : %d\n", nComb);
        printf("Combination index      : %d\n", comb);
    }
    virtual void Clear(Option_t* option = "")
    {
        FAEventT::Clear(option);
        weight = 0;
        startTime = 0;
        rfTime = 0;
        eventID = 0;
        timestamp = 0;
        nComb = 0;
        comb = 0;
    }

    ClassDef(FAEventCLAS12_BI, 1)  // CLAS12 event class (basic, event info)
};

#endif

