/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAUtilsCLAS12                                                        //
//                                                                      //
// This namespace contains some often used utility functions related to //
// the CLAS12 experiment.                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TString.h"

#include "FAUtilsCLAS12.h"

//______________________________________________________________________________
TString FAUtilsCLAS12::DetectorsAsString(FAConfigCLAS12::FADetectorCLAS12_t d)
{
    // Return a string of all detectors bit-marked in 'd'.

    TString out;

    // check detectors
    if (d == 0)
        out += "Empty ";
    if (d & FAConfigCLAS12::kCD)
        out += "Central detector ";
    if (d & FAConfigCLAS12::kFD)
        out += "Forward detector ";
    if (d & FAConfigCLAS12::kFT)
        out += "Forward tagger ";

    // format
    out.Remove(TString::kTrailing, ' ');
    out.ReplaceAll(" ", ", ");

    return out;
}

