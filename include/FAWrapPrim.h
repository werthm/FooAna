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


#ifndef FooAna_FAWrapPrim
#define FooAna_FAWrapPrim

#include "TObject.h"

template <class Type>
class FAWrapPrim : public TObject
{

public:
    Type var;                  // variable

    FAWrapPrim() : TObject(),
                   var(0) { };
    FAWrapPrim(Type var) : TObject(),
                           var(var) { };
    virtual ~FAWrapPrim() { }

    virtual void Print(Option_t* option = "") const;
    virtual void Clear(Option_t* option = "");

    FAWrapPrim& operator=(Type val);
    Bool_t operator==(Type val) const { return var == val; }
    Bool_t operator!=(Type val) const { return var != val; }
    Bool_t operator<(Type val) const { return var < val; }
    Bool_t operator>(Type val) const { return var > val; }
    Bool_t operator<=(Type val) const { return var <= val; }
    Bool_t operator>=(Type val) const { return var >= val; }
    FAWrapPrim& operator=(const FAWrapPrim& v);
    Bool_t operator==(const FAWrapPrim& v) const { return var == v.var; }
    Bool_t operator!=(const FAWrapPrim& v) const { return var != v.var; }
    Bool_t operator<(const FAWrapPrim& v) const { return var < v.var; }
    Bool_t operator>(const FAWrapPrim& v) const { return var > v.var; }
    Bool_t operator<=(const FAWrapPrim& v) const { return var <= v.var; }
    Bool_t operator>=(const FAWrapPrim& v) const { return var >= v.var; }

    ClassDef(FAWrapPrim, 1)  // primitive wrapper
};

#endif

