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
    std::vector<PartType> part;         // array of particles
    std::vector<PartTypeMC> partMC;     // array of particles

    FAEventT() : FAEventBase() { }
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
    part = orig.part;
    partMC = orig.partMC;
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddParticle(PartType& p)
{
    // Add a particle to the list of particles.

    part.push_back(p);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::AddParticleMC(PartTypeMC& p)
{
    // Add a particle to the list of MC particles.

    partMC.push_back(p);
}

//______________________________________________________________________________
template <class PartType, class PartTypeMC>
void FAEventT<PartType, PartTypeMC>::Print(Option_t* option) const
{
    // Print the content of this class.

    FAEventBase::Print(option);
    printf("Number of particles    : %ld\n", part.size());
    for (UInt_t i = 0; i < part.size(); i++)
    {
        printf("-> Particle %d\n", i+1);
        part[i].Print(option);
    }
    printf("Number of MC particles : %ld\n", partMC.size());
    for (UInt_t i = 0; i < partMC.size(); i++)
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
        part = e.part;
        partMC = e.partMC;
    }

    return *this;
}

#endif

