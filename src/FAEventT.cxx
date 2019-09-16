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
void FAEventT<VarType, PartType, PartTypeMC>::AddVariable(VarType v)
{
    // Add a variable to the list of variables.

    vars.push_back(v);
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

    part.push_back(p);
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::AddParticleMC(const PartTypeMC& p)
{
    // Add a particle to the list of MC particles.

    partMC.push_back(p);
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("Number of variables    : %lu\n", vars.size());
    for (UInt_t i = 0; i < vars.size(); i++)
    {
        printf("-> variable %d : %f\n", i+1, vars[i]);
    }
    printf("Number of 4-vectors    : %lu\n", vec4.size());
    for (UInt_t i = 0; i < vec4.size(); i++)
    {
        printf("-> 4-vector %d\n", i+1);
        printf("Px: %f  Py: %f  Pz: %f  E: %f\n",
               vec4[i].Px(), vec4[i].Py(), vec4[i].Pz(), vec4[i].E());
    }
    printf("Number of particles    : %lu\n", part.size());
    for (UInt_t i = 0; i < part.size(); i++)
    {
        printf("-> Particle %d\n", i+1);
        part[i].Print(option);
    }
    printf("Number of MC particles : %lu\n", partMC.size());
    for (UInt_t i = 0; i < partMC.size(); i++)
    {
        printf("-> MC Particle %d\n", i+1);
        partMC[i].Print(option);
    }
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
void FAEventT<VarType, PartType, PartTypeMC>::Clear(Option_t* option)
{
    // Prepare class for a new event by clearing all members.

    vars.clear();
    vec4.clear();
    part.clear();
    partMC.clear();
}

//______________________________________________________________________________
template <class VarType, class PartType, class PartTypeMC>
FAEventT<VarType, PartType, PartTypeMC>& FAEventT<VarType, PartType, PartTypeMC>::operator=(const FAEventT& e)
{
    // Assignment operator.

    // check self assignment
    if (this != &e)
    {
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

