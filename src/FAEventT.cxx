/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventT                                                             //
//                                                                      //
// This class is the template for FooAna events.                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAEventT.h"
#include "FAParticleA2_B.h"
#include "FAParticleA2_BF1.h"
#include "FAParticleA2MC_B.h"

templateClassImp(FAEventT)

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
FAWrapPrim<VarType>* FAEventT<VarType, PartType, PartTypeMC>::variable(Int_t i)
{
    // Return the variable at index 'i'.

    return (FAWrapPrim<VarType>*)vars[i];
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
FAVector4& FAEventT<VarType, PartType, PartTypeMC>::vector4(Int_t i)
{
    // Return the 4-vector at index 'i'.

    return vec4[i];
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
PartType* FAEventT<VarType, PartType, PartTypeMC>::particle(Int_t i)
{
    // Return the particle at index 'i'.

    return (PartType*)part[i];
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
PartTypeMC* FAEventT<VarType, PartType, PartTypeMC>::particleMC(Int_t i)
{
    // Return the MC particle at index 'i'.

    return (PartTypeMC*)partMC[i];
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::AddVariable(VarType v)
{
    // Add a variable to the list of variables.

    new (vars[nVar++]) FAWrapPrim<VarType>(v);
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::AddVector4(const FAVector4& v)
{
    // Add a vector to the list of 4-vectors.

    vec4.push_back(v);
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::AddVector4(const TLorentzVector& v)
{
    // Add a vector to the list of 4-vectors.

    vec4.push_back(FAVector4(v.Px(), v.Py(), v.Pz(), v.E()));
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::AddParticle(const PartType& p)
{
    // Add a particle to the list of particles.

    new (part[nPart++]) PartType(p);
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::AddParticleMC(const PartTypeMC& p)
{
    // Add a particle to the list of MC particles.

    new (partMC[nPartMC++]) PartTypeMC(p);
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("Number of variables    : %d\n", nVar);
    for (Int_t i = 0; i < nVar; i++)
    {
        printf("-> variable %d\n", i+1);
        vars[i]->Print(option);
    }
    printf("Number of 4-vectors    : %lu\n", vec4.size());
    for (UInt_t i = 0; i < vec4.size(); i++)
    {
        printf("-> 4-vector %d\n", i+1);
        printf("Px: %f  Py: %f  Pz: %f  E: %f\n",
               vec4[i].Px(), vec4[i].Py(), vec4[i].Pz(), vec4[i].E());
    }
    printf("Number of particles    : %d\n", nPart);
    for (Int_t i = 0; i < nPart; i++)
    {
        printf("-> Particle %d\n", i+1);
        part[i]->Print(option);
    }
    printf("Number of MC particles : %d\n", nPartMC);
    for (Int_t i = 0; i < nPartMC; i++)
    {
        printf("-> MC Particle %d\n", i+1);
        partMC[i]->Print(option);
    }
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::Clear(Option_t* option)
{
    // Prepare class for a new event by clearing all members.

    nVar = 0;
    nPart = 0;
    nPartMC = 0;
    vars.Clear();
    vec4.clear();
    part.Clear();
    partMC.Clear();
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
FAEventT<VarType, PartType, PartTypeMC>& FAEventT<VarType, PartType, PartTypeMC>::operator=(const FAEventT& e)
{
    // Assignment operator.

    // check self assignment
    if (this != &e)
    {
        nVar = e.nVar;
        nPart = e.nPart;
        nPartMC = e.nPartMC;
        vars = e.vars;
        vec4 = e.vec4;
        part = e.part;
        partMC = e.partMC;
    }

    return *this;
}

// template instantiations
template class FAEventT<Double32_t, FAParticleA2_B, FAParticleA2MC_B>;
template class FAEventT<Double32_t, FAParticleA2_BF1, FAParticleA2MC_B>;

