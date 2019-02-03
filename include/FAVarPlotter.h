/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarPlotter                                                         //
//                                                                      //
// Class for plotting analysis variables.                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAVarPlotter
#define FooAna_FAVarPlotter

#include "TNamed.h"

#include "FAVarPlotterEntry.h"

class TList;
class TVirtualPad;
class TCanvas;

class FAVarPlotter : public TNamed
{

public:
    // normalization mode
    enum ENormMode {
        kNone,
        kIntegral,
        kMaximum
    };

protected:
    struct AxisConfig_t {
        Double_t fMin;
        Double_t fMax;
        Int_t fRebin;
        Bool_t fIsLog;
        AxisConfig_t() : fMin(0), fMax(0), fRebin(0), fIsLog(kFALSE) { }
    };

    struct BinningConfig_t {
        TString fName;
        Int_t fNBins;
        BinningConfig_t() : fName(""), fNBins(0) { }
    };

    struct LegendConfig_t {
        Double_t fX1;
        Double_t fY1;
        Double_t fX2;
        Double_t fY2;
        LegendConfig_t() : fX1(0.65), fY1(0.7), fX2(0.85), fY2(0.85) { }
    };

    BinningConfig_t fBins1;                         // primary bin config
    BinningConfig_t fBins2;                         // secondary bin config
    std::vector<FAVarPlotterEntry> fEntries;        // array of plot entries
    TList* fHistList;                               // list of histograms
    AxisConfig_t fAxisConfigX;                      // x-axis configuration
    AxisConfig_t fAxisConfigY;                      // y-axis configuration
    AxisConfig_t fAxisConfigZ;                      // z-axis configuration
    ENormMode fNormMode;                            // normalization mode
    LegendConfig_t fLegConfig;                      // legend config

    Bool_t DetermineBins(const Char_t* file);
    void ConfigureAxis(TAxis* a, AxisConfig_t& c);
    void ConfigurePad(TVirtualPad* p);
    void DrawBin1D(Int_t bin1, Int_t bin2, TVirtualPad* pad);
    void DrawBin2D(Int_t bin1, Int_t bin2, TVirtualPad* pad);

public:
    FAVarPlotter() : TNamed(),
                     fBins1(), fBins2(),
                     fEntries(0), fHistList(0),
                     fAxisConfigX(), fAxisConfigY(), fAxisConfigZ(),
                     fNormMode(kNone) { }
    FAVarPlotter(const Char_t* name,
                 const Char_t* binName1 = 0, const Char_t* binName2 = 0);
    virtual ~FAVarPlotter();

    void AddEntry(const Char_t* file, const Char_t* title, const Color_t color);

    Int_t GetNEntry() const { return fEntries.size(); }
    const FAVarPlotterEntry& GetEntry(Int_t i) const { return fEntries[i]; }

    void SetMarkerColor(Color_t c);
    void SetMarkerStyle(Style_t s);
    void SetMarkerSize(Size_t s);
    void SetLineColor(Color_t c);
    void SetLineStyle(Style_t s);
    void SetLineWidth(Width_t w);
    void SetAxisRangeX(Double_t min, Double_t max)
    {
        fAxisConfigX.fMin = min;
        fAxisConfigX.fMax = max;
    }
    void SetAxisRangeY(Double_t min, Double_t max)
    {
        fAxisConfigY.fMin = min;
        fAxisConfigY.fMax = max;
    }
    void SetAxisRangeZ(Double_t min, Double_t max)
    {
        fAxisConfigZ.fMin = min;
        fAxisConfigZ.fMax = max;
    }
    void SetAxisRebinX(Int_t r) { fAxisConfigX.fRebin = r; }
    void SetAxisRebinY(Int_t r) { fAxisConfigY.fRebin = r; }
    void SetAxisRebinZ(Int_t r) { fAxisConfigZ.fRebin = r; }
    void SetAxisLogX(Bool_t log = kTRUE) { fAxisConfigX.fIsLog = log; }
    void SetAxisLogY(Bool_t log = kTRUE) { fAxisConfigY.fIsLog = log; }
    void SetAxisLogZ(Bool_t log = kTRUE) { fAxisConfigZ.fIsLog = log; }
    void SetNormMode(ENormMode mode) { fNormMode = mode; }
    void SetLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
    {
        fLegConfig.fX1 = x1;
        fLegConfig.fY1 = y1;
        fLegConfig.fX2 = x2;
        fLegConfig.fY2 = y2;
    }

    TCanvas* DrawBin(Int_t bin1, Int_t bin2,
                     Int_t cWidth = 700, Int_t cHeight = 500);
    TCanvas* DrawBins(Int_t bin1, Int_t cWidth = 700, Int_t cHeight = 500);
    TCanvas* DrawBins(Int_t cWidth = 700, Int_t cHeight = 500);

    ClassDef(FAVarPlotter, 0)  // analysis variable plotter
};

#endif

