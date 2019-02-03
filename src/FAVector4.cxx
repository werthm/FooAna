/*************************************************************************
 * Author: Dominik Werthmueller, 2017-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVector4                                                            //
//                                                                      //
// Tree storage class for 4-vectors.                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TLorentzVector.h"

#include "FAVector4.h"

ClassImp(FAVector4)

//______________________________________________________________________________
FAVector4::FAVector4(TLorentzVector& p)
    : FAVector3(p.Px(), p.Py(), p.Pz())
{
    // Constructor.

    t = p.E();
}

//______________________________________________________________________________
FAVector4::FAVector4(const FAVector4& orig)
    : FAVector3(orig)
{
    // Copy constructor.

    t = orig.t;
}

//______________________________________________________________________________
void FAVector4::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("Px: %f  Py: %f  Pz: %f  E: %f\n", x, y, z, t);
}

//______________________________________________________________________________
void FAVector4::Clear(Option_t* option)
{
    // Clear the content of this class.

    FAVector3::Clear(option);
    t = 0;
}

//______________________________________________________________________________
Double_t FAVector4::M() const
{
    // Calculate the invariant mass.

    Double_t mm = M2();
    return mm < 0.0 ? -TMath::Sqrt(-mm) : TMath::Sqrt(mm);
}

//______________________________________________________________________________
Double_t FAVector4::M2() const
{
    // Calculate the invariant mass squared.

    return t*t - x*x - y*y - z*z;
}

//______________________________________________________________________________
void FAVector4::Convert4Vector(TLorentzVector& out)
{
    // Save this 4-vector to 'out'.

    out.SetPxPyPzE(x, y, z, t);
}

//______________________________________________________________________________
FAVector4& FAVector4::operator=(const FAVector4& p)
{
    // Assignment operator.

    // check self assignment
    if (this != &p)
    {
        FAVector3::operator=(p);
        t = p.t;
    }

    return *this;
}

//______________________________________________________________________________
FAVector4& FAVector4::operator=(const TLorentzVector& p)
{
    // Assignment operator.

    FAVector3::operator=(p.Vect());
    t = p.E();

    return *this;
}

//______________________________________________________________________________
FAVector4& FAVector4::operator+=(const FAVector4& p)
{
    // Compound assignment operator +=.

    FAVector3::operator+=(p);
    t += p.t;

    return *this;
}

//______________________________________________________________________________
const FAVector4 FAVector4::operator+(const FAVector4& p) const
{
    // Arithmetic Operator +.

    return FAVector4(*this) += p;
}

