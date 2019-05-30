/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVar                                                                //
//                                                                      //
// Template class for analysis variables.                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TClass.h"
#include "TList.h"
#include "TRealData.h"
#include "TDataMember.h"
#include "TDataType.h"

#include "FAVar.h"
#include "FAUtils.h"

templateClassImp(FAVar)

//______________________________________________________________________________
template <class Type>
FAVar<Type>::FAVar(const Char_t* name, const Char_t* title, const Char_t* unit,
                   Int_t nbins, Double_t min, Double_t max,
                   UInt_t statusBits)
    : FAVarAbs(name, title, unit, nbins, min, max, statusBits)
{
    // Constructor.

    // init members
    fVar = 0;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>::FAVar(const Char_t* name, const Char_t* title, const Char_t* unit,
                   TAxis* binning,
                   UInt_t statusBits)
    : FAVarAbs(name, title, unit, binning, statusBits)
{
    // Constructor.

    // init members
    fVar = 0;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>::FAVar(FAVarFiller& filler, const Char_t* name, const Char_t* title,
                   const Char_t* unit,
                   Int_t nbins, Double_t min, Double_t max,
                   UInt_t statusBits)
    : FAVarAbs(filler, name, title, unit, nbins, min, max, statusBits)
{
    // Constructor.

    // init members
    fVar = 0;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>::FAVar(FAVarFiller& filler, const Char_t* name, const Char_t* title,
                   const Char_t* unit,
                   TAxis* binning,
                   UInt_t statusBits)
    : FAVarAbs(filler, name, title, unit, binning, statusBits)
{
    // Constructor.

    // init members
    fVar = 0;
}

//______________________________________________________________________________
template <class Type>
Char_t FAVar<Type>::GetTypeChar() const
{
    // Return the variable type identifier used to specify TTree branches.

    // some code found in TTree
    TRealData* rdm = (TRealData*)Class()->GetListOfRealData()->At(0);
    TDataType* dmtype = rdm->GetDataMember()->GetDataType();
    return FAUtils::TreeDataTypeToChar((EDataType)dmtype->GetType());
}

//______________________________________________________________________________
template <class Type>
Bool_t FAVar<Type>::IsCutSigmaOk(Double_t mean, Double_t sigma, Double_t cut) const
{
    // Check if the value of the variable fulfills a 'cut'-sigma cut around
    // 'mean' with sigma 'sigma'.

    if (fVar > mean - cut*sigma && fVar < mean + cut*sigma)
        return kTRUE;
    else
        return kFALSE;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>& FAVar<Type>::operator=(Type val)
{
    // Assignment operator.

    fVar = val;

    return *this;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>& FAVar<Type>::operator=(const FAVar& v)
{
    // Assignment operator.

    fVar = v.fVar;

    return *this;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>& FAVar<Type>::operator+=(Type val)
{
    // Assignment operator.

    fVar += val;

    return *this;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>& FAVar<Type>::operator-=(Type val)
{
    // Assignment operator.

    fVar -= val;

    return *this;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>& FAVar<Type>::operator*=(Type val)
{
    // Assignment operator.

    fVar *= val;

    return *this;
}

//______________________________________________________________________________
template <class Type>
FAVar<Type>& FAVar<Type>::operator/=(Type val)
{
    // Assignment operator.

    fVar /= val;

    return *this;
}

// template instantiations
template class FAVar<Short_t>;
template class FAVar<Int_t>;
template class FAVar<Long64_t>;
template class FAVar<Float_t>;
template class FAVar<Double_t>;

