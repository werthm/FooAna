/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAAnalysisCLAS12                                                     //
//                                                                      //
// FooAna CLAS12 analysis wrapper.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAAnalysisCLAS12
#define FooAna_FAAnalysisCLAS12

#include "FAAnalysis.h"

class FAAnalysisCLAS12 : public FAAnalysis
{

protected:
    Double_t fBeamE;        // electron beam energy

public:
    FAAnalysisCLAS12() : FAAnalysis(),
                         fBeamE(0) { }
    FAAnalysisCLAS12(EAnaMode mode, const Char_t* cfg, const Char_t* treeName = 0);
    virtual ~FAAnalysisCLAS12() { }

    Double_t GetBeamEnergy() const { return fBeamE; }

    virtual void Print(Option_t* option = "") const;

    ClassDef(FAAnalysisCLAS12, 0)  // CLAS12 analysis wrapper
};

#endif

