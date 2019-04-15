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


#include "TH2.h"
#include "TH3.h"
#include "TFile.h"

#include "FAVarHistogram.h"
#include "FAVarAbs.h"

ClassImp(FAVarHistogram)

//______________________________________________________________________________
FAVarHistogram::FAVarHistogram(FAVarAbs* varX, FAVarAbs* varY, FAVarAbs* varZ)
    : TObject()
{
    // Constructor.

    // init members
    fBins1 = 0;
    fBins2 = 0;
    fHist = 0;
    fVarX = varX;
    fVarY = varY;
    fVarZ = varZ;
}

//______________________________________________________________________________
FAVarHistogram::~FAVarHistogram()
{
    // Destructor.

    if (fHist)
    {
        Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
        Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;
        for (Int_t i = 0; i < nb1; i++)
        {
            for (Int_t j = 0; j < nb2; j++)
            {
                delete fHist[i][j];
            }
            delete [] fHist[i];
        }
        delete [] fHist;
    }
}

//______________________________________________________________________________
void FAVarHistogram::CreateHistograms()
{
    // Create the histograms.

    //
    // check binning of variables
    //

    // for 1d-histograms
    if (fVarX && fVarY == 0 && fVarZ == 0)
    {
        // check if binning was set
        if (!fVarX->GetBins())
        {
            Warning("CreateHistograms", "No binning found for variable '%s'", fVarX->GetName());
            return;
        }
    }
    // for 2d-histograms
    else if (fVarX && fVarY && fVarZ == 0)
    {
        // check if binnings were set
        if (!fVarX->GetBins())
        {
            Warning("CreateHistograms", "No binning found for variable '%s'", fVarX->GetName());
            return;
        }
        if (!fVarY->GetBins())
        {
            Warning("CreateHistograms", "No binning found for variable '%s'", fVarY->GetName());
            return;
        }
    }
    // for 3d-histograms
    else if (fVarX && fVarY && fVarZ)
    {
        Warning("CreateHistograms", "3d-histograms were not implemented yet");
        return;
    }
    else
    {
        Error("CreateHistograms", "Could not create histograms using the provided analysis variables!");
        return;
    }

    // skip solely defined second bin axis
    if (!fBins1 && fBins2)
    {
        Error("CreateHistograms", "Error in bin axis configuration!");
        return;
    }

    // check axes
    Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
    Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;

    // create the histograms
    fHist = new TH1**[nb1];

    // some variables
    TString name;
    TString title;

    // loop over bins of first axis
    for (Int_t i = 0; i < nb1; i++)
    {
        fHist[i] = new TH1*[nb2];

        // loop over bins of second axis
        for (Int_t j = 0; j < nb2; j++)
        {
            // init
            fHist[i][j] = 0;

            // create 1d-histogram
            if (fVarX && fVarY == 0 && fVarZ == 0)
            {
                // format name and title
                if (fBins1 && fBins2)
                {
                    name = TString::Format("%s_%s_%d_%s_%d", fVarX->GetName(),
                                           fBins1->GetName(), i, fBins2->GetName(), j);
                    title = TString::Format("%s (%s: [%.3f,%.3f], %s: [%.3f,%.3f])", fVarX->GetTitle(),
                                            fBins1->GetTitle(), fBins1->GetBinLowEdge(i+1), fBins1->GetBinUpEdge(i+1),
                                            fBins2->GetTitle(), fBins2->GetBinLowEdge(j+1), fBins2->GetBinUpEdge(j+1));
                }
                else if (fBins1 && !fBins2)
                {
                    name = TString::Format("%s_%s_%d", fVarX->GetName(),
                                           fBins1->GetName(), i);
                    title = TString::Format("%s (%s: [%.3f,%.3f])", fVarX->GetTitle(),
                                            fBins1->GetTitle(), fBins1->GetBinLowEdge(i+1), fBins1->GetBinUpEdge(i+1));
                }
                else if (!fBins1 && !fBins2)
                {
                    name = fVarX->GetName();
                    title = fVarX->GetTitle();
                }

                // check for variable binning
                if (fVarX->GetBins()->GetXbins()->fN)
                    fHist[i][j] = new TH1F(name.Data(), title.Data(),
                                           fVarX->GetBins()->GetNbins(),
                                           (Double_t*)fVarX->GetBins()->GetXbins()->GetArray());
                else
                    fHist[i][j] = new TH1F(name.Data(), title.Data(),
                                           fVarX->GetBins()->GetNbins(),
                                           fVarX->GetBins()->GetXmin(),
                                           fVarX->GetBins()->GetXmax());

                // call Sumw2 for 1-dim. histograms
                fHist[i][j]->Sumw2();

                // x-axis title
                if (fVarX->HasUnit())
                    fHist[i][j]->GetXaxis()->SetTitle(TString::Format("%s [%s]", fVarX->GetTitle(), fVarX->GetUnit()));
                else
                    fHist[i][j]->GetXaxis()->SetTitle(fVarX->GetTitle());

                // y-axis title
                if (fVarX->GetBins()->GetXbins()->fN)
                    fHist[i][j]->GetYaxis()->SetTitle("Counts");
                else
                {
                    if (fVarX->HasUnit())
                        fHist[i][j]->GetYaxis()->SetTitle(TString::Format("Counts / %.2f %s",
                                                          fHist[i][j]->GetXaxis()->GetBinWidth(1), fVarX->GetUnit()).Data());
                    else
                        fHist[i][j]->GetYaxis()->SetTitle(TString::Format("Counts / %.2f",
                                                          fHist[i][j]->GetXaxis()->GetBinWidth(1)).Data());
                }
            }
            // create 2d-histogram
            else if (fVarX && fVarY && fVarZ == 0)
            {
                // format name and title
                if (fBins1 && fBins2)
                {
                    name = TString::Format("%s_vs_%s_%s_%d_%s_%d", fVarY->GetName(), fVarX->GetName(),
                                           fBins1->GetName(), i, fBins2->GetName(), j);
                    title = TString::Format("%s vs. %s (%s: [%.3f,%.3f], %s: [%.3f,%.3f])", fVarY->GetTitle(), fVarX->GetTitle(),
                                            fBins1->GetTitle(), fBins1->GetBinLowEdge(i+1), fBins1->GetBinUpEdge(i+1),
                                            fBins2->GetTitle(), fBins2->GetBinLowEdge(j+1), fBins2->GetBinUpEdge(j+1));
                }
                else if (fBins1 && !fBins2)
                {
                    name = TString::Format("%s_vs_%s_%s_%d", fVarY->GetName(), fVarX->GetName(),
                                           fBins1->GetName(), i);
                    title = TString::Format("%s vs. %s (%s: [%.3f,%.3f])", fVarY->GetTitle(), fVarX->GetTitle(),
                                            fBins1->GetTitle(), fBins1->GetBinLowEdge(i+1), fBins1->GetBinUpEdge(i+1));
                }
                else if (!fBins1 && !fBins2)
                {
                    name = TString::Format("%s_vs_%s", fVarY->GetName(), fVarX->GetName());
                    title = TString::Format("%s vs. %s", fVarY->GetTitle(), fVarX->GetTitle());
                }

                // check for variable x-binning
                if (fVarX->GetBins()->GetXbins()->fN)
                {
                    // check for variable y-binning
                    if (fVarY->GetBins()->GetXbins()->fN)
                    {
                        fHist[i][j] = new TH2F(name.Data(), title.Data(),
                                               fVarX->GetBins()->GetNbins(),
                                               (Double_t*)fVarX->GetBins()->GetXbins()->GetArray(),
                                               fVarY->GetBins()->GetNbins(),
                                               (Double_t*)fVarY->GetBins()->GetXbins()->GetArray());
                    }
                    else
                    {
                        fHist[i][j] = new TH2F(name.Data(), title.Data(),
                                               fVarX->GetBins()->GetNbins(),
                                               (Double_t*)fVarX->GetBins()->GetXbins()->GetArray(),
                                               fVarY->GetBins()->GetNbins(),
                                               fVarY->GetBins()->GetXmin(),
                                               fVarY->GetBins()->GetXmax());
                    }
                }
                else
                {
                    // check for variable y-binning
                    if (fVarY->GetBins()->GetXbins()->fN)
                    {
                        fHist[i][j] = new TH2F(name.Data(), title.Data(),
                                               fVarX->GetBins()->GetNbins(),
                                               fVarX->GetBins()->GetXmin(),
                                               fVarX->GetBins()->GetXmax(),
                                               fVarY->GetBins()->GetNbins(),
                                               (Double_t*)fVarY->GetBins()->GetXbins()->GetArray());
                    }
                    else
                    {
                        fHist[i][j] = new TH2F(name.Data(), title.Data(),
                                               fVarX->GetBins()->GetNbins(),
                                               fVarX->GetBins()->GetXmin(),
                                               fVarX->GetBins()->GetXmax(),
                                               fVarY->GetBins()->GetNbins(),
                                               fVarY->GetBins()->GetXmin(),
                                               fVarY->GetBins()->GetXmax());
                    }
                }

                // axes titles
                if (fVarX->HasUnit())
                    fHist[i][j]->GetXaxis()->SetTitle(TString::Format("%s [%s]", fVarX->GetTitle(), fVarX->GetUnit()));
                else
                    fHist[i][j]->GetXaxis()->SetTitle(fVarX->GetTitle());
                if (fVarY->HasUnit())
                    fHist[i][j]->GetYaxis()->SetTitle(TString::Format("%s [%s]", fVarY->GetTitle(), fVarY->GetUnit()));
                else
                    fHist[i][j]->GetYaxis()->SetTitle(fVarY->GetTitle());
            }
        }
    }
}

//______________________________________________________________________________
void FAVarHistogram::Fill(Double_t weight, Int_t bin1, Int_t bin2)
{
    // Fill the histogram in 'bin1', 'bin2' using the weight 'weight'.

    // check for histogram
    if (!fHist || !fHist[bin1][bin2])
        return;

    // check histogram type
    Int_t dim = fHist[bin1][bin2]->GetDimension();
    if (dim == 1)
    {
        if (!(fVarX->TestBits(FAVarAbs::kNoFill)))
            fHist[bin1][bin2]->Fill(fVarX->AsDouble(),
                                    weight * fVarX->GetWeight());
    }
    else if (dim == 2)
    {
        if (!(fVarX->TestBits(FAVarAbs::kNoFill)) &&
            !(fVarY->TestBits(FAVarAbs::kNoFill)))
            ((TH2*)fHist[bin1][bin2])->Fill(fVarX->AsDouble(), fVarY->AsDouble(),
                                            weight * fVarX->GetWeight() * fVarY->GetWeight());
    }
    else if (dim == 3)
    {
        if (!(fVarX->TestBits(FAVarAbs::kNoFill)) &&
            !(fVarY->TestBits(FAVarAbs::kNoFill)) &&
            !(fVarZ->TestBits(FAVarAbs::kNoFill)))
         ((TH3*)fHist[bin1][bin2])->Fill(fVarX->AsDouble(), fVarY->AsDouble(), fVarZ->AsDouble(),
                                         weight * fVarX->GetWeight() * fVarY->GetWeight() * fVarZ->GetWeight());
    }
}

//______________________________________________________________________________
void FAVarHistogram::Fill(Double_t weight, Double_t part_weight, Int_t bin1, Int_t bin2)
{
    // Fill the histogram in 'bin1', 'bin2' using the weight 'weight' and the
    // partial (splitting) weight 'part_weight'.

    // check for histogram
    if (!fHist || !fHist[bin1][bin2])
        return;

    // shortcut
    TH1* h = fHist[bin1][bin2];

    // check histogram type
    Int_t dim = h->GetDimension();
    if (dim == 1)
    {
        if (!(fVarX->TestBits(FAVarAbs::kNoFill)))
        {
            Int_t gbin = h->GetXaxis()->FindFixBin(fVarX->AsDouble());
            Double_t fill = weight * fVarX->GetWeight();
            h->SetBinContent(gbin, h->GetBinContent(gbin) + fill * part_weight);
            if (h->GetSumw2()->fN)
                h->GetSumw2()->fArray[gbin] += fill * fill * part_weight;
        }
    }
    else if (dim == 2)
    {
        if (!(fVarX->TestBits(FAVarAbs::kNoFill)) &&
            !(fVarY->TestBits(FAVarAbs::kNoFill)))
        {
            Int_t gbin = h->GetBin(h->GetXaxis()->FindFixBin(fVarX->AsDouble()),
                                   h->GetYaxis()->FindFixBin(fVarY->AsDouble()));
            Double_t fill = weight * fVarX->GetWeight() * fVarY->GetWeight();
            h->SetBinContent(gbin, h->GetBinContent(gbin) + fill * part_weight);
            if (h->GetSumw2()->fN)
                h->GetSumw2()->fArray[gbin] += fill * fill * part_weight;
        }
    }
    else if (dim == 3)
    {
        if (!(fVarX->TestBits(FAVarAbs::kNoFill)) &&
            !(fVarY->TestBits(FAVarAbs::kNoFill)) &&
            !(fVarZ->TestBits(FAVarAbs::kNoFill)))
        {
            Int_t gbin = h->GetBin(h->GetXaxis()->FindFixBin(fVarX->AsDouble()),
                                   h->GetYaxis()->FindFixBin(fVarY->AsDouble()),
                                   h->GetYaxis()->FindFixBin(fVarZ->AsDouble()));
            Double_t fill = weight * fVarX->GetWeight() * fVarY->GetWeight() * fVarZ->GetWeight();
            h->SetBinContent(gbin, h->GetBinContent(gbin) + fill * part_weight);
            if (h->GetSumw2()->fN)
                h->GetSumw2()->fArray[gbin] += fill * fill * part_weight;
        }
    }
}

//______________________________________________________________________________
void FAVarHistogram::AddHistogramsToList(TList* list)
{
    // Add all histograms to the list 'list'.

    // check for histograms
    if (!fHist)
        return;

    // number of bins
    Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
    Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;

    // loop over histograms
    for (Int_t i = 0; i < nb1; i++)
        for (Int_t j = 0; j < nb2; j++)
            if (fHist[i][j]) list->Add(fHist[i][j]);
}

//______________________________________________________________________________
void FAVarHistogram::WriteToFile(TFile* fout)
{
    // Write the histograms to the file 'fout'.

    // check for histograms
    if (!fHist)
        return;

    // number of bins
    Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
    Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;

    // loop over histograms
    for (Int_t i = 0; i < nb1; i++)
    {
        for (Int_t j = 0; j < nb2; j++)
        {
            // create directory for bin
            if (!fout->GetDirectory(TString::Format("bin_%d_%d", i, j).Data()))
                fout->mkdir(TString::Format("bin_%d_%d", i, j).Data());
            fout->cd(TString::Format("bin_%d_%d", i, j).Data());
            if (fHist[i][j]) fHist[i][j]->Write();
        }
    }
}

