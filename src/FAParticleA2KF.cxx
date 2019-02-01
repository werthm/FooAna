/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleA2KF                                                       //
//                                                                      //
// Base class for presorted A2 analysis particles.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAParticleA2KF.h"

ClassImp(FAParticleA2KF)

//______________________________________________________________________________
void FAParticleA2KF::Print(Option_t* option) const
{
    // Print the content of this class.

    FAParticleA2::Print(option);
    printf("Theta pull             : %f\n", pullTheta);
    printf("Phi pull               : %f\n", pullPhi);
    printf("T pull                 : %f\n", pullT);
}

//______________________________________________________________________________
void FAParticleA2KF::Clear(Option_t* option)
{
    // Clear the content of this class.

    FAParticleA2::Clear(option);
    pullTheta = 0;
    pullPhi = 0;
    pullT = 0;
}

