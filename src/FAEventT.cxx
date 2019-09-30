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
void FAEventT<VarType, PartType, PartTypeMC>::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("Number of variables    : %lu\n", var.size());
    for (UInt_t i = 0; i < var.size(); i++)
    {
        printf("-> variable %d : %f\n", i+1, var[i]);
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

// template instantiations
template class FAEventT<Double32_t, FAParticleA2_B, FAParticleA2MC_B>;
template class FAEventT<Double32_t, FAParticleA2_BF1, FAParticleA2MC_B>;

