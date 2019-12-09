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

#include "FAFooAna.h"
#include "TDataType.h"

class TAxis;
class TH1;

namespace FAUtils
{
    Char_t TreeDataTypeToChar(EDataType datatype);
    Int_t LaunchProgressServer();

    TString FormatTimeSec(Double_t seconds);
    TString ExtractFileName(const Char_t* s);
    TString ExpandPath(const Char_t* p);
    Bool_t FileExists(const Char_t* f);
    template <class T>
    Bool_t LoadObject(const Char_t* file, const Char_t* name, T*& out);

    void NormHistogram(TH1* h, TH1* hnorm);
    TAxis CreateVariableAxis(const Char_t* binning);
    Int_t CalcBinOverlapWeights(TAxis* axis, Double_t x, Double_t x_width,
                                std::vector<std::pair<Int_t, Double_t>>& out);

    const FAVector4 CalcVector4(const FAVector3& mom, Double_t mass);

    Double_t CalcEkinTOF(Double_t tof, Double_t mass);
}

#endif

