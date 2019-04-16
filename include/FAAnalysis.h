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
class TH1;
class TH2;
class TGraph;
class TCutG;
class FAVarFiller;
class FAAnalysisResult;

class FAAnalysis : public TObject
{

protected:
    TChain* fChain;                     // file chain
    Int_t fProgSrvPort;                 // port of progress server
    FAAnalysisResult* fResult;          // analysis result
    TAxis* fAxis1;                      // primary bin axis
    TAxis* fAxis2;                      // secondary bin axis
    std::vector<TH1*> fObj_TH1;         // vector of loaded TH1 objects
    std::vector<TH2*> fObj_TH2;         // vector of loaded TH2 objects
    std::vector<TGraph*> fObj_TGraph;   // vector of loaded TGraph objects
    std::vector<TCutG*> fObj_TCutG;     // vector of loaded TCutG objects

    TAxis* CreateAxis(Int_t index);
    template <class T>
    static void LoadObjects(const Char_t* objName, std::vector<T*>& list);

public:
    FAAnalysis() : TObject(),
                   fChain(0), fProgSrvPort(0), fResult(0),
                   fAxis1(0), fAxis2(0) { }
    FAAnalysis(const Char_t* cfg, const Char_t* treeName = 0);
    virtual ~FAAnalysis();

    Int_t GetProgressSrvPort() const { return fProgSrvPort; }
    TAxis* GetAxis1() const { return fAxis1; }
    TAxis* GetAxis2() const { return fAxis2; }
    TH1* GetObjectTH1(Int_t i) const { return fObj_TH1[i]; }
    TH2* GetObjectTH2(Int_t i) const { return fObj_TH2[i]; }
    TGraph* GetObjectTGraph(Int_t i) const { return fObj_TGraph[i]; }
    TCutG* GetObjectTCutG(Int_t i) const { return fObj_TCutG[i]; }

    void AddInput(const Char_t* path);

    void Process(std::function<FAAnalysisResult* (TTreeReader&)> func);
    void WriteOutputFile(const Char_t* out = 0);

    virtual void Print(Option_t* option = "") const;

    static FAAnalysisResult* WritePartialOutput(FAVarFiller& filler, const Char_t* inFile);

    ClassDef(FAAnalysis, 0)  // analysis wrapper
};

#endif

