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
    Int_t fNTagg;              // number of tagger channels
    Double_t* fTaggE;          //[fNTagg] tagger energy
    Double_t* fTaggEWidth;     //[fNTagg] tagger energy width

public:
    FAAnalysisA2() : FAAnalysis(),
                     fNTagg(0), fTaggE(0), fTaggEWidth(0) { }
    FAAnalysisA2(const Char_t* cfg);
    virtual ~FAAnalysisA2();

    Int_t GetNTagg() const { return fNTagg; }
    Double_t* GetTaggE() const { return fTaggE; }
    Double_t* GetTaggEWidth() const { return fTaggEWidth; }
    Double_t GetTaggE(Int_t c) const { return fTaggE[c]; }
    Double_t GetTaggEWidth(Int_t c) const { return fTaggEWidth[c]; }

    ClassDef(FAAnalysisA2, 0)  // A2 analysis wrapper
};

#endif

