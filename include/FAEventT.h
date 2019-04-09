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

    PartType* particle(Int_t i) { return (PartType*)part[i]; }
    PartTypeMC* particleMC(Int_t i) { return (PartTypeMC*)partMC[i]; }
    FAVector4* vector4(Int_t i) { return (FAVector4*)vec4[i]; }

    void AddParticle(const PartType& p);
    void AddParticleMC(const PartTypeMC& p);
    void AddVector4(const FAVector4& v);
    void AddVector4(const TLorentzVector& v);

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    virtual FAEventT& operator=(const FAEventT& e);

    ClassDef(FAEventT, 1)  // Event class template
};

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddParticle(const PartType& p)
{
    // Add a particle to the list of particles.

    new (part[nPart++]) PartType(p);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddParticleMC(const PartTypeMC& p)
{
    // Add a particle to the list of MC particles.

    new (partMC[nPartMC++]) PartTypeMC(p);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddVector4(const FAVector4& v)
{
    // Add a vector to the list of 4-vectors.

    new (vec4[nVec4++]) FAVector4(v);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddVector4(const TLorentzVector& v)
{
    // Add a vector to the list of 4-vectors.

    new (vec4[nVec4++]) FAVector4(v);
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
        part[i]->Print(option);
    }
    printf("Number of MC particles : %d\n", nPartMC);
    for (Int_t i = 0; i < nPartMC; i++)
    {
        printf("-> MC Particle %d\n", i+1);
        partMC[i]->Print(option);
    }
    printf("Number of 4-vectors    : %d\n", nVec4);
    for (Int_t i = 0; i < nVec4; i++)
    {
        printf("-> 4-vector %d\n", i+1);
        vec4[i]->Print(option);
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
    part.Clear();
    partMC.Clear();
    vec4.Clear();
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

