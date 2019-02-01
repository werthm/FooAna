/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAParticle                                                           //
//                                                                      //
// Base class for presorted analysis particles.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAParticle.h"

ClassImp(FAParticle)

//______________________________________________________________________________
void FAParticle::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("Detector               : %d\n", detector);
    printf("Detector element       : %d\n", detElem);
}

//______________________________________________________________________________
void FAParticle::Clear(Option_t* option)
{
    // Clear the content of this class.

    detector = 0;
    detElem = 0;
}

