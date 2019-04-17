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
template <class PartType, class PartTypeMC>
PartType* FAEventT<PartType, PartTypeMC>::particle(Int_t i)
{
    // Return the particle at index 'i'.

    return (PartType*)part[i];
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
PartTypeMC* FAEventT<PartType, PartTypeMC>::particleMC(Int_t i)
{
    // Return the MC particle at index 'i'.

    return (PartTypeMC*)partMC[i];
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
FAVector4* FAEventT<PartType, PartTypeMC>::vector4(Int_t i)
{
    // Return the 4-vector at index 'i'.

    return (FAVector4*)vec4[i];
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddParticle(const PartType& p)
{
    // Add a particle to the list of particles.

    new (part[nPart++]) PartType(p);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddParticleMC(const PartTypeMC& p)
{
    // Add a particle to the list of MC particles.

    new (partMC[nPartMC++]) PartTypeMC(p);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddVector4(const FAVector4& v)
{
    // Add a vector to the list of 4-vectors.

    new (vec4[nVec4++]) FAVector4(v);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddVector4(const TLorentzVector& v)
{
    // Add a vector to the list of 4-vectors.

    new (vec4[nVec4++]) FAVector4(v);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::Print(Option_t* option) const
{
    // Print the content of this class.

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
    printf("Number of 4-vectors    : %d\n", nVec4);
    for (Int_t i = 0; i < nVec4; i++)
    {
        printf("-> 4-vector %d\n", i+1);
        vec4[i]->Print(option);
    }
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::Clear(Option_t* option)
{
    // Prepare class for a new event by clearing all members.

    nPart = 0;
    nPartMC = 0;
    nVec4 = 0;
    part.Clear();
    partMC.Clear();
    vec4.Clear();
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
FAEventT<PartType, PartTypeMC>& FAEventT<PartType, PartTypeMC>::operator=(const FAEventT& e)
{
    // Assignment operator.

    // check self assignment
    if (this != &e)
    {
        nPart = e.nPart;
        nPartMC = e.nPartMC;
        nVec4 = e.nVec4;
        part = e.part;
        partMC = e.partMC;
        vec4 = e.vec4;
    }

    return *this;
}

// template instantiations
template class FAEventT<FAParticleA2_B, FAParticleA2MC_B>;
template class FAEventT<FAParticleA2_BF1, FAParticleA2MC_B>;

