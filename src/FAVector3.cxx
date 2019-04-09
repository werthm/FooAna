/*************************************************************************
 * Author: Dominik Werthmueller, 2017-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVector3                                                            //
//                                                                      //
// Tree storage class for 3-vectors.                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TVector3.h"
#include "TMath.h"

#include "FAVector3.h"

ClassImp(FAVector3)

//______________________________________________________________________________
FAVector3::FAVector3(TVector3& v)
    : TObject()
{
    // Constructor.

    x = v.X();
    y = v.Y();
    z = v.Z();
}

//______________________________________________________________________________
FAVector3::FAVector3(const FAVector3& orig)
    : TObject(orig)
{
    // Copy constructor.

    x = orig.x;
    y = orig.y;
    z = orig.z;
}

//______________________________________________________________________________
Double_t FAVector3::Perp() const
{
    // Return the transverse component.

    return TMath::Sqrt(Perp2());
}

//______________________________________________________________________________
Double_t FAVector3::Theta() const
{
    // Return the azimuthal angle.

    return x == 0.0 && y == 0.0 && z == 0.0 ? 0.0 : TMath::ATan2(Perp(), z);
}

//______________________________________________________________________________
void FAVector3::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("x: %f  y: %f  z: %f\n", x, y, z);
}

//______________________________________________________________________________
void FAVector3::Clear(Option_t* option)
{
    // Clear the content of this class.

    x = 0;
    y = 0;
    z = 0;
}

//______________________________________________________________________________
FAVector3& FAVector3::operator=(const FAVector3& v)
{
    // Assignment operator.

    // check self assignment
    if (this != &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    return *this;
}

//______________________________________________________________________________
FAVector3& FAVector3::operator=(const TVector3& v)
{
    // Assignment operator.

    x = v.X();
    y = v.Y();
    z = v.Z();

    return *this;
}

//______________________________________________________________________________
FAVector3& FAVector3::operator+=(const FAVector3& v)
{
    // Compound assignment operator +=.

    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

//______________________________________________________________________________
const FAVector3 FAVector3::operator+(const FAVector3& v) const
{
    // Arithmetic Operator +.

    return FAVector3(*this) += v;
}

