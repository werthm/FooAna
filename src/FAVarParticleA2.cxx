/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarParticleA2                                                      //
//                                                                      //
// Class for variables of A2 analysis particles.                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAVarParticleA2.h"
#include "FAConfigA2.h"
#include "FAParticleA2_B.h"
#include "FAParticleA2_BF1.h"

ClassImp(FAVarParticleA2)

//______________________________________________________________________________
FAVarParticleA2::FAVarParticleA2(const Char_t* name, const Char_t* title)
    : FAVarList(name, title)
{
    // Constructor.

    // init members
    fVarEnergy = 0;
    fVarEnergyOrig = 0;
    fVarTheta = 0;
    fVarCB_dE = 0;
    fVarTAPS_dE = 0;
    fVarCB_TOF = 0;
    fVarTAPS_TOF = 0;
    fVarPSA_A = 0;
    fVarPSA_R = 0;
    fVarPullTheta = 0;
    fVarPullPhi= 0;
    fVarPullT = 0;
}

//______________________________________________________________________________
FAVarParticleA2::~FAVarParticleA2()
{
    // Destructor.

    if (fVarEnergy) delete fVarEnergy;
    if (fVarEnergyOrig) delete fVarEnergyOrig;
    if (fVarTheta) delete fVarTheta;
    if (fVarCB_dE) delete fVarCB_dE;
    if (fVarTAPS_dE) delete fVarTAPS_dE;
    if (fVarCB_TOF) delete fVarCB_TOF;
    if (fVarTAPS_TOF) delete fVarTAPS_TOF;
    if (fVarPSA_A) delete fVarPSA_A;
    if (fVarPSA_R) delete fVarPSA_R;
    if (fVarPullTheta) delete fVarPullTheta;
    if (fVarPullPhi) delete fVarPullPhi;
    if (fVarPullT) delete fVarPullT;
}

//______________________________________________________________________________
void FAVarParticleA2::AddVarsKinematics(Int_t nbinsE, Double_t minE, Double_t maxE,
                                        Int_t nbinsth, Double_t minth, Double_t maxth,
                                        UInt_t statusBits)
{
    // Add the kinematics variables.

    // create variables
    Bool_t addEnergy = kFALSE;
    if (!fVarEnergy)
    {
        fVarEnergy = new FAVar<Float_t>(TString::Format("%s_Energy", GetName()).Data(),
                                        TString::Format("energy of %s", GetTitle()).Data(),
                                        "MeV",
                                        nbinsE, minE, maxE, statusBits);
        addEnergy = kTRUE;
    }
    fVarTheta = new FAVar<Float_t>(TString::Format("%s_Theta", GetName()).Data(),
                                   TString::Format("Polar angle of %s", GetTitle()).Data(),
                                   "deg",
                                   nbinsth, minth, maxth, statusBits);

    // no individual 1d-histograms of variables
    fVarEnergy->SetBit(FAVarAbs::kNoBinned);
    fVarTheta->SetBit(FAVarAbs::kNoBinned);

    // set up related variable
    fVarEnergy->AddRelatedVariable(fVarTheta);

    // register variables
    if (addEnergy)
        AddVariable(fVarEnergy);
    AddVariable(fVarTheta);
}

//______________________________________________________________________________
void FAVarParticleA2::AddVarsDeltaEE(Int_t nbinsE, Double_t minE, Double_t maxE,
                                     Int_t nbinsdE, Double_t mindE, Double_t maxdE,
                                     UInt_t statusBits)
{
    // Add the dE-E variables.

    // create variables
    Bool_t addEnergy = kFALSE;
    if (!fVarEnergy)
    {
        fVarEnergy = new FAVar<Float_t>(TString::Format("%s_Energy", GetName()).Data(),
                                        TString::Format("energy of %s", GetTitle()).Data(),
                                        "MeV",
                                        nbinsE, minE, maxE, statusBits);
        addEnergy = kTRUE;
    }
    fVarCB_dE = new FAVar<Float_t>(TString::Format("%s_CB_dE", GetName()).Data(),
                                   TString::Format("PID energy of %s", GetTitle()).Data(),
                                   "MeV",
                                      nbinsdE, mindE, maxdE, statusBits);
    fVarTAPS_dE = new FAVar<Float_t>(TString::Format("%s_TAPS_dE", GetName()).Data(),
                                     TString::Format("Veto energy of %s", GetTitle()).Data(),
                                     "MeV",
                                     nbinsdE, mindE, maxdE, statusBits);

    // no individual 1d-histograms of variables
    fVarEnergy->SetBit(FAVarAbs::kNoBinned);
    fVarCB_dE->SetBit(FAVarAbs::kNoBinned);
    fVarTAPS_dE->SetBit(FAVarAbs::kNoBinned);

    // set up related variable
    fVarEnergy->AddRelatedVariable(fVarCB_dE);
    fVarEnergy->AddRelatedVariable(fVarTAPS_dE);

    // register variables
    if (addEnergy)
        AddVariable(fVarEnergy);
    AddVariable(fVarCB_dE);
    AddVariable(fVarTAPS_dE);
}

//______________________________________________________________________________
void FAVarParticleA2::AddVarsTOF(Int_t nbinsTOF, Double_t minTOF, Double_t maxTOF,
                                 Int_t nbinsE, Double_t minE, Double_t maxE,
                                 UInt_t statusBits)
{
    // Add the TOF variables.

    // create variables
    Bool_t addEnergyOrig = kFALSE;
    if (!fVarEnergyOrig)
    {
        fVarEnergyOrig = new FAVar<Float_t>(TString::Format("%s_EnergyOrig", GetName()).Data(),
                                            TString::Format("original energy of %s", GetTitle()).Data(),
                                            "MeV",
                                            nbinsE, minE, maxE, statusBits);
        addEnergyOrig = kTRUE;
    }
    fVarCB_TOF = new FAVar<Float_t>(TString::Format("%s_CB_TOF", GetName()).Data(),
                                    TString::Format("CB TOF of %s", GetTitle()).Data(),
                                    "ns",
                                    nbinsTOF, minTOF, maxTOF, statusBits);
    fVarTAPS_TOF = new FAVar<Float_t>(TString::Format("%s_TAPS_TOF", GetName()).Data(),
                                      TString::Format("TAPS TOF of %s", GetTitle()).Data(),
                                      "ns",
                                      nbinsTOF, minTOF, maxTOF, statusBits);

    // no individual 1d-histograms of variables
    fVarEnergyOrig->SetBit(FAVarAbs::kNoBinned);
    fVarCB_TOF->SetBit(FAVarAbs::kNoBinned);
    fVarTAPS_TOF->SetBit(FAVarAbs::kNoBinned);

    // set up related variable
    fVarCB_TOF->AddRelatedVariable(fVarEnergyOrig);
    fVarTAPS_TOF->AddRelatedVariable(fVarEnergyOrig);

    // register variables
    if (addEnergyOrig)
        AddVariable(fVarEnergyOrig);
    AddVariable(fVarCB_TOF);
    AddVariable(fVarTAPS_TOF);
}

//______________________________________________________________________________
void FAVarParticleA2::AddVarsPSA(Int_t nbinsA, Double_t minA, Double_t maxA,
                                 Int_t nbinsR, Double_t minR, Double_t maxR,
                                 UInt_t statusBits)
{
    // Add the TAPS PSA variables.

    // create variables
    fVarPSA_A = new FAVar<Float_t>(TString::Format("%s_PSA_A", GetName()).Data(),
                                   TString::Format("PSA angle of %s", GetTitle()).Data(),
                                   "deg",
                                   nbinsA, minA, maxA, statusBits);
    fVarPSA_R = new FAVar<Float_t>(TString::Format("%s_PSA_R", GetName()).Data(),
                                   TString::Format("PSA radius of %s", GetTitle()).Data(),
                                   "MeV",
                                   nbinsR, minR, maxR, statusBits);

    // no individual 1d-histograms of variables
    fVarPSA_A->SetBit(FAVarAbs::kNoBinned);
    fVarPSA_R->SetBit(FAVarAbs::kNoBinned);

    // set up related variable
    fVarPSA_A->AddRelatedVariable(fVarPSA_R);

    // register variables
    AddVariable(fVarPSA_A);
    AddVariable(fVarPSA_R);
}

//______________________________________________________________________________
void FAVarParticleA2::AddVarsPull(Int_t nbins, Double_t min, Double_t max,
                                  UInt_t statusBits)
{
    // Add kinfit pull variables.

    // create variables
    fVarPullTheta = new FAVar<Float_t>(TString::Format("%s_pull_theta", GetName()).Data(),
                                       TString::Format("Pull of #theta_{%s}", GetTitle()).Data(),
                                       0,
                                       nbins, min, max, statusBits);
    fVarPullPhi = new FAVar<Float_t>(TString::Format("%s_pull_phi", GetName()).Data(),
                                     TString::Format("Pull of #phi_{%s}", GetTitle()).Data(),
                                     0,
                                     nbins, min, max, statusBits);
    fVarPullT = new FAVar<Float_t>(TString::Format("%s_pull_T", GetName()).Data(),
                                   TString::Format("Pull of T_{%s}", GetTitle()).Data(),
                                   0,
                                   nbins, min, max, statusBits);

    // register variables
    AddVariable(fVarPullTheta);
    AddVariable(fVarPullPhi);
    AddVariable(fVarPullT);
}

//______________________________________________________________________________
template <class T>
void FAVarParticleA2::Set(const T* part)
{
    // Set the general analysis variables using the particle 'part'.

    // check kinematics filling
    if (fVarTheta)
    {
        fVarEnergy->SetVar(part->energy);
        fVarTheta->SetVar(part->theta * TMath::RadToDeg());
    }

    // check dE-E filling
    if (fVarCB_dE)
    {
        fVarEnergy->SetVar(part->energy);
        fVarCB_dE->SetVar(part->deltaE);
        fVarTAPS_dE->SetVar(part->deltaE);

        // check the detector and set the no-fill bits accordingly
        if (part->det & FAConfigA2::kTAPS)
        {
            fVarCB_dE->SetBit(FAVarAbs::kNoFill);
            fVarTAPS_dE->ResetBit(FAVarAbs::kNoFill);
        }
        else if (part->det & FAConfigA2::kCB)
        {
            fVarCB_dE->ResetBit(FAVarAbs::kNoFill);
            fVarTAPS_dE->SetBit(FAVarAbs::kNoFill);
        }
        else
        {
            fVarCB_dE->SetBit(FAVarAbs::kNoFill);
            fVarTAPS_dE->SetBit(FAVarAbs::kNoFill);
        }
    }

    // check TOF filling
    if (fVarCB_TOF)
    {
        fVarEnergyOrig->SetVar(part->energyOrig);
        fVarCB_TOF->SetVar(part->tof);
        fVarTAPS_TOF->SetVar(part->tof);

        // check the detector and set the no-fill bits accordingly
        if (part->det & FAConfigA2::kTAPS)
        {
            fVarCB_TOF->SetBit(FAVarAbs::kNoFill);
            fVarTAPS_TOF->ResetBit(FAVarAbs::kNoFill);
        }
        else if (part->det & FAConfigA2::kCB)
        {
            fVarCB_TOF->ResetBit(FAVarAbs::kNoFill);
            fVarTAPS_TOF->SetBit(FAVarAbs::kNoFill);
        }
        else
        {
            fVarCB_TOF->SetBit(FAVarAbs::kNoFill);
            fVarTAPS_TOF->SetBit(FAVarAbs::kNoFill);
        }
    }

    // check PSA filling
    if (fVarPSA_A)
    {
        fVarPSA_A->SetVar(part->psa_a);
        fVarPSA_R->SetVar(part->psa_r);

        // check the detector and set the no-fill bits for CB particles
        if (part->det & FAConfigA2::kTAPS)
        {
            fVarPSA_A->ResetBit(FAVarAbs::kNoFill);
            fVarPSA_R->ResetBit(FAVarAbs::kNoFill);
        }
        else
        {
            fVarPSA_A->SetBit(FAVarAbs::kNoFill);
            fVarPSA_R->SetBit(FAVarAbs::kNoFill);
        }
    }
}

//______________________________________________________________________________
template <class T>
void FAVarParticleA2::SetKF(const T* part)
{
    // Set the general analysis variables using the particle 'part'.

    // check kinfit pull filling
    if (fVarPullTheta)
    {
        fVarPullTheta->SetVar(part->pullTheta);
        fVarPullPhi->SetVar(part->pullPhi);
        fVarPullT->SetVar(part->pullT);
    }
}

// template instantiations
template void FAVarParticleA2::Set(const FAParticleA2_B*);
template void FAVarParticleA2::Set(const FAParticleA2_BF1*);
template void FAVarParticleA2::SetKF(const FAParticleA2_BF1*);

