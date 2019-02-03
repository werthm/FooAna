/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarParticleA2KF                                                    //
//                                                                      //
// Class for variables of A2 analysis particles (kinfit).               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAVarParticleA2KF.h"
#include "FAVar.h"
#include "FAParticleA2KF.h"

ClassImp(FAVarParticleA2KF)

//______________________________________________________________________________
FAVarParticleA2KF::FAVarParticleA2KF(const Char_t* name, const Char_t* title)
    : FAVarParticleA2(name, title)
{
    // Constructor.

    // init members
    fVarPullTheta = 0;
    fVarPullPhi= 0;
    fVarPullT = 0;
}

//______________________________________________________________________________
FAVarParticleA2KF::~FAVarParticleA2KF()
{
    // Destructor.

    if (fVarPullTheta) delete fVarPullTheta;
    if (fVarPullPhi) delete fVarPullPhi;
    if (fVarPullT) delete fVarPullT;
}

//______________________________________________________________________________
void FAVarParticleA2KF::AddVarsPull(Int_t nbins, Double_t min, Double_t max,
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
FAVarParticleA2KF& FAVarParticleA2KF::operator=(const FAParticleA2KF& part)
{
    // Assignment operator.

    // call parent operator
    FAVarParticleA2::operator=(part);

    // check kinfit pull filling
    if (fVarPullTheta)
    {
        ((FAVar<Float_t>*)fVarPullTheta)->SetVar(part.pullTheta);
        ((FAVar<Float_t>*)fVarPullPhi)->SetVar(part.pullPhi);
        ((FAVar<Float_t>*)fVarPullT)->SetVar(part.pullT);
    }

    return *this;
}

