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

class TLorentzVector;
class TAxis;

namespace FAUtils
{
    Char_t TreeDataTypeToChar(EDataType datatype);
    Int_t ShowBanner();
    TString FormatTimeSec(Double_t seconds);
    Int_t LaunchProgressServer();
    TAxis CreateVariableAxis(const Char_t* binning);

    void Calculate4Vector(Double_t theta, Double_t phi, Double_t t, Double_t mass,
                          TLorentzVector& p4);
}

#endif

