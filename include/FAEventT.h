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

template <class VarType, class PartType, class PartTypeMC>
class FAEventT
{

public:
    virtual ~FAEventT() { }

    std::vector<VarType> var;       // list of variables
    std::vector<FAVector4> vec4;    // list of 4-vectors
    std::vector<PartType> part;     // list of particles
    std::vector<PartTypeMC> partMC; // list of particles

    void AddVariable(VarType v) { var.push_back(v); }
    void AddVector4(const FAVector4& v) { vec4.push_back(v); }
    void AddParticle(const PartType& p) { part.push_back(p); }
    void AddParticleMC(const PartTypeMC& p) { partMC.push_back(p); }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "")
    {
        // Prepare class for a new event by clearing all members.

        var.clear();
        vec4.clear();
        part.clear();
        partMC.clear();
    }

    virtual FAEventT& operator=(const FAEventT& e)
    {
        // Assignment operator.

        // check self assignment
        if (this != &e)
        {
            var = e.var;
            vec4 = e.vec4;
            part = e.part;
            partMC = e.partMC;
        }
        return *this;
    }

    ClassDef(FAEventT, 2)  // Event class template
};

#endif

