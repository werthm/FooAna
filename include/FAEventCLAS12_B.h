/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventCLAS12_B                                                      //
//                                                                      //
// CLAS12 event class (basic).                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventCLAS12_B
#define FooAna_FAEventCLAS12_B

#include "FAEventT.h"
#include "FAParticleCLAS12_B.h"
#include "FAParticleCLAS12MC_B.h"

class FAEventCLAS12_B : public FAEventT<Double32_t, FAParticleCLAS12_B, FAParticleCLAS12MC_B>
{

public:
    Double32_t weight;          // event weight
    Double32_t startTime;       // event start time
    Double32_t rfTime;          // accelerator RF time

    FAEventCLAS12_B() : FAEventT(),
                        weight(0),
                        startTime(0), rfTime(0) { }
    virtual ~FAEventCLAS12_B() { }

    virtual void Print(Option_t* option = "") const
    {
        FAEventT::Print(option);
        printf("Weight                 : %f\n", weight);
        printf("Start time             : %f\n", startTime);
        printf("RF time                : %f\n", rfTime);
    }
    virtual void Clear(Option_t* option = "")
    {
        FAEventT::Clear(option);
        weight = 0;
        startTime = 0;
        rfTime = 0;
    }

    ClassDef(FAEventCLAS12_B, 1)  // CLAS12 event class (basic)
};

#endif

