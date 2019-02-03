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


#ifndef FooAna_FAVarParticleA2KF
#define FooAna_FAVarParticleA2KF

#include "FAVarParticleA2.h"

class FAParticleA2KF;

class FAVarParticleA2KF : public FAVarParticleA2
{

protected:
    FAVarAbs* fVarPullTheta;                 // theta pull
    FAVarAbs* fVarPullPhi;                   // phi pull
    FAVarAbs* fVarPullT;                     // kinetic energy pull

public:
    FAVarParticleA2KF() : FAVarParticleA2(),
                          fVarPullTheta(0), fVarPullPhi(0), fVarPullT(0) { }
    FAVarParticleA2KF(const Char_t* name, const Char_t* title);
    virtual ~FAVarParticleA2KF();

    void AddVarsPull(Int_t nbins = 100, Double_t min = -5, Double_t max = 5,
                     UInt_t statusBits = FAVarAbs::kNoUnbinned);

    FAVarParticleA2KF& operator=(const FAParticleA2KF& part);

    ClassDef(FAVarParticleA2KF, 1)  // analysis variables of A2 particle (kinfit)
};

#endif

