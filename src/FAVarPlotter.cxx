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


#include "TCanvas.h"
#include "TH2.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TFile.h"

#include "FAVarPlotter.h"

ClassImp(FAVarPlotter)

//______________________________________________________________________________
FAVarPlotter::FAVarPlotter(const Char_t* name,
                           const Char_t* binName1, const Char_t* binName2)
    : TNamed(name, name)
{
    // Constructor.

    // init members
    if (binName1)
        fBins1.fName = binName1;
    if (binName2)
        fBins2.fName = binName2;
    fHistList = new TList();
    fHistList->SetOwner(kTRUE);
    fNormMode = kNone;
}

//______________________________________________________________________________
FAVarPlotter::~FAVarPlotter()
{
    // Destructor.

    fEntries.clear();
    if (fHistList)
        delete fHistList;
}

//______________________________________________________________________________
Bool_t FAVarPlotter::DetermineBins(const Char_t* file)
{
    // Determine the number of primary and secondary bins using the file 'file'.
    // Return kTRUE on success, otherwise return kFALSE.

    // try to open the file
    TFile f(file);
    if (f.IsZombie())
    {
        Error("DetermineBins", "Could not open the file '%s'!", file);
        return kFALSE;
    }

    // loop over primary bins
    for (Int_t i = 0; i >= 0; i++)
    {
        TDirectoryFile* dir = (TDirectoryFile*)f.Get(TString::Format("bin_%d_0", i));
        if (!dir)
        {
            fBins1.fNBins = i;
            break;
        }
    }

    // loop over secondary bins
    for (Int_t i = 0; i >= 0; i++)
    {
        TDirectoryFile* dir = (TDirectoryFile*)f.Get(TString::Format("bin_0_%d", i));
        if (!dir)
        {
            fBins2.fNBins = i;
            break;
        }
    }

    // user info
    Info("DetermineBins", "Found %d x %d = %d bins",
         fBins1.fNBins, fBins2.fNBins, fBins1.fNBins*fBins2.fNBins);

    return kTRUE;
}

//______________________________________________________________________________
void FAVarPlotter::AddEntry(const Char_t* file, const Char_t* title, const Color_t color)
{
    // Add a plot entry from the file 'file' using the title 'title' and
    // the marker/line color 'color'.

    // determine the binning once
    if (fEntries.size() == 0)
    {
        if (!DetermineBins(file))
        {
            Error("AddEntry", "Could not determine the number of bins from the file '%s'!",
                  file);
            return;
        }
    }

    // check binning and specified bin names
    if (fBins1.fNBins > 1 && fBins1.fName == "")
        Warning("AddEntry", "More than 1 primary bins found but bin name was not specified!");
    if (fBins2.fNBins > 1 && fBins2.fName == "")
        Warning("AddEntry", "More than 1 secondary bins found but bin name was not specified!");

    // create plot entry
    FAVarPlotterEntry entry(title, fBins1.fNBins, fBins2.fNBins);

    // configure plot entry
    entry.SetMarkerColor(color);
    entry.SetLineColor(color);

    // load histograms for all bins
    TH1* h;
    for (Int_t i = 0; i < fBins1.fNBins; i++)
    {
        for (Int_t j = 0; j < fBins2.fNBins; j++)
        {
            // try to load histogram
            TString hName;
            if (fBins1.fNBins == 1 && fBins2.fNBins == 1)
                hName = TString::Format("bin_%d_%d/%s",
                                        i, j, GetName());
            else if (fBins1.fNBins > 1 && fBins2.fNBins == 1)
                hName = TString::Format("bin_%d_%d/%s_%s_%d",
                                        i, j, GetName(), fBins1.fName.Data(), i);
            else if (fBins1.fNBins > 1 && fBins2.fNBins > 1)
                hName = TString::Format("bin_%d_%d/%s_%s_%d_%s_%d",
                                        i, j, GetName(), fBins1.fName.Data(), i, fBins2.fName.Data(), j);
            TFile fin(file);
            fin.GetObject(hName.Data(), h);

            // set histogram
            if (h)
            {
                h->SetDirectory(0);
                entry.SetHistogram(i, j, h);
                fHistList->Add(h);
            }
            else
            {
                Error("AddEntry", "Could not load histogram '%s' from file '%s'!",
                      hName.Data(), file);
                return;
            }
        }
    }

    // add to plot entries
    fEntries.push_back(entry);
}

//______________________________________________________________________________
void FAVarPlotter::SetMarkerColor(Color_t c)
{
    // Set the marker color for all plot entries to 'c'.

    for (std::vector<FAVarPlotterEntry>::iterator it = fEntries.begin(); it != fEntries.end(); ++it)
        it->SetMarkerColor(c);
}

//______________________________________________________________________________
void FAVarPlotter::SetMarkerStyle(Style_t s)
{
    // Set the marker style for all plot entries to 's'.

    for (std::vector<FAVarPlotterEntry>::iterator it = fEntries.begin(); it != fEntries.end(); ++it)
        it->SetMarkerStyle(s);
}

//______________________________________________________________________________
void FAVarPlotter::SetMarkerSize(Size_t s)
{
    // Set the marker size for all plot entries to 's'.

    for (std::vector<FAVarPlotterEntry>::iterator it = fEntries.begin(); it != fEntries.end(); ++it)
        it->SetMarkerSize();
}

//______________________________________________________________________________
void FAVarPlotter::SetLineColor(Color_t c)
{
    // Set the line color for all plot entries to 'c'.

    for (std::vector<FAVarPlotterEntry>::iterator it = fEntries.begin(); it != fEntries.end(); ++it)
        it->SetLineColor(c);
}

//______________________________________________________________________________
void FAVarPlotter::SetLineStyle(Style_t s)
{
    // Set the line style for all plot entries to 's'.

    for (std::vector<FAVarPlotterEntry>::iterator it = fEntries.begin(); it != fEntries.end(); ++it)
        it->SetLineStyle(s);
}

//______________________________________________________________________________
void FAVarPlotter::SetLineWidth(Width_t w)
{
    // Set the line width for all plot entries to 'w'.

    for (std::vector<FAVarPlotterEntry>::iterator it = fEntries.begin(); it != fEntries.end(); ++it)
        it->SetLineWidth(w);
}

//______________________________________________________________________________
void FAVarPlotter::ConfigureAxis(TAxis* a, AxisConfig_t& c)
{
    // Configure the axis 'a' according to the configuration 'c'.

    if (c.fMin != 0 || c.fMax != 0)
        a->SetRangeUser(c.fMin, c.fMax);
}

//______________________________________________________________________________
void FAVarPlotter::ConfigurePad(TVirtualPad* p)
{
    // Configure the pad 'p'.

    p->SetLogx(fAxisConfigX.fIsLog);
    p->SetLogy(fAxisConfigY.fIsLog);
    p->SetLogz(fAxisConfigZ.fIsLog);
}

//______________________________________________________________________________
void FAVarPlotter::DrawBin1D(Int_t bin1, Int_t bin2, TVirtualPad* pad)
{
    // Draw the 1-dim. content of bin 'bin1','bin2' on the pad 'pad'.

    // create the legend
    TLegend* leg = new TLegend(fLegConfig.fX1, fLegConfig.fY1,
                               fLegConfig.fX2, fLegConfig.fY2);
    leg->SetTextSize(0.03);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);

    // configure the pad
    ConfigurePad(pad);

    // loop over histograms
    TH1* hFirst = 0;
    for (std::vector<FAVarPlotterEntry>::iterator it = fEntries.begin(); it != fEntries.end(); ++it)
    {
        // apply format
        it->FormatHistograms();

        // get histogram
        TH1* h = it->GetHistogram(bin1, bin2);

        // rebin
        if (fAxisConfigX.fRebin)
        {
            h->Rebin(fAxisConfigX.fRebin);

            // update title of y-axis
            if (it == fEntries.begin())
                h->GetYaxis()->SetTitle(TString::Format("%s #times %d (rebinned)",
                                        h->GetYaxis()->GetTitle(), fAxisConfigX.fRebin).Data());
        }

        // first entry
        if (it == fEntries.begin())
        {
            // keep a pointer to first histogram
            hFirst = h;

            // draw histogram
            h->Draw();

            // add to legend
            leg->AddEntry(h, it->GetTitle(), "lm");
        }
        // other entries
        else
        {
            // normalization
            if (fNormMode == kIntegral)
                h->Scale(hFirst->Integral() / h->Integral());
            else if (fNormMode == kMaximum)
                h->Scale(hFirst->GetMaximum() / h->GetMaximum());

            // draw histogram
            h->Draw("same");

            // add to legend
            if (fNormMode == kNone)
                leg->AddEntry(h, it->GetTitle(), "lm");
            else if (fNormMode == kIntegral)
                leg->AddEntry(h, TString::Format("%s (int. norm.)", it->GetTitle()).Data(), "lm");
            else if (fNormMode == kMaximum)
                leg->AddEntry(h, TString::Format("%s (max. norm.)", it->GetTitle()).Data(), "lm");
        }
    }

    // configure axes
    ConfigureAxis(hFirst->GetXaxis(), fAxisConfigX);
    ConfigureAxis(hFirst->GetYaxis(), fAxisConfigY);

    // draw legend
    leg->Draw();
}

//______________________________________________________________________________
void FAVarPlotter::DrawBin2D(Int_t bin1, Int_t bin2, TVirtualPad* pad)
{
    // Draw the 2-dim. content of bin 'bin1','bin2' on the pad 'pad'.

    // divide canvas
    ((TPad*)pad)->DivideSquare(fEntries.size(), 0.001, 0.001);

    // loop over histograms
    Int_t n = 1;
    for (std::vector<FAVarPlotterEntry>::iterator it = fEntries.begin(); it != fEntries.end(); ++it)
    {
        // apply format
        it->FormatHistograms();

        // get histogram
        TH1* h = it->GetHistogram(bin1, bin2);

        // rebin
        if (fAxisConfigX.fRebin)
            h->RebinX(fAxisConfigX.fRebin);
        if (fAxisConfigY.fRebin)
            ((TH2*)h)->RebinY(fAxisConfigY.fRebin);

        // set title
        h->SetTitle(it->GetTitle());

        // configure axes
        ConfigureAxis(h->GetXaxis(), fAxisConfigX);
        ConfigureAxis(h->GetYaxis(), fAxisConfigY);
        ConfigureAxis(h->GetZaxis(), fAxisConfigZ);

        // draw
        TVirtualPad* sub_pad = pad->cd(n);
        ConfigurePad(sub_pad);
        h->Draw("col");

        n++;
    }
}

//______________________________________________________________________________
TCanvas* FAVarPlotter::DrawBin(Int_t bin1, Int_t bin2,
                               Int_t cWidth, Int_t cHeight)
{
    // Draw the content of bin 'bin1','bin2' on a TCanvas with dimensions
    // 'cWidth' x 'cHeight'.

    // check entries
    if (!fEntries.size())
    {
        Error("DrawBin", "List of plot entries is empty!");
        return 0;
    }

    // create canvas
    TCanvas* c = new TCanvas(GetName(), GetName(), cWidth, cHeight);

    // draw entries
    Int_t dim = fEntries[0].GetHistogram(bin1, bin2)->GetDimension();
    if (dim == 1)
        DrawBin1D(bin1, bin2, c);
    else if (dim == 2)
        DrawBin2D(bin1, bin2, c);
    else
        Error("DrawBin", "Histogram dimension %d not yet supported!", dim);

    return c;
}

//______________________________________________________________________________
TCanvas* FAVarPlotter::DrawBins(Int_t bin1, Int_t cWidth, Int_t cHeight)
{
    // Draw all secondary bins of the primary bin 'bin1' on a TCanvas with dimensions
    // 'cWidth' x 'cHeight'.

    // check entries
    if (!fEntries.size())
    {
        Error("DrawBins", "List of plot entries is empty!");
        return 0;
    }

    // create canvas
    TCanvas* c = new TCanvas(GetName(), GetName(), cWidth, cHeight);
    c->DivideSquare(fBins2.fNBins, 0.001, 0.001);

    // get histogram dimension
    Int_t dim = fEntries[0].GetHistogram(bin1, 0)->GetDimension();

    // loop over secondary bins
    for (Int_t i = 0; i < fBins2.fNBins; i++)
    {
        // get current pad
        TVirtualPad* pad = c->cd(i+1);

        // draw entries
        if (dim == 1)
            DrawBin1D(bin1, i, pad);
        else if (dim == 2)
            DrawBin2D(bin1, i, pad);
        else
            Error("DrawBins", "Histogram dimension %d not yet supported!", dim);
    }

    return c;
}

//______________________________________________________________________________
TCanvas* FAVarPlotter::DrawBins(Int_t cWidth, Int_t cHeight)
{
    // Draw all primary bins on a TCanvas with dimensions 'cWidth' x 'cHeight'.

    // check entries
    if (!fEntries.size())
    {
        Error("DrawBins", "List of plot entries is empty!");
        return 0;
    }

    // create canvas
    TCanvas* c = new TCanvas(GetName(), GetName(), cWidth, cHeight);
    c->DivideSquare(fBins1.fNBins, 0.001, 0.001);

    // get histogram dimension
    Int_t dim = fEntries[0].GetHistogram(0, 0)->GetDimension();

    // loop over primary bins
    for (Int_t i = 0; i < fBins1.fNBins; i++)
    {
        // get current pad
        TVirtualPad* pad = c->cd(i+1);

        // draw entries
        if (dim == 1)
            DrawBin1D(i, 0, pad);
        else if (dim == 2)
            DrawBin2D(i, 0, pad);
        else
            Error("DrawBins", "Histogram dimension %d not yet supported!", dim);
    }

    return c;
}

