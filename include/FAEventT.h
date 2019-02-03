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

#include "FAEventBase.h"

template <class PartType, class PartTypeMC>
class FAEventT : public FAEventBase
{

public:
    Short_t nPart;                      // number of particles
    Short_t nPartMC;                    // number of MC particles
    std::vector<PartType> part;         // array of particles
    std::vector<PartTypeMC> partMC;     // array of particles

    FAEventT() : FAEventBase(),
                 nPart(0), nPartMC(0) { }
    FAEventT(const FAEventT& orig);
    virtual ~FAEventT() { }

    void AddParticle(PartType& p);
    void AddParticleMC(PartTypeMC& p);

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    virtual FAEventT& operator=(const FAEventT& e);

    ClassDef(FAEventT, 1)  // Event class template
};

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
FAEventT<PartType, PartTypeMC>::FAEventT(const FAEventT& orig)
    : FAEventBase(orig)
{
    // Copy constructor.

    // init members
    nPart = orig.nPart;
    nPartMC = orig.nPartMC;
    part = orig.part;
    partMC = orig.partMC;
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
void FAEventT<PartType, PartTypeMC>::Print(Option_t* option) const
{
    // Print the content of this class.

    FAEventBase::Print(option);
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
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::Clear(Option_t* option)
{
    // Prepare class for a new event by clearing all members.

    FAEventBase::Clear(option);
    nPart = 0;
    nPartMC = 0;
    part.clear();
    partMC.clear();
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
FAEventT<PartType, PartTypeMC>& FAEventT<PartType, PartTypeMC>::operator=(const FAEventT& e)
{
    // Assignment operator.

    // check self assignment
    if (this != &e)
    {
        FAEventBase::operator=(e);
        nPart = e.nPart;
        nPartMC = e.nPartMC;
        part = e.part;
        partMC = e.partMC;
    }

    return *this;
}

#endif

