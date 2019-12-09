/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarParticleCLAS12                                                  //
//                                                                      //
// Class for variables of CLAS12 analysis particles.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMath.h"

#include "FAVarParticleCLAS12.h"
#include "FAParticleCLAS12_B.h"

ClassImp(FAVarParticleCLAS12)

//______________________________________________________________________________
FAVarParticleCLAS12::FAVarParticleCLAS12(const Char_t* name, const Char_t* title)
    : FAVarList(name, title)
{
    // Constructor.

    // init members
    fVarMom = 0;
    fVarTheta = 0;
    fVarPhi = 0;
    fVarBetaTOF = 0;
    fVarBetaPID = 0;
    fVarDeltaBeta = 0;
}

//______________________________________________________________________________
FAVarParticleCLAS12::~FAVarParticleCLAS12()
{
    // Destructor.

    if (fVarMom) delete fVarMom;
    if (fVarTheta) delete fVarTheta;
    if (fVarPhi) delete fVarPhi;
    if (fVarBetaTOF) delete fVarBetaTOF;
    if (fVarBetaPID) delete fVarBetaPID;
    if (fVarDeltaBeta) delete fVarDeltaBeta;
}

//______________________________________________________________________________
void FAVarParticleCLAS12::AddVarsKinematics(Int_t nbinsP, Double_t minP, Double_t maxP,
                                            Int_t nbinsth, Double_t minth, Double_t maxth,
                                            Int_t nbinsph, Double_t minph, Double_t maxph,
                                            UInt_t statusBits)
{
    // Add the kinematics variables.

    // create variables
    Bool_t addMom = kFALSE;
    if (!fVarMom)
    {
        fVarMom = new FAVar<Float_t>(TString::Format("%s_Momentum", GetName()).Data(),
                                     TString::Format("momentum of %s", GetTitle()).Data(),
                                     "GeV/c",
                                     nbinsP, minP, maxP, statusBits);
        addMom = kTRUE;
    }
    fVarTheta = new FAVar<Float_t>(TString::Format("%s_Theta", GetName()).Data(),
                                   TString::Format("Polar angle of %s", GetTitle()).Data(),
                                   "deg",
                                   nbinsth, minth, maxth, statusBits);
    fVarPhi = new FAVar<Float_t>(TString::Format("%s_Phi", GetName()).Data(),
                                 TString::Format("Azimuthal angle of %s", GetTitle()).Data(),
                                 "deg",
                                 nbinsph, minph, maxph, statusBits);

    // no individual 1d-histograms of variables
    fVarMom->SetBit(FAVarAbs::kNoBinned);
    fVarTheta->SetBit(FAVarAbs::kNoBinned);
    fVarPhi->SetBit(FAVarAbs::kNoBinned);

    // set up related variable
    fVarMom->AddRelatedVariable(fVarTheta);
    fVarTheta->AddRelatedVariable(fVarPhi);

    // register variables
    if (addMom)
        AddVariable(fVarMom);
    AddVariable(fVarTheta);
    AddVariable(fVarPhi);
}

//______________________________________________________________________________
void FAVarParticleCLAS12::AddVarsPID(Int_t nbinsP, Double_t minP, Double_t maxP,
                                     Int_t nbinsBeta, Double_t minBeta, Double_t maxBeta,
                                     Int_t nbinsdeltaBeta, Double_t mindeltaBeta, Double_t maxdeltaBeta,
                                     UInt_t statusBits)
{
    // Add the PID variables.

    // create variables
    Bool_t addMom = kFALSE;
    if (!fVarMom)
    {
        fVarMom = new FAVar<Float_t>(TString::Format("%s_Momentum", GetName()).Data(),
                                     TString::Format("momentum of %s", GetTitle()).Data(),
                                     "GeV/c",
                                     nbinsP, minP, maxP, statusBits);
        addMom = kTRUE;
    }
    fVarBetaTOF = new FAVar<Float_t>(TString::Format("%s_BetaTOF", GetName()).Data(),
                                   TString::Format("#beta_{TOF} of %s", GetTitle()).Data(),
                                   "",
                                   nbinsBeta, minBeta, maxBeta, statusBits);
    fVarBetaPID = new FAVar<Float_t>(TString::Format("%s_BetaPID", GetName()).Data(),
                                   TString::Format("#beta_{PID} of %s", GetTitle()).Data(),
                                   "",
                                   nbinsBeta, minBeta, maxBeta, statusBits);
    fVarDeltaBeta = new FAVar<Float_t>(TString::Format("%s_DeltaBeta", GetName()).Data(),
                                   TString::Format("#Delta#beta of %s", GetTitle()).Data(),
                                   "",
                                   nbinsdeltaBeta, mindeltaBeta, maxdeltaBeta, statusBits);

    // no individual 1d-histograms of variables
    fVarMom->SetBit(FAVarAbs::kNoBinned);
    fVarBetaTOF->SetBit(FAVarAbs::kNoBinned);
    fVarBetaPID->SetBit(FAVarAbs::kNoBinned);
    fVarDeltaBeta->SetBit(FAVarAbs::kNoBinned);

    // set up related variable
    fVarMom->AddRelatedVariable(fVarBetaTOF);
    fVarMom->AddRelatedVariable(fVarBetaPID);
    fVarMom->AddRelatedVariable(fVarDeltaBeta);

    // register variables
    if (addMom)
        AddVariable(fVarMom);
    AddVariable(fVarBetaTOF);
    AddVariable(fVarBetaPID);
    AddVariable(fVarDeltaBeta);
}

//______________________________________________________________________________
template <class T>
void FAVarParticleCLAS12::Set(const T& part)
{
    // Set the general analysis variables using the particle 'part'.

    // check kinematics filling
    if (fVarTheta)
    {
        fVarMom->SetVar(part.mom.R());
        fVarTheta->SetVar(part.mom.Theta() * TMath::RadToDeg());
        fVarPhi->SetVar(part.mom.Phi() * TMath::RadToDeg());
    }

    // check PID filling
    if (fVarBetaTOF)
    {
        fVarMom->SetVar(part.mom.R());
        fVarBetaTOF->SetVar(part.betaTOF);
        fVarBetaPID->SetVar(part.betaPID);
        fVarDeltaBeta->SetVar(part.betaTOF - part.betaPID);
    }
}

// template instantiations
template void FAVarParticleCLAS12::Set(const FAParticleCLAS12_B&);

