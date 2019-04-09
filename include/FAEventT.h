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

#include <vector>

#include "FAVector4.h"

template <class PartType, class PartTypeMC>
class FAEventT
{

public:
    Short_t nPart;                      // number of particles
    Short_t nPartMC;                    // number of MC particles
    Short_t nVec4;                      // number of 4-vectors
    std::vector<PartType> part;         // array of particles
    std::vector<PartTypeMC> partMC;     // array of particles
    std::vector<FAVector4> vec4;        // array of 4-vectors

    FAEventT() : nPart(0), nPartMC(0), nVec4(0) { }
    FAEventT(const FAEventT& orig);
    virtual ~FAEventT() { }

    void AddParticle(PartType& p);
    void AddParticleMC(PartTypeMC& p);
    void AddVector4(FAVector4& v);
    void AddVector4(TLorentzVector& v);

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    virtual FAEventT& operator=(const FAEventT& e);

    ClassDef(FAEventT, 1)  // Event class template
};

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
FAEventT<PartType, PartTypeMC>::FAEventT(const FAEventT& orig)
{
    // Copy constructor.

    // init members
    nPart = orig.nPart;
    nPartMC = orig.nPartMC;
    nVec4 = orig.nVec4;
    part = orig.part;
    partMC = orig.partMC;
    vec4 = orig.vec4;
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddParticle(PartType& p)
{
    // Add a particle to the list of particles.

    part.push_back(p);
    nPart++;
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddParticleMC(PartTypeMC& p)
{
    // Add a particle to the list of MC particles.

    partMC.push_back(p);
    nPartMC++;
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddVector4(FAVector4& v)
{
    // Add a vector to the list of 4-vectors.

    vec4.push_back(v);
    nVec4++;
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddVector4(TLorentzVector& v)
{
    // Add a vector to the list of 4-vectors.

    vec4.push_back(FAVector4(v));
    nVec4++;
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("Number of particles    : %d\n", nPart);
    for (Int_t i = 0; i < nPart; i++)
    {
        printf("-> Particle %d\n", i+1);
        part[i].Print(option);
    }
    printf("Number of MC particles : %d\n", nPartMC);
    for (Int_t i = 0; i < nPartMC; i++)
    {
        printf("-> MC Particle %d\n", i+1);
        partMC[i].Print(option);
    }
    printf("Number of 4-vectors    : %d\n", nVec4);
    for (Int_t i = 0; i < nVec4; i++)
    {
        printf("-> 4-vector %d\n", i+1);
        vec4[i].Print(option);
    }
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::Clear(Option_t* option)
{
    // Prepare class for a new event by clearing all members.

    nPart = 0;
    nPartMC = 0;
    nVec4 = 0;
    part.clear();
    partMC.clear();
    vec4.clear();
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
FAEventT<PartType, PartTypeMC>& FAEventT<PartType, PartTypeMC>::operator=(const FAEventT& e)
{
    // Assignment operator.

    // check self assignment
    if (this != &e)
    {
        nPart = e.nPart;
        nPartMC = e.nPartMC;
        nVec4 = e.nVec4;
        part = e.part;
        partMC = e.partMC;
        vec4 = e.vec4;
    }

    return *this;
}

#endif

