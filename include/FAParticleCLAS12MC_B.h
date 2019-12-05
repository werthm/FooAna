/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleCLAS12MC_B                                                 //
//                                                                      //
// CLAS12 MC particle class (basic).                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAParticleCLAS12MC_B
#define FooAna_FAParticleCLAS12MC_B

#include "FAFooAna.h"

class FAParticleCLAS12MC_B
{

public:
    Int_t pdg;              // PDG ID
    FAVector4 vec4;         // 4-vector

    FAParticleCLAS12MC_B() : pdg(0), vec4(0, 0, 0, 0) { }
    FAParticleCLAS12MC_B(Int_t pdg, FAVector4& v)
        : pdg(pdg), vec4(v.Px(), v.Py(), v.Pz(), v.E()) { }
    virtual ~FAParticleCLAS12MC_B() { }

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

    ClassDef(FAParticleCLAS12MC_B, 1)  // CLAS12 MC particle class (basic)
};

#endif

