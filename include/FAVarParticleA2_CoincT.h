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


#ifndef FooAna_FAVarParticleA2_CoincT
#define FooAna_FAVarParticleA2_CoincT

#include "FAVarList.h"
#include "FAVar.h"

class FAVarParticleA2_CoincT : public FAVarList
{

protected:
    FAVar<Float_t>* fVar_dT_CB_CB;     // CB-CB coincidence time
    FAVar<Float_t>* fVar_dT_CB_TAPS;   // CB-TAPS coincidence time
    FAVar<Float_t>* fVar_dT_TAPS_TAPS; // TAPS-TAPS coincidence time

public:
    FAVarParticleA2_CoincT() : FAVarList(),
                               fVar_dT_CB_CB(0), fVar_dT_CB_TAPS(0),
                               fVar_dT_TAPS_TAPS(0) { }
    FAVarParticleA2_CoincT(const Char_t* name, const Char_t* title);
    virtual ~FAVarParticleA2_CoincT();

    void AddVars(Int_t nbins = 400, Double_t min = -40, Double_t max = 40,
                 UInt_t statusBits = FAVarAbs::kNoUnbinned);

    template <class T>
    void Set(const T* p1, const T* p2, Bool_t isMC);

    ClassDef(FAVarParticleA2_CoincT, 1)  // coincidence time variables of A2 particle
};

#endif

