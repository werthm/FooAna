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
    Int_t LaunchProgressServer();

    TString FormatTimeSec(Double_t seconds);
    TString ExtractFileName(const Char_t* s);

    TAxis CreateVariableAxis(const Char_t* binning);
    Int_t CalcBinOverlapWeights(TAxis* axis, Double_t x, Double_t x_width,
                                std::vector<std::pair<Int_t, Double_t>>& out);

    void Calculate4Vector(Double_t theta, Double_t phi, Double_t t, Double_t mass,
                          TLorentzVector& p4);
}

#endif

