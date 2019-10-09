/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAUtilsA2                                                            //
//                                                                      //
// This namespace contains some often used utility functions related to //
// the A2 experiment.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAUtilsA2
#define FooAna_FAUtilsA2

#include <functional>

#include "FAFooAna.h"
#include "FAVarAbs.h"
#include "FAConfigA2.h"

namespace FAUtilsA2
{
    Int_t LoadTaggerCalibration(const Char_t* fileName, Int_t nChannel,
                                Double_t* taggEnergy, Double_t* taggEnergyWidth = 0,
                                Bool_t quiet = kFALSE);

    void SetDetFillFlags(Int_t det, FAVarAbs& v_cb, FAVarAbs& v_taps);
    template <class PartType>
    void FillCBSumPart(FAVarAbs& cbsum, FAVarAbs& n_cb, std::vector<PartType>& part_list,
                       std::function<Bool_t (Int_t)> include_part);
    TString DetectorsAsString(FAConfigA2::FADetectorA2_t d);

    template <class PartType>
    inline const FAVector4 CalcVector4(const PartType& part, Double_t mass);
    template <class PartType>
    inline const FAVector4 CalcVector4TOF(const PartType& part, Double_t mass);
}

#endif

