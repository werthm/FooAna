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


#ifndef FooAna_FAUtils
#define FooAna_FAUtils

#include "TDataType.h"

namespace FAUtils
{
    Char_t TreeDataTypeToChar(EDataType datatype);
    Int_t ShowBanner();
    TString FormatTimeSec(Double_t seconds);
    Int_t LaunchProgressServer();
}

#endif

