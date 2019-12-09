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


#ifndef FooAna_FAVarParticleCLAS12
#define FooAna_FAVarParticleCLAS12

#include "FAVarList.h"
#include "FAVar.h"

class FAVarParticleCLAS12 : public FAVarList
{

protected:
    FAVar<Float_t>* fVarMom;        // momentum
    FAVar<Float_t>* fVarTheta;      // polar angle
    FAVar<Float_t>* fVarPhi;        // azimuthal angle
    FAVar<Float_t>* fVarBetaTOF;    // beta TOF
    FAVar<Float_t>* fVarBetaPID;    // beta PID
    FAVar<Float_t>* fVarDeltaBeta;  // delta beta

public:
    FAVarParticleCLAS12() : FAVarList(),
                            fVarMom(0),
                            fVarTheta(0), fVarPhi(0),
                            fVarBetaTOF(0), fVarBetaPID(0), fVarDeltaBeta(0) { }
    FAVarParticleCLAS12(const Char_t* name, const Char_t* title);
    virtual ~FAVarParticleCLAS12();

    void AddVarsKinematics(Int_t nbinsP = 1000, Double_t minP = 0, Double_t maxP = 10,
                           Int_t nbinsth = 90, Double_t minth = 0, Double_t maxth = 180,
                           Int_t nbinsph = 180, Double_t minph = -180, Double_t maxph = 180,
                           UInt_t statusBits = FAVarAbs::kNoUnbinned);
    void AddVarsPID(Int_t nbinsP = 1000, Double_t minP = 0, Double_t maxP = 10,
                    Int_t nbinsBeta = 300, Double_t minBeta = 0, Double_t maxBeta = 1.5,
                    Int_t nbinsdeltaBeta = 400, Double_t mindeltaBeta = -0.5, Double_t maxdeltaBeta = 0.5,
                    UInt_t statusBits = FAVarAbs::kNoUnbinned);

    template <class T>
    void Set(const T& part);

    ClassDef(FAVarParticleCLAS12, 1)  // analysis variables of CLAS12 particle
};

#endif

