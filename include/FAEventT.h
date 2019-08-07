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
#include "FAWrapPrim.h"

class TLorentzVector;

template <class VarType, class PartType, class PartTypeMC>
class FAEventT
{

public:
    Short_t nVar;                       // number of variables
    Short_t nVec4;                      // number of 4-vectors
    Short_t nPart;                      // number of particles
    Short_t nPartMC;                    // number of MC particles
    TClonesArray vars;                  // array of variables
    TClonesArray vec4;                  // array of 4-vectors
    TClonesArray part;                  // array of particles
    TClonesArray partMC;                // array of particles

    FAEventT() : nVar(0),
                 nVec4(0),
                 nPart(0),
                 nPartMC(0),
                 vars(FAWrapPrim<VarType>::Class(), 10),
                 vec4(FAVector4::Class(), 10),
                 part(PartType::Class(), 10),
                 partMC(PartTypeMC::Class(), 10) { }
    FAEventT(const FAEventT& orig) : nVar(orig.nVar),
                                     nVec4(orig.nVec4),
                                     nPart(orig.nPart),
                                     nPartMC(orig.nPartMC),
                                     vars(orig.vars),
                                     vec4(orig.vec4),
                                     part(orig.part),
                                     partMC(orig.partMC) { }
    virtual ~FAEventT() { }

    FAWrapPrim<VarType>* variable(Int_t i);
    FAVector4* vector4(Int_t i);
    PartType* particle(Int_t i);
    PartTypeMC* particleMC(Int_t i);

    void AddVariable(VarType v);
    void AddVector4(const FAVector4& v);
    void AddVector4(const TLorentzVector& v);
    void AddParticle(const PartType& p);
    void AddParticleMC(const PartTypeMC& p);

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    virtual FAEventT& operator=(const FAEventT& e);

    ClassDef(FAEventT, 2)  // Event class template
};

#endif

