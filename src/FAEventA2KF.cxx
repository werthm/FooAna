/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventA2KF                                                          //
//                                                                      //
// Class for A2 (kinfit) events.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAEventA2KF.h"

ClassImp(FAEventA2KF)

//______________________________________________________________________________
void FAEventA2KF::Print(Option_t* option) const
{
    // Print the content of this class.

    FAEventT::Print(option);
    FAEventBaseA2::Print(option);
    printf("Number of combinations : %d\n", nComb);
    printf("Combination index      : %d\n", comb);
    printf("Fit return code        : %d\n", retCode);
    printf("Fit confidence level   : %f\n", cl);
    printf("Fit iterations         : %d\n", nIter);
}

//______________________________________________________________________________
void FAEventA2KF::Clear(Option_t* option)
{
    // Clear the content of this class.

    FAEventT::Clear(option);
    FAEventBaseA2::Clear(option);
    nComb = 0;
    comb = 0;
    retCode = 1;
    cl = 0;
    nIter = 0;
}

