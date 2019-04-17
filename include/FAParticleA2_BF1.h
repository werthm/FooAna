/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleA2_BF1                                                     //
//                                                                      //
// A2 particle class (basic, kinfit type 1).                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticleA2_BF1
#define FooAna_FAParticleA2_BF1

#include "TObject.h"
#include "TMath.h"

#include "FAUtils.h"

class FAParticleA2_BF1 : public TObject
{

public:
    Int_t detector;         // detector index/pattern
    Short_t detElem;        // detector element index
    Double32_t theta;       // polar angle [rad]
    Double32_t phi;         // azimuthal angle [rad]
    Double32_t energy;      // energy [MeV]
    Double32_t time;        // time [ns]
    Double32_t deltaE;      // deltaE [MeV]
    Double32_t tof;         // normalized time-of-flight [ns]
    Double32_t psa_a;       // PSA angle [deg]
    Double32_t psa_r;       // PSA radius [MeV]
    Double32_t pullTheta;   // theta pull
    Double32_t pullPhi;     // phi pull
    Double32_t pullT;       // kinetic energy pull

    FAParticleA2_BF1() : TObject(),
                         detector(0), detElem(0),
                         theta(0), phi(0), energy(0), time(0),
                         deltaE(0), tof(0),
                         psa_a(0), psa_r(0),
                         pullTheta(0), pullPhi(0), pullT(0) { }
    virtual ~FAParticleA2_BF1() { }

    void Calculate4Vector(TLorentzVector& p4, Double_t mass) const
    {
        FAUtils::Calculate4Vector(theta, phi, energy, mass, p4);
    }
    virtual void Print(Option_t* option = "") const
    {
        printf("Detector               : %d\n", detector);
        printf("Detector element       : %d\n", detElem);
        printf("Theta [deg]            : %f\n", theta*TMath::RadToDeg());
        printf("Phi [deg]              : %f\n", phi*TMath::RadToDeg());
        printf("Energy                 : %f\n", energy);
        printf("Time                   : %f\n", time);
        printf("DeltaE                 : %f\n", deltaE);
        printf("time-of-flight         : %f\n", tof);
        printf("PSA angle              : %f\n", psa_a);
        printf("PSA radius             : %f\n", psa_r);
        printf("Theta pull             : %f\n", pullTheta);
        printf("Phi pull               : %f\n", pullPhi);
        printf("T pull                 : %f\n", pullT);
    }
    virtual void Clear(Option_t* option = "")
    {
        detector = 0;
        detElem = 0;
        theta = 0;
        phi = 0;
        energy = 0;
        time = 0;
        deltaE = 0;
        tof = 0;
        psa_a = 0;
        psa_r = 0;
        pullTheta = 0;
        pullPhi = 0;
        pullT = 0;
    }

    ClassDef(FAParticleA2_BF1, 1)  // A2 particle class (basic, kinfit type 1)
};

#endif

