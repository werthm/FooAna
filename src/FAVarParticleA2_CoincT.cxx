/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarParticleA2_CoincT                                               //
//                                                                      //
// Class for coincidence time variables of A2 analysis particles.       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAVarParticleA2_CoincT.h"
#include "FAConfigA2.h"
#include "FAParticleA2_B.h"
#include "FAParticleA2_BF1.h"

ClassImp(FAVarParticleA2_CoincT)

//______________________________________________________________________________
FAVarParticleA2_CoincT::FAVarParticleA2_CoincT(const Char_t* name, const Char_t* title)
    : FAVarList(name, title)
{
    // Constructor.

    // init members
    fVar_dT_CB_CB = 0;
    fVar_dT_CB_TAPS = 0;
    fVar_dT_TAPS_TAPS = 0;
}

//______________________________________________________________________________
FAVarParticleA2_CoincT::~FAVarParticleA2_CoincT()
{
    // Destructor.

    if (fVar_dT_CB_CB)
        delete fVar_dT_CB_CB;
    if (fVar_dT_CB_TAPS)
        delete fVar_dT_CB_TAPS;
    if (fVar_dT_TAPS_TAPS)
        delete fVar_dT_TAPS_TAPS;
}

//______________________________________________________________________________
void FAVarParticleA2_CoincT::AddVars(Int_t nbins, Double_t min, Double_t max,
                                     UInt_t statusBits)
{
    // Add coincidence time variables.

    // create variables
    fVar_dT_CB_CB = new FAVar<Float_t>(TString::Format("%s_coinc_time_CB_CB", GetName()).Data(),
                                       TString::Format("%s coincidence time (CB-CB)", GetTitle()).Data(),
                                       "ns", nbins, min, max, statusBits);
    fVar_dT_CB_TAPS = new FAVar<Float_t>(TString::Format("%s_coinc_time_CB_TAPS", GetName()).Data(),
                                         TString::Format("%s coincidence time (CB-TAPS)", GetTitle()).Data(),
                                         "ns", nbins, min, max, statusBits);
    fVar_dT_TAPS_TAPS = new FAVar<Float_t>(TString::Format("%s_coinc_time_TAPS_TAPS", GetName()).Data(),
                                           TString::Format("%s coincidence time (TAPS-TAPS)", GetTitle()).Data(),
                                           "ns", nbins, min, max, statusBits);

    // register variables
    AddVariable(fVar_dT_CB_CB);
    AddVariable(fVar_dT_CB_TAPS);
    AddVariable(fVar_dT_TAPS_TAPS);
}

//______________________________________________________________________________
template <class T>
void FAVarParticleA2_CoincT::Set(const T* p1, const T* p2, Bool_t isMC)
{
    // Set the coincidence time variables of the particles 'p1' with respect to
    // 'p2'.
    // The flag 'isMC' is used to ensure removal of the trigger time for real data.

    // check variables
    if (!fVar_dT_CB_CB)
        return;

    // CB-CB
    if ((p1->det & FAConfigA2::kCB) && (p2->det & FAConfigA2::kCB))
    {
        fVar_dT_CB_CB->SetVar(p1->time - p2->time);
        fVar_dT_CB_CB->ResetBit(FAVarAbs::kNoFill);
        fVar_dT_CB_TAPS->SetBit(FAVarAbs::kNoFill);
        fVar_dT_TAPS_TAPS->SetBit(FAVarAbs::kNoFill);
    }
    // TAPS-TAPS
    else if ((p1->det & FAConfigA2::kTAPS) && (p2->det & FAConfigA2::kTAPS))
    {
        if (isMC)
            fVar_dT_TAPS_TAPS->SetVar(p1->time - p2->time);
        else
            fVar_dT_TAPS_TAPS->SetVar(p2->time - p1->time);
        fVar_dT_CB_CB->SetBit(FAVarAbs::kNoFill);
        fVar_dT_CB_TAPS->SetBit(FAVarAbs::kNoFill);
        fVar_dT_TAPS_TAPS->ResetBit(FAVarAbs::kNoFill);
    }
    // CB-TAPS
    else if ((p1->det & FAConfigA2::kCB) && (p2->det & FAConfigA2::kTAPS))
    {
        if (isMC)
            fVar_dT_CB_TAPS->SetVar(p1->time - p2->time);
        else
            fVar_dT_CB_TAPS->SetVar(p1->time + p2->time);
        fVar_dT_CB_CB->SetBit(FAVarAbs::kNoFill);
        fVar_dT_CB_TAPS->ResetBit(FAVarAbs::kNoFill);
        fVar_dT_TAPS_TAPS->SetBit(FAVarAbs::kNoFill);
    }
    // TAPS-CB
    else if ((p1->det & FAConfigA2::kTAPS) && (p2->det & FAConfigA2::kCB))
    {
        if (isMC)
            fVar_dT_CB_TAPS->SetVar(p1->time - p2->time);
        else
            fVar_dT_CB_TAPS->SetVar(-(p1->time + p2->time));
        fVar_dT_CB_CB->SetBit(FAVarAbs::kNoFill);
        fVar_dT_CB_TAPS->ResetBit(FAVarAbs::kNoFill);
        fVar_dT_TAPS_TAPS->SetBit(FAVarAbs::kNoFill);
    }
}

// template instantiations
template void FAVarParticleA2_CoincT::Set(const FAParticleA2_B*, const FAParticleA2_B*, Bool_t);
template void FAVarParticleA2_CoincT::Set(const FAParticleA2_BF1*, const FAParticleA2_BF1*, Bool_t);

