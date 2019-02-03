/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarPlotterEntry                                                    //
//                                                                      //
// A plot entry for FAVarPlotter.                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAVarPlotterEntry
#define FooAna_FAVarPlotterEntry

#include "TAttMarker.h"
#include "TAttLine.h"

class TH1;

class FAVarPlotterEntry : public TNamed, public TAttMarker, public TAttLine
{

protected:
    Int_t fNBins1;          // number of primary bins
    Int_t fNBins2;          // number of secondary bins
    TH1*** fHist;           //[fNBins1][fNBins2] histograms (elements not owned)

public:
    FAVarPlotterEntry() : TNamed(), TAttMarker(), TAttLine(),
                          fNBins1(0), fNBins2(0),
                          fHist(0) { }
    FAVarPlotterEntry(const Char_t* title, Int_t nBins1, Int_t nBins2);
    FAVarPlotterEntry(const FAVarPlotterEntry& orig);
    virtual ~FAVarPlotterEntry();

    TH1* GetHistogram(Int_t bin1, Int_t bin2) const { return fHist[bin1][bin2]; }

    void SetHistogram(Int_t bin1, Int_t bin2, TH1* h) { fHist[bin1][bin2] = h; }

    void FormatHistograms();

    ClassDef(FAVarPlotterEntry, 0)  // analysis variable plotter entry
};

#endif

