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

#include "FAFooAna.h"

class TLorentzVector;

template <class VarType, class PartType, class PartTypeMC>
class FAEventT
{

public:
    virtual ~FAEventT() { }

    std::vector<VarType> vars;      // list of variables
    std::vector<FAVector4> vec4;    // list of 4-vectors
    std::vector<PartType> part;     // list of particles
    std::vector<PartTypeMC> partMC; // list of particles

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

