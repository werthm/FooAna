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


#ifndef FooAna_FAVector3
#define FooAna_FAVector3

#include "Rtypes.h"

class TVector3;

class FAVector3
{

public:
    Double32_t x;
    Double32_t y;
    Double32_t z;

    FAVector3()
        : x(0), y(0), z(0) { }
    FAVector3(Double_t x_in, Double_t y_in, Double_t z_in)
        : x(x_in), y(y_in), z(z_in) { }
    FAVector3(TVector3& v);
    FAVector3(const FAVector3& orig);
    virtual ~FAVector3() { }

    void SetXYZ(Double_t x_in, Double_t y_in, Double_t z_in)
    {
        x = x_in;
        y = y_in;
        z = z_in;
    }

    Double_t Perp2() const { return x*x + y*y; }
    Double_t Perp() const;
    Double_t Theta() const;
    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    FAVector3& operator=(const FAVector3& v);
    FAVector3& operator=(const TVector3& v);
    FAVector3& operator+=(const FAVector3& v);
    const FAVector3 operator+(const FAVector3& v) const;

    ClassDef(FAVector3, 1)  // tree storage for 3-vectors
};

#endif

