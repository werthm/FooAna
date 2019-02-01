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

