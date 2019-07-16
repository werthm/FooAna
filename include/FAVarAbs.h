/*************************************************************************
 * Author: Dominik Werthmueller, 2018-2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAVarAbs                                                             //
//                                                                      //
// Abstract base class for analysis variables.                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAVarAbs
#define FooAna_FAVarAbs

#include "TNamed.h"

#include "FAVarFiller.h"

class TAxis;
class FAVarList;

class FAVarAbs : public TNamed
{

public:
    // status bits
    enum EStatusBits {
        kNoUnbinned = BIT(14),      // no unbinned storage
        kNoBinned = BIT(15),        // no binned storage
        kNoFill = BIT(16),          // no binned filling
        kDoNorm = BIT(17)           // divide by (unweighted) normalization histogram
    };

protected:
    TString fUnit;                  // unit of variable
    TAxis* fBins;                   // binning of variable
    Double_t fWeight;               // individual weight of variable
    FAVarList* fRelVar;             // list of related variables

public:
    FAVarAbs() : TNamed(),
                 fUnit(),
                 fBins(0),
                 fWeight(1),
                 fRelVar(0) { }
    FAVarAbs(const Char_t* name, const Char_t* title, const Char_t* unit = 0,
             Int_t nbins = 0, Double_t min = 0, Double_t max = 0,
             UInt_t statusBits = 0);
    FAVarAbs(const Char_t* name, const Char_t* title, const Char_t* unit,
             TAxis* binning,
             UInt_t statusBits = 0);
    FAVarAbs(FAVarFiller& filler, const Char_t* name, const Char_t* title,
             const Char_t* unit = 0,
             Int_t nbins = 0, Double_t min = 0, Double_t max = 0,
             UInt_t statusBits = 0);
    FAVarAbs(FAVarFiller& filler, const Char_t* name, const Char_t* title,
             const Char_t* unit,
             TAxis* binning,
             UInt_t statusBits = 0);
    virtual ~FAVarAbs();

    const Char_t* GetUnit() const { return fUnit.Data(); }
    Bool_t HasUnit() const { return fUnit == "" ? kFALSE : kTRUE; }
    TAxis* GetBins() const { return fBins; }
    Double_t GetWeight() const { return fWeight; }
    FAVarList* GetRelatedVariables() const { return fRelVar; }

    void SetUnit(const Char_t* unit) { fUnit = unit; }
    void SetWeight(Double_t w) { fWeight = w; }
    virtual void SetDouble(Double_t d) = 0;

    void AddRelatedVariable(FAVarAbs* var);

    virtual Double_t AsDouble() const = 0;
    virtual void* GetVarPtr() const = 0;
    virtual Char_t GetTypeChar() const = 0;

    ClassDef(FAVarAbs, 1)  // abstract analysis variable
};

#endif

