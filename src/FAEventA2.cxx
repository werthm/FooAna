/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2                                                            //
//                                                                      //
// Class for A2 events.                                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAEventA2.h"

ClassImp(FAEventA2)

//______________________________________________________________________________
void FAEventA2::Print(Option_t* option) const
{
    // Print the content of this class.

    FAEventT::Print(option);
    FAEventBaseA2::Print(option);
}

//______________________________________________________________________________
void FAEventA2::Clear(Option_t* option)
{
    // Clear the content of this class.

    FAEventT::Clear(option);
    FAEventBaseA2::Clear(option);
}

