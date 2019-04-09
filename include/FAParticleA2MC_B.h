/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleA2MC_B                                                     //
//                                                                      //
// A2 MC particle class (basic).                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticleA2MC_B
#define FooAna_FAParticleA2MC_B

#include "TObject.h"

class FAParticleA2MC_B : public TObject
{

public:
    Int_t pdg;              // PDG ID
    Double32_t theta;       // polar angle
    Double32_t phi;         // azimuthal angle
    Double32_t energy;      // energy

    FAParticleA2MC_B() : TObject(),
                         pdg(0),
                         theta(0), phi(0), energy(0) { }
    virtual ~FAParticleA2MC_B() { }

    virtual void Print(Option_t* option = "") const
    {
        printf("PDG index              : %d\n", pdg);
        printf("Theta [deg]            : %f\n", theta);
        printf("Phi [deg]              : %f\n", phi);
        printf("Energy                 : %f\n", energy);
    }
    virtual void Clear(Option_t* option = "")
    {
        pdg = 0;
        theta = 0;
        phi = 0;
        energy = 0;
    }

    ClassDef(FAParticleA2MC_B, 1)  // A2 MC particle class (basic)
};

#endif

