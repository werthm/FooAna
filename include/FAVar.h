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


#ifndef FooAna_FAVar
#define FooAna_FAVar

#include "FAVarAbs.h"

template <class Type>
class FAVar : public FAVarAbs
{

protected:
    Type fVar;                      // analysis variable

public:
    FAVar() : FAVarAbs(),
              fVar(0) { }
    FAVar(const Char_t* name, const Char_t* title, const Char_t* unit = 0,
          Int_t nbins = 0, Double_t min = 0, Double_t max = 0,
          UInt_t statusBits = 0);
    FAVar(const Char_t* name, const Char_t* title, const Char_t* unit,
          TAxis* binning,
          UInt_t statusBits = 0);
    FAVar(FAVarFiller& filler, const Char_t* name, const Char_t* title,
          const Char_t* unit = 0,
          Int_t nbins = 0, Double_t min = 0, Double_t max = 0,
          UInt_t statusBits = 0);
    FAVar(FAVarFiller& filler, const Char_t* name, const Char_t* title,
          const Char_t* unit,
          TAxis* binning,
          UInt_t statusBits = 0);
    virtual ~FAVar() { }

    Type GetVar() const { return fVar; }

    void SetVar(Type val) { fVar = val; }
    virtual void SetDouble(Double_t d) { fVar = d; }

    virtual Double_t AsDouble() const { return (Double_t)fVar; }
    virtual void* GetVarPtr() const { return (void*)&fVar; }
    virtual Char_t GetTypeChar() const;

    FAVar& operator=(Type val);
    Bool_t operator==(Type val) const { return fVar == val; }
    Bool_t operator!=(Type val) const { return fVar != val; }
    Bool_t operator<(Type val) const { return fVar < val; }
    Bool_t operator>(Type val) const { return fVar > val; }
    Bool_t operator<=(Type val) const { return fVar <= val; }
    Bool_t operator>=(Type val) const { return fVar >= val; }
    FAVar& operator=(const FAVar& v);
    Bool_t operator==(const FAVar& v) const { return fVar == v.fVar; }
    Bool_t operator!=(const FAVar& v) const { return fVar != v.fVar; }
    Bool_t operator<(const FAVar& v) const { return fVar < v.fVar; }
    Bool_t operator>(const FAVar& v) const { return fVar > v.fVar; }
    Bool_t operator<=(const FAVar& v) const { return fVar <= v.fVar; }
    Bool_t operator>=(const FAVar& v) const { return fVar >= v.fVar; }

    FAVar& operator+=(Type val);
    FAVar& operator-=(Type val);
    FAVar& operator*=(Type val);
    FAVar& operator/=(Type val);

    ClassDef(FAVar, 1)  // analysis variable
};

#endif

