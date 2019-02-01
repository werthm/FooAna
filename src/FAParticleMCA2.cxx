/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleMCA2                                                       //
//                                                                      //
// Base class for presorted analysis A2 MC particles.                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAParticleMCA2.h"

ClassImp(FAParticleMCA2)

//______________________________________________________________________________
void FAParticleMCA2::Print(Option_t* option) const
{
    // Print the content of this class.

    FAParticleMC::Print(option);
    printf("Theta [deg]            : %f\n", theta);
    printf("Phi [deg]              : %f\n", phi);
    printf("Energy                 : %f\n", energy);
}

//______________________________________________________________________________
void FAParticleMCA2::Clear(Option_t* option)
{
    // Clear the content of this class.

    FAParticleMC::Clear(option);
    theta = 0;
    phi = 0;
    energy = 0;
}

