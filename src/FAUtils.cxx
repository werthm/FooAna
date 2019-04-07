/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAUtils                                                              //
//                                                                      //
// This namespace contains some often used utility functions.           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAUtils.h"
#include "FAVersion.h"

// init static members
static Int_t dummy = FAUtils::ShowBanner();

//______________________________________________________________________________
Char_t FAUtils::TreeDataTypeToChar(EDataType datatype)
{
   // Return the leaflist 'char' for a given datatype.
   // Copied from TTree.cxx

    switch (datatype)
    {
        case kChar_t:     return 'B';
        case kUChar_t:    return 'b';
        case kBool_t:     return 'O';
        case kShort_t:    return 'S';
        case kUShort_t:   return 's';
        case kCounter:
        case kInt_t:      return 'I';
        case kUInt_t:     return 'i';
        case kDouble_t:
        case kDouble32_t: return 'D';
        case kFloat_t:
        case kFloat16_t:  return 'F';
        case kLong_t:     return 0; // unsupported
        case kULong_t:    return 0; // unsupported?
        case kchar:       return 0; // unsupported
        case kLong64_t:   return 'L';
        case kULong64_t:  return 'l';

        case kCharStar:   return 'C';
        case kBits:       return 0; //unsupported

        case kOther_t:
        case kNoType_t:
        default:
            return 0;
    }

    return 0;
}

//______________________________________________________________________________
Int_t FAUtils::ShowBanner()
{
    // Show the FooAna banner.

    printf("\n");
    printf("FooAna %s by Dominik Werthmueller, University of York (2017-2019)\n", FOOANA_VERSION);
    printf("https://github.com/werthm/FooAna\n");
    printf("\n");

    return 0;
}

//______________________________________________________________________________
TString FAUtils::FormatTimeSec(Double_t seconds)
{
    // Return a time string in the format HH:MM:SS of the seconds 'seconds'.

    // convert seconds
    Int_t hours = Int_t(seconds / 3600);
    seconds -= hours * 3600;
    Int_t min = Int_t(seconds / 60);
    seconds -= min * 60;
    Int_t sec = Int_t(seconds);

    // format string
    return TString::Format("%02d:%02d:%02d", hours, min, sec);
}

