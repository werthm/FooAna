/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAReaderSWeights                                                     //
//                                                                      //
// Read sWeights from a TTree.                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef FooAna_FAReaderSWeights
#define FooAna_FAReaderSWeights

#include <vector>

#include "TTreeReaderValue.h"

class TFile;
class TTree;
class TTreeReader;

class FAReaderSWeights
{

protected:
    TFile* fFile;                                       // input file
    TTree* fTree;                                       // sWeights tree
    TTreeReader* fReader;                               // tree reader
    TTreeReaderValue<Double_t>* fREventID;              // tree reader value
    std::vector<TTreeReaderValue<Float_t>*> fWeights;   // list of weights reader values
    Bool_t fIsAvail;                                    // flag for available weights

public:
    FAReaderSWeights() : fFile(nullptr), fTree(nullptr),
                         fReader(nullptr), fREventID(nullptr),
                         fIsAvail(kFALSE) { }
    FAReaderSWeights(const Char_t* file, const Char_t* tree,
                     const Char_t* event_id = "event_id");
    virtual ~FAReaderSWeights();

    void LoadWeights(const Char_t* bname);
    Bool_t ReadWeight(Double_t event_id, UInt_t index, Double_t& outw);
    Bool_t ReadWeights(Double_t event_id, Double_t* outw);

    ClassDef(FAReaderSWeights, 0)  // sWeights reader
};

#endif

