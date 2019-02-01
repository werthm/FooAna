/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventBase                                                          //
//                                                                      //
// This class is the base for all FooAna events.                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <iomanip>

#include "FAEventBase.h"

ClassImp(FAEventBase)

//______________________________________________________________________________
void FAEventBase::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("Event ID               : ");
    std::cout << std::setprecision(30) << eventID << std::endl;
    printf("Timestamp              : %d\n", timestamp);
    printf("Weight                 : %f\n", weight);
    printf("Flux                   : %f\n", flux);
}

//______________________________________________________________________________
void FAEventBase::Clear(Option_t* option)
{
    // Clear the content of this class.

    eventID = 0;
    timestamp = 0;
    weight = 0;
    flux = 0;
}

