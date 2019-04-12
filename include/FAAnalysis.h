/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAAnalysis                                                           //
//                                                                      //
// FooAna analysis wrapper.                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAAnalysis
#define FooAna_FAAnalysis

#include <functional>

#include "TObject.h"

class TChain;
class TTreeReader;
class TAxis;
class FAVarFiller;
class FAAnalysisResult;

class FAAnalysis : public TObject
{

protected:
    TChain* fChain;                 // file chain
    Int_t fProgSrvPort;             // port of progress server
    FAAnalysisResult* fResult;      // analysis result
    TAxis* fAxis1;                  // primary bin axis
    TAxis* fAxis2;                  // secondary bin axis

    TAxis* CreateAxis(Int_t index);

public:
    FAAnalysis() : TObject(),
                   fChain(0), fProgSrvPort(0), fResult(0),
                   fAxis1(0), fAxis2(0) { }
    FAAnalysis(const Char_t* cfg);
    virtual ~FAAnalysis();

    Int_t GetProgressSrvPort() const { return fProgSrvPort; }
    TAxis* GetAxis1() const { return fAxis1; }
    TAxis* GetAxis2() const { return fAxis2; }

    void Process(std::function<FAAnalysisResult* (TTreeReader&)> func);
    void WriteOutputFile(const Char_t* out = 0);

    static FAAnalysisResult* WritePartialOutput(FAVarFiller& filler, const Char_t* inFile);

    ClassDef(FAAnalysis, 0)  // analysis wrapper
};

#endif

