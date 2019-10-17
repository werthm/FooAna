/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAAnalysisA2                                                         //
//                                                                      //
// FooAna A2 analysis wrapper.                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAAnalysisA2
#define FooAna_FAAnalysisA2

#include "FAAnalysis.h"

class FAAnalysisA2 : public FAAnalysis
{

protected:
    Int_t fNTagg;                       // number of tagger channels
    Double_t* fTaggE;                   //[fNTagg] tagger energy
    Double_t* fTaggEWidth;              //[fNTagg] tagger energy width
    Double_t fTrig_CB_ESum_Mean;        // mean of CB energy sum software trigger
    Double_t fTrig_CB_ESum_Sigma;       // sigma of CB energy sum software trigger
    Int_t fTrig_Mult_Total;             // total trigger multiplicity
    Bool_t fIsTrig_Mult_TAPS;           // flag indicating if TAPS is included in multiplicity

public:
    FAAnalysisA2() : FAAnalysis(),
                     fNTagg(0), fTaggE(0), fTaggEWidth(0),
                     fTrig_CB_ESum_Mean(0), fTrig_CB_ESum_Sigma(0),
                     fTrig_Mult_Total(0), fIsTrig_Mult_TAPS(kFALSE) { }
    FAAnalysisA2(EAnaMode mode, const Char_t* cfg, const Char_t* treeName = 0);
    virtual ~FAAnalysisA2();

    Int_t GetNTagg() const { return fNTagg; }
    Double_t* GetTaggE() const { return fTaggE; }
    Double_t* GetTaggEWidth() const { return fTaggEWidth; }
    Double_t GetTaggE(Int_t c) const { return fTaggE[c]; }
    Double_t GetTaggERndm(Int_t c) const;
    Double_t GetTaggEWidth(Int_t c) const { return fTaggEWidth[c]; }

    Bool_t IsTrigger(Double_t cb_esum, Int_t mult_cb = 9999, Int_t mult_taps = 9999) const;

    virtual void Print(Option_t* option = "") const;

    ClassDef(FAAnalysisA2, 0)  // A2 analysis wrapper
};

#endif

