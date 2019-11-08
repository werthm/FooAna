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


#include "TTree.h"
#include "TAxis.h"
#include "TFile.h"
#include "TMath.h"

#include "FAVarFiller.h"
#include "FAVarAbs.h"
#include "FAVarHistogram.h"
#include "FAVarList.h"
#include "FAUtils.h"

ClassImp(FAVarFiller)

//______________________________________________________________________________
FAVarFiller::FAVarFiller(const Char_t* name, const Char_t* title, Bool_t doCleanup)
    : TNamed(name, title)
{
    // Constructor.
    // If 'doCleanup' is kFALSE, the output histograms/trees will not be deleted.

    // init members
    fBins1 = 0;
    fBins2 = 0;
    fVarWeight = 0;
    fMode= kNone;
    fEntries = 0;
    fIsCleanup = doCleanup;
    fTree = 0;
}

//______________________________________________________________________________
FAVarFiller::~FAVarFiller()
{
    // Destructor.

    if (fIsCleanup)
    {
        for (FAVarHistogram* h : fHist)
            delete h;
    }
    if (fTree && fIsCleanup)
    {
        Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
        Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;
        for (Int_t i = 0; i < nb1; i++)
        {
            for (Int_t j = 0; j < nb2; j++)
            {
                delete fTree[i][j];
            }
            delete [] fTree[i];
        }
        delete [] fTree;
    }
}

//______________________________________________________________________________
void FAVarFiller::Init(EFillMode mode)
{
    // Initialize by setting the filling mode to 'mode'.

    // set filling mode
    fMode = mode;
    fEntries = 0;

    // prepare histograms for binned filling
    if (fMode == kBinned || fMode == kBoth)
    {
        // remove histograms with disabled binned storage
        auto remove_no_binned = [](FAVarHistogram* hist)
        {
            // get associated variables
            FAVarAbs* varX = hist->GetVarX();
            FAVarAbs* varY = hist->GetVarY();
            FAVarAbs* varZ = hist->GetVarZ();

            // check binned storage
            if (varX && varY && varZ)
            {
                // do not disable any 3d-histogram
                return kFALSE;
            }
            else if (varX && varY)
            {
                // do not disable any 2d-histogram
                return kFALSE;
            }
            else if (varX)
            {
                if (varX->TestBits(FAVarAbs::kNoBinned))
                    return kTRUE;
            }

            return kFALSE;
        };

        // find histograms to be removed
        auto new_end = std::remove_if(fHist.begin(), fHist.end(), remove_no_binned);

        // remove histograms
        fHist.erase(new_end, fHist.end());

        // create histogram
        for (FAVarHistogram* h : fHist)
        {
            h->SetBins1(fBins1);
            h->SetBins2(fBins2);
            h->CreateHistograms();
        }
    }

    // prepare output tree for unbinned filling
    if (fMode == kUnbinned || fMode == kBoth)
    {
        // check axes
        Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
        Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;

        // create the trees
        fTree = new TTree**[nb1];

        // loop over bins of first axis
        for (Int_t i = 0; i < nb1; i++)
        {
            fTree[i] = new TTree*[nb2];

            // loop over bins of second axis
            for (Int_t j = 0; j < nb2; j++)
            {
                // create the tree
                if (fBins1 && fBins2)
                {
                    fTree[i][j] = new TTree(TString::Format("%s_%s_%d_%s_%d",
                                                            GetName(),
                                                            fBins1->GetName(), i,
                                                            fBins2->GetName(), j).Data(),
                                            TString::Format("%s, %s: [%.3f,%.3f], %s: [%.3f,%.3f]",
                                                            GetTitle(),
                                                            fBins1->GetTitle(), fBins1->GetBinLowEdge(i+1), fBins1->GetBinUpEdge(i+1),
                                                            fBins2->GetTitle(), fBins2->GetBinLowEdge(j+1), fBins2->GetBinUpEdge(j+1)).Data());
                }
                else if (fBins1 && !fBins2)
                {
                    fTree[i][j] = new TTree(TString::Format("%s_%s_%d",
                                                            GetName(),
                                                            fBins1->GetName(), i).Data(),
                                            TString::Format("%s, %s: [%.3f,%.3f]",
                                                            GetTitle(),
                                                            fBins1->GetTitle(), fBins1->GetBinLowEdge(i+1), fBins1->GetBinUpEdge(i+1)).Data());
                }
                else if (!fBins1 && !fBins2)
                {
                    fTree[i][j] = new TTree(GetName(), GetTitle());
                }
                else
                {
                    Error("Init", "Error in bin axis configuration!");
                    return;
                }

                // create branches
                for (FAVarAbs* v : fVar)
                {
                    if (!(v->TestBits(FAVarAbs::kNoUnbinned)))
                        fTree[i][j]->Branch(v->GetName(), v->GetVarPtr(),
                                            TString::Format("%s/%c", v->GetName(), v->GetTypeChar()).Data());
                }
            }
        }
    }
}

//______________________________________________________________________________
void FAVarFiller::AddVariable(FAVarAbs* var)
{
    // Add the variable 'var'.

    // add variable
    fVar.push_back(var);

    // create and add 1d-histogram of variable
    AddHistogram1D(var);

    // create and add 2d-histograms of related variables
    for (Int_t i = 0; i < var->GetRelatedVariables()->GetNVar(); i++)
        AddHistogram2D(var, var->GetRelatedVariables()->GetVariable(i));
}

//______________________________________________________________________________
void FAVarFiller::AddVariableList(FAVarList* list)
{
    // Add the variable list 'list'.

    // loop over variables
    for (Int_t i = 0; i < list->GetNVar(); i++)
        AddVariable(list->GetVariable(i));
}

//______________________________________________________________________________
void FAVarFiller::AddHistogram(FAVarHistogram* hist)
{
    // Add the histogram 'hist'.

    // add histogram
    fHist.push_back(hist);
}

//______________________________________________________________________________
void FAVarFiller::AddHistogram1D(FAVarAbs* varX, Bool_t sumw2)
{
    // Add a 1d-histogram of the variable 'varX'.
    // Activate storage of weights if 'sumw2' is kTRUE.

    // create and add histogram
    FAVarHistogram* hist = new FAVarHistogram(varX);
    hist->SetSumw2(sumw2);
    AddHistogram(hist);
}

//______________________________________________________________________________
void FAVarFiller::AddHistogram2D(FAVarAbs* varX, FAVarAbs* varY, Bool_t sumw2)
{
    // Add a 2d-histogram of the variables 'varX' and 'varY'.
    // Activate storage of weights if 'sumw2' is kTRUE.

    // create and add histogram
    FAVarHistogram* hist = new FAVarHistogram(varX, varY);
    hist->SetSumw2(sumw2);
    AddHistogram(hist);
}

//______________________________________________________________________________
void FAVarFiller::AddHistogram3D(FAVarAbs* varX, FAVarAbs* varY, FAVarAbs* varZ, Bool_t sumw2)
{
    // Add a 3d-histogram of the variables 'varX', 'varY' and 'varZ'.
    // Activate storage of weights if 'sumw2' is kTRUE.

    // create and add histogram
    FAVarHistogram* hist = new FAVarHistogram(varX, varY, varZ);
    hist->SetSumw2(sumw2);
    AddHistogram(hist);
}

//______________________________________________________________________________
Bool_t FAVarFiller::FindBin(Double_t axisVar1, Double_t axisVar2, Int_t& bin1, Int_t& bin2)
{
    // Find the bins 'bin1' and 'bin2' corresponding to the 'axisVar1' and
    // 'axisVar2' bin axis values.
    // Return kFALSE for under/overflow bins.

    // find bin
    bin1 = 0;
    bin2 = 0;
    if (fBins1 && fBins2)
    {
        if (axisVar1 < fBins1->GetXmin() || axisVar1 >= fBins1->GetXmax())
            return kFALSE;
        if (axisVar2 < fBins2->GetXmin() || axisVar2 >= fBins2->GetXmax())
            return kFALSE;
        bin1 = fBins1->FindFixBin(axisVar1)-1;
        bin2 = fBins2->FindFixBin(axisVar2)-1;
    }
    else if (fBins1 && !fBins2)
    {
        if (axisVar1 < fBins1->GetXmin() || axisVar1 >= fBins1->GetXmax())
            return kFALSE;
        bin1 = fBins1->FindFixBin(axisVar1)-1;
        bin2 = 0;
    }
    else if (!fBins1 && !fBins2)
    {
        bin1 = 0;
        bin2 = 0;
    }

    return kTRUE;
}

//______________________________________________________________________________
void FAVarFiller::Fill(Double_t weight, Double_t axisVar1, Double_t axisVar2)
{
    // Fill all analysis variables using the weight 'weight' and the values of
    // 'axisVar1' and 'axisVar2' as variables for the first and second bin
    // axis variables, respectively.

    Int_t bin1, bin2;
    if (FindBin(axisVar1, axisVar2, bin1, bin2))
        FillBin(weight, bin1, bin2);
}

//______________________________________________________________________________
void FAVarFiller::Fill(std::function<Double_t(void)> wFunc, Double_t axisVar1, Double_t axisVar2)
{
    // Fill all analysis variables using the weight provided by the function
    // 'wFunc', and the values of 'axisVar1' and 'axisVar2' as variables for the
    // first and second bin axis variables, respectively.
    // The weighting function will only be called for event that will be filled,
    // i.e. are within the range of the binning variables.
    // If the weighting function returns 0, the event will not be filled.

    Int_t bin1, bin2;

    // find the bin
    if (!FindBin(axisVar1, axisVar2, bin1, bin2))
        return;

    // call weighting function
    Double_t weight = wFunc();

    // fill bin
    if (weight != 0)
        FillBin(weight, bin1, bin2);
}

//______________________________________________________________________________
void FAVarFiller::FillBin(Double_t weight, Int_t bin1, Int_t bin2)
{
    // Fill all analysis variables using the weight 'weight' and 'bin1' and 'bin2'
    // as the first and second axis bins, respectively.

    // check all variables for NaN
    FAVarAbs* badVar = 0;
    for (FAVarAbs* v : fVar)
    {
        if (TMath::IsNaN(v->AsDouble()))
        {
            badVar = v;
            break;
        }
    }
    if (badVar)
    {
        Error("FillBin", "Variable '%s' is NaN, this event will not be filled!", badVar->GetName());
        return;
    }

    // binned filling
    if (fMode == kBinned || fMode == kBoth)
    {
        for (FAVarHistogram* h : fHist)
            h->Fill(weight, bin1, bin2);
    }

    // unbinned filling
    if (fMode == kUnbinned || fMode == kBoth)
        fTree[bin1][bin2]->Fill();

    // count entry
    fEntries++;
}

//______________________________________________________________________________
void FAVarFiller::FillOverlap(std::function<Double_t(void)> wFunc, Double_t axisVar1,
                              Double_t axisVarWidth1, Double_t axisVar2)
{
    // Fill all analysis variables using the weight provided by the function
    // 'wFunc', and the values of 'axisVar1' and 'axisVar2' as variables for the
    // first and second bin axis variables, respectively.
    // The weight will be split across several primary bins according to 'axisVar1'
    // and the width 'axisVarWidth1' associated with 'axisVar1'.
    // The weighting function will only be called for event that will be filled,
    // i.e. are within the range of the binning variables.
    // If the weighting function returns 0, the event will not be filled.
    // NOTE: This method leads to incorrect statistical errors for unbinned filling.

    // check all variables for NaN
    FAVarAbs* badVar = 0;
    for (FAVarAbs* v : fVar)
    {
        if (TMath::IsNaN(v->AsDouble()))
        {
            badVar = v;
            break;
        }
    }
    if (badVar)
    {
        Error("FillOverlap", "Variable '%s' is NaN, this event will not be filled!", badVar->GetName());
        return;
    }

    // check binning
    Int_t bin2 = 0;
    if (fBins1 && fBins2)
    {
        if (axisVar2 < fBins2->GetXmin() || axisVar2 >= fBins2->GetXmax())
            return;
        bin2 = fBins2->FindFixBin(axisVar2)-1;
    }
    else if (fBins1 && !fBins2)
    {
        bin2 = 0;
    }
    else if (!fBins1 && !fBins2)
    {
        Double_t weight = wFunc();
        if (weight != 0)
            FillBin(weight, 0, 0);
        return;
    }

    // exclude pure under-/overflow bins
    if (fBins1->FindFixBin(axisVar1 + 0.5*axisVarWidth1) == 0)
        return;
    if (fBins1->FindFixBin(axisVar1 - 0.5*axisVarWidth1) == fBins1->GetNbins()+1)
        return;

    // check for linked weight variable for unbinned filling
    if (!fVarWeight && (fMode == kUnbinned || fMode == kBoth))
    {
        Error("FillOverlap", "Pointer to weight variable was not set!");
        return;
    }

    // find bins and partial weights
    std::vector<std::pair<Int_t, Double_t>> bins;
    FAUtils::CalcBinOverlapWeights(fBins1, axisVar1, axisVarWidth1, bins);

    // leave if there are no bins to fill
    if (!bins.size())
        return;

    // call weighting function
    Double_t weight = wFunc();

    // skip filling with weight 0
    if (weight == 0)
        return;

    // save original weight
    Double_t orig_weight = 1;
    if (fVarWeight)
        orig_weight = fVarWeight->AsDouble();

    // fill all bins
    for (const std::pair<Int_t, Double_t>& p : bins)
    {
        // skip partial under- and overflow bins
        if (p.first == 0 || p.first == fBins1->GetNbins()+1)
            continue;

        // binned filling
        if (fMode == kBinned || fMode == kBoth)
        {
            for (FAVarHistogram* h : fHist)
                h->Fill(weight, p.second, p.first-1, bin2);
        }

        // unbinned filling
        if (fMode == kUnbinned || fMode == kBoth)
        {
            // modify weight
            fVarWeight->SetDouble(orig_weight * p.second);

            // fill tree
            fTree[p.first-1][bin2]->Fill();
        }
    }

    // count entry (as a whole)
    fEntries++;
}

//______________________________________________________________________________
void FAVarFiller::EnableFillBinned()
{
    // Enable binned filling for all analysis variables.

    for (FAVarAbs* v : fVar)
        v->EnableFillBinned();
}

//______________________________________________________________________________
void FAVarFiller::DisableFillBinned()
{
    // Disable binned filling for all analysis variables.

    for (FAVarAbs* v : fVar)
        v->DisableFillBinned();
}

//______________________________________________________________________________
void FAVarFiller::EnableStorageBinned()
{
    // Enable binned storage for all analysis variables.

    for (FAVarAbs* v : fVar)
        v->EnableStorageBinned();
}

//______________________________________________________________________________
void FAVarFiller::DisableStorageBinned()
{
    // Disable binned storage for all analysis variables.

    for (FAVarAbs* v : fVar)
        v->DisableStorageBinned();
}

//______________________________________________________________________________
void FAVarFiller::EnableStorageUnbinned()
{
    // Enable unbinned storage for all analysis variables.

    for (FAVarAbs* v : fVar)
        v->EnableStorageUnbinned();
}

//______________________________________________________________________________
void FAVarFiller::DisableStorageUnbinned()
{
    // Disable unbinned storage for all analysis variables.

    for (FAVarAbs* v : fVar)
        v->DisableStorageUnbinned();
}

//______________________________________________________________________________
void FAVarFiller::AddObjectsToList(TList* list)
{
    // Add all fill objects to the list 'list'.

    // number of bins
    Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
    Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;

    // unbinned filling
    if (fMode == kUnbinned || fMode == kBoth)
    {
        for (Int_t i = 0; i < nb1; i++)
            for (Int_t j = 0; j < nb2; j++)
                list->Add(fTree[i][j]);
    }

    // binned filling
    if (fMode == kBinned || fMode == kBoth)
    {
        for (FAVarHistogram* h : fHist)
            h->AddHistogramsToList(list);
    }
}

//______________________________________________________________________________
void FAVarFiller::WriteFile(const Char_t* filename, Bool_t flat)
{
    // Write the analysis variable data to the file 'filename'.
    // If 'flat' is kTRUE, a flat directory is structure is used.

    // open the output file
    TFile* fout = new TFile(filename, "recreate");

    // number of bins
    Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
    Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;

    // unbinned filling
    if (fMode == kUnbinned || fMode == kBoth)
    {
        for (Int_t i = 0; i < nb1; i++)
        {
            for (Int_t j = 0; j < nb2; j++)
            {
                // clone the tree
                TTree* t = (TTree*)fTree[i][j]->CloneTree();

                // write to file with compression
                t->Write();

                // clean-up
                delete t;
            }
        }
    }

    // binned filling
    if (fMode == kBinned || fMode == kBoth)
    {
        for (FAVarHistogram* h : fHist)
            h->WriteToFile(fout, flat);
    }

    // close the file
    delete fout;
}

