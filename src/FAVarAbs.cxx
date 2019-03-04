/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarAbs                                                             //
//                                                                      //
// Abstract base class for analysis variables.                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TH1.h"

#include "FAVarAbs.h"
#include "FAVarList.h"

ClassImp(FAVarAbs)

//______________________________________________________________________________
FAVarAbs::FAVarAbs(const Char_t* name, const Char_t* title, const Char_t* unit,
                   Int_t nbins, Double_t min, Double_t max,
                   UInt_t statusBits)
    : TNamed(name, title)
{
    // Constructor.

    // init members
    if (unit)
        fUnit = unit;
    if (nbins == 0 && min == 0 && max == 0)
        fBins = 0;
    else
        fBins = new TAxis(nbins, min, max);
    fWeight = 1;
    SetBit(statusBits);
    fRelVar = new FAVarList(TString::Format("rel_var_%s", GetName()).Data(),
                            TString::Format("Related variables of %s", GetTitle()).Data());
}

//______________________________________________________________________________
FAVarAbs::FAVarAbs(const Char_t* name, const Char_t* title, const Char_t* unit,
                   TAxis* binning,
                   UInt_t statusBits)
    : TNamed(name, title)
{
    // Constructor.

    // init members
    fUnit = unit;
    fBins = new TAxis(*binning);
    fWeight = 1;
    SetBit(statusBits);
    fRelVar = new FAVarList(TString::Format("rel_var_%s", GetName()).Data(),
                            TString::Format("Related variables of %s", GetTitle()).Data());
}

//______________________________________________________________________________
FAVarAbs::FAVarAbs(FAVarFiller& filler, const Char_t* name, const Char_t* title,
                   const Char_t* unit,
                   Int_t nbins, Double_t min, Double_t max,
                   UInt_t statusBits)
    : TNamed(name, title)
{
    // Constructor.

    // init members
    if (unit)
        fUnit = unit;
    if (nbins == 0 && min == 0 && max == 0)
        fBins = 0;
    else
        fBins = new TAxis(nbins, min, max);
    fWeight = 1;
    SetBit(statusBits);
    fRelVar = new FAVarList(TString::Format("rel_var_%s", GetName()).Data(),
                            TString::Format("Related variables of %s", GetTitle()).Data());

    // add variable to filler
    filler.AddVariable(this);
}

//______________________________________________________________________________
FAVarAbs::FAVarAbs(FAVarFiller& filler, const Char_t* name, const Char_t* title,
                   const Char_t* unit,
                   TAxis* binning,
                   UInt_t statusBits)
    : TNamed(name, title)
{
    // Constructor.

    // init members
    fUnit = unit;
    fBins = new TAxis(*binning);
    fWeight = 1;
    SetBit(statusBits);
    fRelVar = new FAVarList(TString::Format("rel_var_%s", GetName()).Data(),
                            TString::Format("Related variables of %s", GetTitle()).Data());

    // add variable to filler
    filler.AddVariable(this);
}

//______________________________________________________________________________
FAVarAbs::~FAVarAbs()
{
    // Destructor.

    if (fBins)
        delete fBins;
    if (fRelVar)
        delete fRelVar;
}

//______________________________________________________________________________
void FAVarAbs::AddRelatedVariable(FAVarAbs* var)
{
    // Add the variable 'var' to the list of related variables.

    fRelVar->AddVariable(var);
}

