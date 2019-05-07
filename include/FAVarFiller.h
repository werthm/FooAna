/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarFiller                                                          //
//                                                                      //
// This class manages FAVar analysis variables.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAVarFiller
#define FooAna_FAVarFiller

#include "TNamed.h"

class TTree;

class FAVarAbs;
class FAVarHistogram;
class FAVarList;
class TAxis;

class FAVarFiller : public TNamed
{

public:
    // data filling mode
    enum EFillMode {
        kNone,
        kBinned,
        kUnbinned,
        kBoth
    };

protected:
    TAxis* fBins1;                  // first bin axis (not owned)
    TAxis* fBins2;                  // second bin axis (not owned)
    Int_t fNVar;                    // number of analysis variables
    FAVarAbs** fVar;                //[fNVar] array of analysis variables (elements not owned)
    FAVarAbs* fVarWeight;           // pointer to weighting variable for unbinned (overlap) filling
    EFillMode fMode;                // filling mode
    Bool_t fIsCleanup;              // clean-up flag
    Int_t fNHist;                   // number of histograms
    FAVarHistogram** fHist;         //[fNHist] array of histograms
    TTree*** fTree;                 // output trees for unbinned filling

    void AddHistogram(FAVarHistogram* hist);

public:
    FAVarFiller() : TNamed(),
                    fBins1(0), fBins2(0),
                    fNVar(0), fVar(0), fVarWeight(0),
                    fIsCleanup(kTRUE),
                    fNHist(0), fHist(0),
                    fTree(0) { }
    FAVarFiller(const Char_t* name, const Char_t* title, Bool_t doCleanup = kTRUE);
    virtual ~FAVarFiller();

    void SetBins1(TAxis* axis) { fBins1 = axis; }
    void SetBins2(TAxis* axis) { fBins2 = axis; }

    void AddVariable(FAVarAbs* var);
    void AddVariableList(FAVarList* list);
    void SetWeightVariable(FAVarAbs* var) { fVarWeight = var; }
    void AddHistogram1D(FAVarAbs* varX, Bool_t sumw2 = kTRUE);
    void AddHistogram2D(FAVarAbs* varX, FAVarAbs* varY, Bool_t sumw2 = kFALSE);
    void AddHistogram3D(FAVarAbs* varX, FAVarAbs* varY, FAVarAbs* varZ, Bool_t sumw2 = kFALSE);
    void Init(EFillMode mode);
    Bool_t FindBin(Double_t axisVar1, Double_t axisVar2, Int_t& bin1, Int_t& bin2);
    void Fill(Double_t weight = 1, Double_t axisVar1 = 0, Double_t axisVar2 = 0);
    void Fill(std::function<Double_t(void)> wFunc, Double_t axisVar1 = 0, Double_t axisVar2 = 0);
    void FillBin(Double_t weight = 1, Int_t bin1 = 0, Int_t bin2 = 0);
    void FillOverlap(std::function<Double_t(void)> wFunc, Double_t axisVar1,
                     Double_t axisVarWidth1, Double_t axisVar2 = 0);
    void AddObjectsToList(TList* list);
    void WriteFile(const Char_t* filename, Bool_t flat = kFALSE);

    ClassDef(FAVarFiller, 0)  // analysis variable manager
};

#endif

