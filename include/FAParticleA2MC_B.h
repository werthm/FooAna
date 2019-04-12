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

#include "FAVector4.h"

class FAParticleA2MC_B : public TObject
{

public:
    Int_t pdg;              // PDG ID
    FAVector4 vector4;      // 4-vector

    FAParticleA2MC_B() : TObject(),
                         pdg(0), vector4(0, 0, 0, 0) { }
    FAParticleA2MC_B(Int_t pdg, TLorentzVector& v)
        : TObject(),
          pdg(pdg), vector4(v) { }
    virtual ~FAParticleA2MC_B() { }

    virtual void Print(Option_t* option = "") const
    {
        printf("PDG index              : %d\n", pdg);
        printf("Px                     : %f\n", vector4.x);
        printf("Py                     : %f\n", vector4.y);
        printf("Pz                     : %f\n", vector4.z);
        printf("Energy                 : %f\n", vector4.t);
    }
    virtual void Clear(Option_t* option = "")
    {
        pdg = 0;
        vector4.Clear();
    }

    ClassDef(FAParticleA2MC_B, 1)  // A2 MC particle class (basic)
};

#endif

