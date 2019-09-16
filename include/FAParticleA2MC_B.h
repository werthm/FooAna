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

#include "TLorentzVector.h"

#include "FAFooAna.h"

class FAParticleA2MC_B
{

public:
    Int_t pdg;              // PDG ID
    FAVector4 vec4;         // 4-vector

    FAParticleA2MC_B() : pdg(0), vec4(0, 0, 0, 0) { }
    FAParticleA2MC_B(Int_t pdg, TLorentzVector& v)
        : pdg(pdg), vec4(v.Px(), v.Py(), v.Pz(), v.E()) { }
    virtual ~FAParticleA2MC_B() { }

    const FAVector4& vector4() const { return vec4; }

    virtual void Print(Option_t* option = "") const
    {
        printf("PDG index              : %d\n", pdg);
        printf("Px                     : %f\n", vec4.Px());
        printf("Py                     : %f\n", vec4.Py());
        printf("Pz                     : %f\n", vec4.Pz());
        printf("Energy                 : %f\n", vec4.E());
    }
    virtual void Clear(Option_t* option = "")
    {
        pdg = 0;
        vec4.SetPxPyPzE(0, 0, 0, 0);
    }

    ClassDef(FAParticleA2MC_B, 1)  // A2 MC particle class (basic)
};

#endif

