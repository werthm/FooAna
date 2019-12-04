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


#include "TError.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"

#include "FAReaderSWeights.h"

ClassImp(FAReaderSWeights)

//______________________________________________________________________________
FAReaderSWeights::FAReaderSWeights(const Char_t* file, const Char_t* tree,
                                   const Char_t* event_id)
{
    // Constructor.
    // Read sWeights from the tree 'tree' in the file 'file' using the event id
    // branch 'event_id'.

    // init members
    fFile = nullptr;
    fTree = nullptr;
    fReader = nullptr;
    fREventID = nullptr;
    fIsAvail = kFALSE;

    // try to open file
    fFile = new TFile(file);
    if (fFile && !fFile->IsZombie())
    {
        // try to load tree
        fFile->GetObject(tree, fTree);
        if (!fTree)
        {
            Error("FAReaderSWeights", "Could not load tree '%s'", tree);
            return;
        }

        // configure tree reader
        fReader = new TTreeReader(fTree);
        fREventID = new TTreeReaderValue<Double_t>(*fReader, event_id);
    }
    else
    {
        Error("FAReaderSWeights", "Could not open file '%s'", file);
        return;
    }
}

//______________________________________________________________________________
FAReaderSWeights::~FAReaderSWeights()
{
    // Destructor.

    for (TTreeReaderValue<Float_t>* v : fWeights)
        delete v;
    if (fREventID)
        delete fREventID;
    if (fReader)
        delete fReader;
    if (fTree)
        delete fTree;
    if (fFile)
        delete fFile;
}

//______________________________________________________________________________
void FAReaderSWeights::LoadWeights(const Char_t* bname)
{
    // Register the weights in the branch 'bname' for reading.

    // check if reader exists
    if (fReader)
    {
        TTreeReaderValue<Float_t>* v = new TTreeReaderValue<Float_t>(*fReader, bname);
        fWeights.push_back(v);
    }
}

//______________________________________________________________________________
Bool_t FAReaderSWeights::ReadWeight(Double_t event_id, UInt_t index, Double_t& outw)
{
    // Read the weight with index 'index' for the event 'event_id' into 'outw'.
    // Return kTRUE if a weight was read, otherwise return kFALSE.

    // check for reader
    if (!fReader)
        return kFALSE;

    // check index
    if (index >= fWeights.size())
    {
        Error("ReadWeight", "Cannot read weight with index %d!", index);
        return kFALSE;
    }

    // read first tree entry
    if (!fIsAvail)
        fIsAvail = fReader->Next();

    // search event
    Bool_t found = kFALSE;
    for (;;)
    {
        // check if weights are available
        if (fIsAvail)
        {
            // matching event ID
            if (*(fREventID->Get()) == event_id)
            {
                outw = *(fWeights[index]->Get());
                found = kTRUE;
                break;
            }

            // higher event ID
            if (*(fREventID->Get()) > event_id)
                break;

            // load next entry
            fIsAvail = fReader->Next();
        }
        else
        {
            break;
        }
    }

    return found;
}

//______________________________________________________________________________
Bool_t FAReaderSWeights::ReadWeights(Double_t event_id, Double_t* outw)
{
    // Read all loaded weights for the event 'event_id' into 'outw' (NOTE: array
    // has to be large enough).
    // Return kTRUE if weights were read, otherwise return kFALSE.

    // check for reader
    if (!fReader)
        return kFALSE;

    // read first tree entry
    if (!fIsAvail)
        fIsAvail = fReader->Next();

    // search event
    Bool_t found = kFALSE;
    for (;;)
    {
        // check if weights are available
        if (fIsAvail)
        {
            // matching event ID
            if (*(fREventID->Get()) == event_id)
            {
                for (UInt_t i = 0; i < fWeights.size(); i++)
                    outw[i] = *(fWeights[i]->Get());
                found = kTRUE;
                break;
            }

            // higher event ID
            if (*(fREventID->Get()) > event_id)
                break;

            // load next entry
            fIsAvail = fReader->Next();
        }
        else
        {
            break;
        }
    }

    return found;
}

