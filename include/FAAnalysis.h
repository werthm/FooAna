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
class TF1;
class FAVarFiller;
class FAAnalysisResult;

class FAAnalysis : public TObject
{

public:
    // analysis mode
    enum EAnaMode {
        kData,
        kMC
    };

protected:
    EAnaMode fMode;                     // analysis mode
    TDirectory* fOrigGDir;              // original gDirectory
    TChain* fChain;                     // file chain
    Int_t fProgSrvPort;                 // port of progress server
    FAAnalysisResult* fResult;          // analysis result
    TAxis* fAxis1;                      // primary bin axis
    TAxis* fAxis2;                      // secondary bin axis
    std::vector<TH1*> fObj_TH1;         // vector of loaded TH1 objects
    std::vector<TH2*> fObj_TH2;         // vector of loaded TH2 objects
    std::vector<TGraph*> fObj_TGraph;   // vector of loaded TGraph objects
    std::vector<TCutG*> fObj_TCutG;     // vector of loaded TCutG objects
    std::vector<TF1*> fObj_TF1;         // vector of loaded TF1 objects

    TAxis* CreateAxis(Int_t index);
    template <class T>
    void LoadObjects(const Char_t* objName, std::vector<T*>& list);
    TString LoadKeyString(const Char_t* name, const Char_t* defValue);
    Int_t LoadKeyInt(const Char_t* name, Int_t defValue);
    Double_t LoadKeyDouble(const Char_t* name, Double_t defValue);

public:
    FAAnalysis() : TObject(),
                   fMode(kData),
                   fOrigGDir(0),
                   fChain(0), fProgSrvPort(0), fResult(0),
                   fAxis1(0), fAxis2(0) { }
    FAAnalysis(EAnaMode mode, const Char_t* cfg, const Char_t* treeName = 0);
    virtual ~FAAnalysis();

    TDirectory* GetOrigGDirectory() const { return fOrigGDir; }
    Int_t GetProgressSrvPort() const { return fProgSrvPort; }
    TAxis* GetAxis1() const { return fAxis1; }
    TAxis* GetAxis2() const { return fAxis2; }
    TH1* GetTH1(Int_t i) const { return fObj_TH1[i]; }
    TH2* GetTH2(Int_t i) const { return fObj_TH2[i]; }
    TGraph* GetTGraph(Int_t i) const { return fObj_TGraph[i]; }
    TCutG* GetTCutG(Int_t i) const { return fObj_TCutG[i]; }
    TF1* GetTF1(Int_t i) const { return fObj_TF1[i]; }

    void AddInput(const Char_t* path);

    void Process(std::function<FAAnalysisResult* (TTreeReader&)> func);
    void WriteOutputFile(const Char_t* out = 0);

    virtual void Print(Option_t* option = "") const;

    static FAAnalysisResult* WritePartialOutput(FAVarFiller& filler, const Char_t* inFile,
                                                Bool_t flat = kFALSE);

    ClassDef(FAAnalysis, 0)  // analysis wrapper
};

#endif

