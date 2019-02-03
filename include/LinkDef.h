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


#ifdef __CINT__

// turn everything off
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all typedef;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;

#pragma link C++ namespace FAUtils;
#pragma link C++ class FAVector3+;
#pragma link C++ class FAVector4+;
#pragma link C++ class FAVarAbs+;
#pragma link C++ class FAVar<Short_t>+;
#pragma link C++ class FAVar<Int_t>+;
#pragma link C++ class FAVar<Long64_t>+;
#pragma link C++ class FAVar<Float_t>+;
#pragma link C++ class FAVar<Double_t>+;
#pragma link C++ class FAVarList+;
#pragma link C++ class FAVarParticleA2+;
#pragma link C++ class FAVarParticleA2KF+;
#pragma link C++ class FAVarHistogram+;
#pragma link C++ class FAParticle+;
#pragma link C++ class FAParticleA2+;
#pragma link C++ class FAParticleA2KF+;
#pragma link C++ class FAParticleMC+;
#pragma link C++ class FAParticleMCA2+;
#pragma link C++ class FAEventBase+;
#pragma link C++ class FAEventBaseA2+;
#pragma link C++ class FAEventA2+;
#pragma link C++ class FAEventT<FAParticleA2, FAParticleMCA2>+;
#pragma link C++ class FAEventA2KF+;
#pragma link C++ class FAEventT<FAParticleA2KF, FAParticleMCA2>+;

#endif

