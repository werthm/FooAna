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


#include "TH1.h"

#include "FAVarPlotterEntry.h"

ClassImp(FAVarPlotterEntry)

//______________________________________________________________________________
FAVarPlotterEntry::FAVarPlotterEntry(const Char_t* title, Int_t nBins1, Int_t nBins2)
    : TNamed(title, title), TAttMarker(kBlack, 1, 1), TAttLine(kBlack, kSolid, 1)
{
    // Constructor.

    // set members
    fNBins1 = nBins1;
    fNBins2 = nBins2;

    // create histogram array
    fHist = new TH1**[fNBins1];
    for (Int_t i = 0; i < fNBins1; i++)
    {
        fHist[i] = new TH1*[fNBins2];
        for (Int_t j = 0; j < fNBins2; j++)
            fHist[i][j] = 0;
    }
}

//______________________________________________________________________________
FAVarPlotterEntry::FAVarPlotterEntry(const FAVarPlotterEntry& orig)
    : TNamed(orig), TAttMarker(orig), TAttLine(orig)
{
    // Copy constructor.

    // set members
    fNBins1 = orig.fNBins1;
    fNBins2 = orig.fNBins2;

    // create histogram array
    fHist = new TH1**[fNBins1];
    for (Int_t i = 0; i < fNBins1; i++)
    {
        fHist[i] = new TH1*[fNBins2];
        for (Int_t j = 0; j < fNBins2; j++)
            fHist[i][j] = orig.fHist[i][j];
    }
}

//______________________________________________________________________________
FAVarPlotterEntry::~FAVarPlotterEntry()
{
    // Destructor.

    if (fHist)
    {
        for (Int_t i = 0; i < fNBins1; i++)
            delete [] fHist[i];
        delete [] fHist;
    }
}

//______________________________________________________________________________
void FAVarPlotterEntry::FormatHistograms()
{
    // Format the histograms.

    // loop over primary bins
    for (Int_t i = 0; i < fNBins1; i++)
    {
        // loop over secondary bins
        for (Int_t j = 0; j < fNBins2; j++)
        {
            // apply the current format
            if (fHist[i][j])
            {
                fHist[i][j]->SetMarkerColor(fMarkerColor);
                fHist[i][j]->SetMarkerStyle(fMarkerStyle);
                fHist[i][j]->SetMarkerSize(fMarkerSize);
                fHist[i][j]->SetLineColor(fLineColor);
                fHist[i][j]->SetLineStyle(fLineStyle);
                fHist[i][j]->SetLineWidth(fLineWidth);
            }
        }
    }
}

