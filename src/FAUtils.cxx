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
    printf("FooAna %s by Dominik Werthmueller\n", FOOANA_VERSION);
    printf("https://github.com/werthm/FooAna\n");
    printf("\n");

    return 0;
}

