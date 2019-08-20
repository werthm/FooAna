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
#include "FAVar.h"

class FAVarParticleA2 : public FAVarList
{

protected:
    FAVar<Float_t>* fVarEnergy;     // energy
    FAVar<Float_t>* fVarEnergyOrig; // original energy
    FAVar<Float_t>* fVarTheta;      // polar angle
    FAVar<Float_t>* fVarCB_dE;      // deltaE for CB
    FAVar<Float_t>* fVarTAPS_dE;    // deltaE for TAPS
    FAVar<Float_t>* fVarCB_TOF;     // TOF for CB
    FAVar<Float_t>* fVarTAPS_TOF;   // TOF for TAPS
    FAVar<Float_t>* fVarPSA_A;      // PSA angle
    FAVar<Float_t>* fVarPSA_R;      // PSA radius
    FAVar<Float_t>* fVarPullTheta;  // theta pull
    FAVar<Float_t>* fVarPullPhi;    // phi pull
    FAVar<Float_t>* fVarPullT;      // kinetic energy pull

public:
    FAVarParticleA2() : FAVarList(),
                        fVarEnergy(0),
                        fVarEnergyOrig(0),
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

    template <class T>
    void Set(const T* part);
    template <class T>
    void SetKF(const T* part);

    ClassDef(FAVarParticleA2, 1)  // analysis variables of A2 particle
};

#endif

