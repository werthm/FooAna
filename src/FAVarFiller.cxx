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

ClassImp(FAVarFiller)

//______________________________________________________________________________
FAVarFiller::FAVarFiller(const Char_t* name, const Char_t* title)
    : TNamed(name, title)
{
    // Constructor.

    // init members
    fBins1 = 0;
    fBins2 = 0;
    fNVar = 0;
    fVar = 0;
    fMode= kNone;
    fNHist = 0;
    fHist = 0;
    fTree = 0;
}

//______________________________________________________________________________
FAVarFiller::~FAVarFiller()
{
    // Destructor.

    if (fVar)
        delete [] fVar;
    if (fHist)
    {
        for (Int_t i = 0; i < fNHist; i++)
            delete fHist[i];
        delete [] fHist;
    }
    if (fTree)
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

    // prepare histograms for binned filling
    if (fMode == kBinned || fMode == kBoth)
    {
        for (Int_t i = 0; i < fNHist; i++)
        {
            fHist[i]->SetBins1(fBins1);
            fHist[i]->SetBins2(fBins2);
            fHist[i]->CreateHistograms();
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
                for (Int_t k = 0; k < fNVar; k++)
                {
                    if (!(fVar[k]->TestBits(FAVarAbs::kNoUnbinned)))
                        fTree[i][j]->Branch(fVar[k]->GetName(), fVar[k]->GetVarPtr(),
                                            TString::Format("%s/%c", fVar[k]->GetName(), fVar[k]->GetTypeChar()).Data());
                }
            }
        }
    }
}

//______________________________________________________________________________
void FAVarFiller::AddVariable(FAVarAbs* var)
{
    // Add the variable 'var'.

    // backup old list
    FAVarAbs** old = fVar;

    // create new list
    fVar = new FAVarAbs*[fNVar+1];
    for (Int_t i = 0; i < fNVar; i++) fVar[i] = old[i];

    // add new element
    fVar[fNVar] = var;
    fNVar++;

    // destroy old list
    if (old) delete [] old;

    // create and add 1d-histogram of variable
    if (!(var->TestBits(FAVarAbs::kNoBinned)))
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

    // backup old list
    FAVarHistogram** old = fHist;

    // create new list
    fHist = new FAVarHistogram*[fNHist+1];
    for (Int_t i = 0; i < fNHist; i++) fHist[i] = old[i];

    // add new element
    fHist[fNHist] = hist;
    fNHist++;

    // destroy old list
    if (old) delete [] old;
}

//______________________________________________________________________________
void FAVarFiller::AddHistogram1D(FAVarAbs* varX)
{
    // Add a 1d-histogram of the variable 'varX'.

    // create and add histogram
    FAVarHistogram* hist = new FAVarHistogram(varX);
    AddHistogram(hist);
}

//______________________________________________________________________________
void FAVarFiller::AddHistogram2D(FAVarAbs* varX, FAVarAbs* varY)
{
    // Add a 2d-histogram of the variables 'varX' and 'varY'.

    // create and add histogram
    FAVarHistogram* hist = new FAVarHistogram(varX, varY);
    AddHistogram(hist);
}

//______________________________________________________________________________
void FAVarFiller::AddHistogram3D(FAVarAbs* varX, FAVarAbs* varY, FAVarAbs* varZ)
{
    // Add a 3d-histogram of the variables 'varX', 'varY' and 'varZ'.

    // create and add histogram
    FAVarHistogram* hist = new FAVarHistogram(varX, varY, varZ);
    AddHistogram(hist);
}

//______________________________________________________________________________
void FAVarFiller::Fill(Double_t weight, Double_t axisVar1, Double_t axisVar2)
{
    // Fill all analysis variables using the weight 'weight' and the values of
    // 'axisVar1' and 'axisVar2' as variables for the first and second bin
    // axis variables, respectively.

    // check all variables for NaN
    FAVarAbs* badVar = 0;
    for (Int_t i = 0; i < fNVar; i++)
    {
        if (TMath::IsNaN(fVar[i]->AsDouble()))
        {
            badVar = fVar[i];
            break;
        }
    }
    if (badVar)
    {
        Error("Fill", "Variable '%s' is NaN, this event will not be filled!", badVar->GetName());
        return;
    }

    // find bin
    Int_t bin1 = 0;
    Int_t bin2 = 0;
    if (fBins1 && fBins2)
    {
        if (axisVar1 < fBins1->GetXmin() || axisVar1 >= fBins1->GetXmax())
            return;
        if (axisVar2 < fBins2->GetXmin() || axisVar2 >= fBins2->GetXmax())
            return;
        bin1 = fBins1->FindFixBin(axisVar1)-1;
        bin2 = fBins2->FindFixBin(axisVar2)-1;
    }
    else if (fBins1 && !fBins2)
    {
        if (axisVar1 < fBins1->GetXmin() || axisVar1 >= fBins1->GetXmax())
            return;
        bin1 = fBins1->FindFixBin(axisVar1)-1;
        bin2 = 0;
    }
    else if (!fBins1 && !fBins2)
    {
        bin1 = 0;
        bin2 = 0;
    }

    // binned filling
    if (fMode == kBinned || fMode == kBoth)
    {
        for (Int_t i = 0; i < fNHist; i++)
            fHist[i]->Fill(weight, bin1, bin2);
    }

    // unbinned filling
    if (fMode == kUnbinned || fMode == kBoth)
        fTree[bin1][bin2]->Fill();
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
        for (Int_t i = 0; i < fNHist; i++)
            fHist[i]->AddHistogramsToList(list);
    }
}

//______________________________________________________________________________
void FAVarFiller::WriteFile(const Char_t* filename)
{
    // Write the analysis variable data to the file 'filename'.

    // open the output file
    TFile* fout = new TFile(filename, "recreate");

    // number of bins
    Int_t nb1 = fBins1 ? fBins1->GetNbins() : 1;
    Int_t nb2 = fBins2 ? fBins2->GetNbins() : 1;

    // unbinned filling
    if (fMode == kUnbinned || fMode == kBoth)
    {
        for (Int_t i = 0; i < nb1; i++)
            for (Int_t j = 0; j < nb2; j++)
                fTree[i][j]->Write();
    }

    // binned filling
    if (fMode == kBinned || fMode == kBoth)
    {
        for (Int_t i = 0; i < fNHist; i++)
            fHist[i]->WriteToFile(fout);
    }

    // close the file
    delete fout;
}

