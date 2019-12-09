/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleCLAS12_B                                                   //
//                                                                      //
// CLAS12 particle class (basic).                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticleCLAS12_B
#define FooAna_FAParticleCLAS12_B

#include "FAConfigCLAS12.h"
#include "FAUtilsCLAS12.h"
#include "FAFooAna.h"

class FAParticleCLAS12_B
{

public:
    FAConfigCLAS12::FADetectorCLAS12_t det; // detector bit map
    FAVector3 mom;                          // momentum [GeV]
    Double32_t time;                        // time [ns]
    Double32_t energy;                      // detected energy [GeV]
    Double32_t deltaE;                      // detected deltaE [GeV]
    Double32_t betaTOF;                     // beta from time-of-flight
    Double32_t betaPID;                     // beta from momentum and PID
    Double32_t chi2PID;                     // chi2 PID

    FAParticleCLAS12_B() : det(FAConfigCLAS12::kNoDet),
                           mom(0, 0, 0), time(0),
                           energy(0), deltaE(0),
                           betaTOF(0), betaPID(0),
                           chi2PID(0) { }
    virtual ~FAParticleCLAS12_B() { }

    virtual void Print(Option_t* option = "") const
    {
        printf("Detectors              : %s\n", FAUtilsCLAS12::DetectorsAsString(det).Data());
        printf("Momentum               : %f, %f, %f\n", mom.X(), mom.Y(), mom.Z());
        printf("Time                   : %f\n", time);
        printf("Energy                 : %f\n", energy);
        printf("DeltaE                 : %f\n", deltaE);
        printf("Beta TOF               : %f\n", betaTOF);
        printf("Beta PID               : %f\n", betaPID);
        printf("Chi2 of PID            : %f\n", chi2PID);
    }
    virtual void Clear(Option_t* option = "")
    {
        det = FAConfigCLAS12::kNoDet;
        mom.SetXYZ(0, 0, 0);
        time = 0;
        energy = 0;
        deltaE = 0;
        betaTOF = 0;
        betaPID = 0;
        chi2PID = 0;
    }

    ClassDef(FAParticleCLAS12_B, 1)  // CLAS12 particle class (basic)
};

#endif

