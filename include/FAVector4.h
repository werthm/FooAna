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


#ifndef FooAna_FAVector4
#define FooAna_FAVector4

#include "FAVector3.h"

class TLorentzVector;

class FAVector4 : public FAVector3
{

public:
    Double32_t t;

    FAVector4()
        : FAVector3(),
          t(0) { }
    FAVector4(Double_t x_in, Double_t y_in, Double_t z_in, Double_t t_in)
        : FAVector3(x_in, y_in, z_in),
          t(t_in) { }
    FAVector4(const TLorentzVector& p);
    FAVector4(const FAVector4& orig);
    virtual ~FAVector4() { }

    void SetPxPyPzE(Double_t px, Double_t py, Double_t pz, Double_t e)
    {
        x = px;
        y = py;
        z = pz;
        t = e;
    }

    Double_t M() const;
    Double_t M2() const;
    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    void Convert4Vector(TLorentzVector& out);

    FAVector4& operator=(const FAVector4& p);
    FAVector4& operator=(const TLorentzVector& p);
    FAVector4& operator+=(const FAVector4& p);
    const FAVector4 operator+(const FAVector4& p) const;

    ClassDef(FAVector4, 1)  // tree storage for 4-vectors
};

#endif

