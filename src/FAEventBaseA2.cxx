/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventBaseA2                                                        //
//                                                                      //
// Base class for A2 events.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAEventBaseA2.h"

ClassImp(FAEventBaseA2)

//______________________________________________________________________________
void FAEventBaseA2::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("Tagger channel         : %d\n", taggCh);
    printf("Coincidence time       : %f\n", coincTime);
    printf("CB energy sum          : %f\n", cbSum);
    printf("Trig. multipl. CB      : %d\n", trigMultCB);
    printf("Trig. multipl. TAPS 1  : %d\n", trigMultTAPSLED1);
    printf("Trig. multipl. TAPS 2  : %d\n", trigMultTAPSLED2);
}

//______________________________________________________________________________
void FAEventBaseA2::Clear(Option_t* option)
{
    // Clear the content of this class.

    taggCh = -1;
    coincTime = 0;
    cbSum = 0;
    trigMultCB = 0;
    trigMultTAPSLED1 = 0;
    trigMultTAPSLED2 = 0;
}

