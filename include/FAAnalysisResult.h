/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAAnalysisResult                                                     //
//                                                                      //
// FooAna analysis result class.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAAnalysisResult
#define FooAna_FAAnalysisResult

#include "TNamed.h"

class FAAnalysisResult : public TNamed
{

protected:
    TObjArray* fFiles;           //-> list of partial files

    void Init();

public:
    FAAnalysisResult() : TNamed(),
                         fFiles(0) { }
    FAAnalysisResult(const Char_t* name, const Char_t* file);
    virtual ~FAAnalysisResult();

    Int_t GetNFiles() const;
    const Char_t* GetFile(Int_t i) const;
    void AddFile(const Char_t* f);

    Long64_t Merge(TCollection* c);
    virtual void Print(Option_t* option = "") const;

    ClassDef(FAAnalysisResult, 1)  // analysis result
};

#endif

