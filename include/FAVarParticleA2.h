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


#ifndef FooAna_FAVarParticleA2
#define FooAna_FAVarParticleA2

#include "FAVarList.h"
#include "FAVarAbs.h"

class FAParticleA2_B;
class FAParticleA2_BF1;

class FAVarParticleA2 : public FAVarList
{

protected:
    FAVarAbs* fVarEnergy;                    // energy
    FAVarAbs* fVarTheta;                     // polar angle
    FAVarAbs* fVarCB_dE;                     // deltaE for CB
    FAVarAbs* fVarTAPS_dE;                   // deltaE for TAPS
    FAVarAbs* fVarCB_TOF;                    // TOF for CB
    FAVarAbs* fVarTAPS_TOF;                  // TOF for TAPS
    FAVarAbs* fVarPSA_A;                     // PSA angle
    FAVarAbs* fVarPSA_R;                     // PSA radius
    FAVarAbs* fVarPullTheta;                 // theta pull
    FAVarAbs* fVarPullPhi;                   // phi pull
    FAVarAbs* fVarPullT;                     // kinetic energy pull

public:
    FAVarParticleA2() : FAVarList(),
                        fVarEnergy(0),
                        fVarTheta(0),
                        fVarCB_dE(0), fVarTAPS_dE(0),
                        fVarCB_TOF(0), fVarTAPS_TOF(0),
                        fVarPSA_A(0), fVarPSA_R(0),
                        fVarPullTheta(0), fVarPullPhi(0), fVarPullT(0) { }
    FAVarParticleA2(const Char_t* name, const Char_t* title);
    virtual ~FAVarParticleA2();

    void AddVarsKinematics(Int_t nbinsE = 1000, Double_t minE = 0, Double_t maxE = 1000,
                           Int_t nbinsth = 90, Double_t minth = 0, Double_t maxth = 180,
                           UInt_t statusBits = FAVarAbs::kNoUnbinned);
    void AddVarsDeltaEE(Int_t nbinsE = 1000, Double_t minE = 0, Double_t maxE = 1000,
                        Int_t nbinsdE = 400, Double_t mindE = 0, Double_t maxdE = 10,
                        UInt_t statusBits = FAVarAbs::kNoUnbinned);
    void AddVarsTOF(Int_t nbinsTOF = 300, Double_t minTOF = -10, Double_t maxTOF = 20,
                    Int_t nbinsE = 1000, Double_t minE = 0, Double_t maxE = 1000,
                    UInt_t statusBits = FAVarAbs::kNoUnbinned);
    void AddVarsPSA(Int_t nbinsA = 600, Double_t minA = 0, Double_t maxA = 60,
                    Int_t nbinsR = 600, Double_t minR = 0, Double_t maxR = 600,
                    UInt_t statusBits = FAVarAbs::kNoUnbinned);
    void AddVarsPull(Int_t nbins = 100, Double_t min = -5, Double_t max = 5,
                     UInt_t statusBits = FAVarAbs::kNoUnbinned);

    FAVarParticleA2& operator=(const FAParticleA2_B& part);
    FAVarParticleA2& operator=(const FAParticleA2_BF1& part);

    ClassDef(FAVarParticleA2, 1)  // analysis variables of A2 particle
};

#endif

