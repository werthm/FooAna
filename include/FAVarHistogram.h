/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarHistogram                                                       //
//                                                                      //
// Class for histograms to be filled with analysis variables.           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAVarHistogram
#define FooAna_FAVarHistogram

#include "TObject.h"

class TH1;
class TFile;
class FAVarAbs;

class FAVarHistogram : public TObject
{

protected:
    TAxis* fBins1;               // first bin axis (not owned)
    TAxis* fBins2;               // second bin axis (not owned)
    TH1*** fHist;                // array of histograms
    TH1*** fHistNorm;            // array of normalization histograms
    FAVarAbs* fVarX;             // x-variable (not owned)
    FAVarAbs* fVarY;             // y-variable (not owned)
    FAVarAbs* fVarZ;             // z-variable (not owned)
    Bool_t fIsSumw2;             // flag for storing weights

public:
    FAVarHistogram() : TObject(),
                       fBins1(0), fBins2(0),
                       fHist(0), fHistNorm(0),
                       fVarX(0), fVarY(0), fVarZ(0),
                       fIsSumw2(kFALSE) { }
    FAVarHistogram(FAVarAbs* varX, FAVarAbs* varY = 0, FAVarAbs* varZ = 0);
    virtual ~FAVarHistogram();

    FAVarAbs* GetVarX() const { return fVarX; }
    FAVarAbs* GetVarY() const { return fVarY; }
    FAVarAbs* GetVarZ() const { return fVarZ; }

    void SetBins1(TAxis* axis) { fBins1 = axis; }
    void SetBins2(TAxis* axis) { fBins2 = axis; }
    void SetSumw2(Bool_t s) { fIsSumw2 = s; }

    void CreateHistograms();
    void Fill(Double_t weight, Int_t bin1, Int_t bin2);
    void Fill(Double_t weight, Double_t part_weight, Int_t bin1, Int_t bin2);
    void AddHistogramsToList(TList* list);
    void WriteToFile(TFile* fout, Bool_t flat = kFALSE);

    ClassDef(FAVarHistogram, 1)  // analysis variable histogram class
};

#endif

