/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAWrapPrim                                                           //
//                                                                      //
// Template class for primitive TObject-wrappers.                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "FAWrapPrim.h"

templateClassImp(FAWrapPrim)

//______________________________________________________________________________
template <class Type>
void FAWrapPrim<Type>::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("%f\n", var);
}

//______________________________________________________________________________
template <class Type>
void FAWrapPrim<Type>::Clear(Option_t* option)
{
    // Clear the content of this class.

    var = 0;
}

//______________________________________________________________________________
template <class Type>
FAWrapPrim<Type>& FAWrapPrim<Type>::operator=(Type val)
{
    // Assignment operator.

    var = val;

    return *this;
}

//______________________________________________________________________________
template <class Type>
FAWrapPrim<Type>& FAWrapPrim<Type>::operator=(const FAWrapPrim& v)
{
    // Assignment operator.

    var = v.var;

    return *this;
}

// template instantiations
template class FAWrapPrim<Double32_t>;

