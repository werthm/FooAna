/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAEventT                                                             //
//                                                                      //
// This class is the template for FooAna events.                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAEventT
#define FooAna_FAEventT

#include "TClonesArray.h"
#include "TClass.h"

#include "FAVector4.h"

class TLorentzVector;

template <class PartType, class PartTypeMC>
class FAEventT
{

public:
    Short_t nPart;                      // number of particles
    Short_t nPartMC;                    // number of MC particles
    Short_t nVec4;                      // number of 4-vectors
    TClonesArray part;                  // array of particles
    TClonesArray partMC;                // array of particles
    TClonesArray vec4;                  // array of 4-vectors

    FAEventT() : nPart(0),
                 nPartMC(0),
                 nVec4(0),
                 part(PartType::Class(), 10),
                 partMC(PartTypeMC::Class(), 10),
                 vec4(FAVector4::Class(), 10) { }
    FAEventT(const FAEventT& orig) : nPart(orig.nPart),
                                     nPartMC(orig.nPartMC),
                                     nVec4(orig.nVec4),
                                     part(orig.part),
                                     partMC(orig.partMC),
                                     vec4(orig.vec4) { }
    virtual ~FAEventT() { }

    PartType* particle(Int_t i);
    PartTypeMC* particleMC(Int_t i);
    FAVector4* vector4(Int_t i);

    void AddParticle(const PartType& p);
    void AddParticleMC(const PartTypeMC& p);
    void AddVector4(const FAVector4& v);
    void AddVector4(const TLorentzVector& v);

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    virtual FAEventT& operator=(const FAEventT& e);

    ClassDef(FAEventT, 1)  // Event class template
};

#endif

