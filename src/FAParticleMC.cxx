/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticleMC                                                         //
//                                                                      //
// Base class for presorted analysis MC particles.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAParticleMC.h"

ClassImp(FAParticleMC)

//______________________________________________________________________________
void FAParticleMC::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("PDG index              : %d\n", pdg);
}

//______________________________________________________________________________
void FAParticleMC::Clear(Option_t* option)
{
    // Clear the content of this class.

    pdg = 0;
}

