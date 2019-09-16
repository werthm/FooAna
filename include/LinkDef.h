/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// LinkDef.h                                                            //
//                                                                      //
// FooAna dictionary header file.                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "Math/GenVector/LorentzVector.h"

#ifdef __CINT__

// turn everything off
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all typedef;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;

#pragma link C++ enum FADetectorA2_t;
#pragma link C++ namespace FAUtils;
#pragma link C++ namespace FAUtilsA2;
#pragma link C++ namespace FAConfigA2;
#pragma link C++ class FAAnalysis+;
#pragma link C++ class FAAnalysisA2+;
#pragma link C++ class FAAnalysisResult+;
#pragma link C++ typedef FAVector4;
#pragma link C++ class std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<Double32_t>>>+;
#pragma link C++ class FAProgressServer+;
#pragma link C++ class FAProgressClient+;
#pragma link C++ class FAProgressMessage+;
#pragma link C++ class FAVarAbs+;
#pragma link C++ class FAVar<Short_t>+;
#pragma link C++ class FAVar<Int_t>+;
#pragma link C++ class FAVar<Long64_t>+;
#pragma link C++ class FAVar<Float_t>+;
#pragma link C++ class FAVar<Double_t>+;
#pragma link C++ class FAVarList+;
#pragma link C++ class FAVarFiller+;
#pragma link C++ class FAVarPlotter+;
#pragma link C++ class FAVarPlotterEntry+;
#pragma link C++ class FAVarParticleA2+;
#pragma link C++ class FAVarParticleA2_CoincT+;
#pragma link C++ class FAVarHistogram+;
#pragma link C++ class FAWrapPrim<Double32_t>+;
#pragma link C++ class FAParticleA2_B+;
#pragma link C++ class FAParticleA2_BF1+;
#pragma link C++ class FAParticleA2MC_B+;
#pragma link C++ class FAEventT<Double32_t, FAParticleA2_B, FAParticleA2MC_B>+;
#pragma link C++ class FAEventT<Double32_t, FAParticleA2_BF1, FAParticleA2MC_B>+;
#pragma link C++ class FAEventA2_B+;
#pragma link C++ class FAEventA2_BF1+;
#pragma link C++ class FAEventA2_BF2+;
#pragma link C++ class FAEventA2_BF3+;
#pragma link C++ class FAEventA2_BF4+;
#pragma link C++ class FAEventA2_BIF3+;
#pragma link C++ class FAEventA2_BIF4+;

#endif

