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


#include "TCollection.h"
#include "TObjArray.h"
#include "TObjString.h"

#include "FAAnalysisResult.h"

ClassImp(FAAnalysisResult)

//______________________________________________________________________________
FAAnalysisResult::FAAnalysisResult(const Char_t* name, const Char_t* file)
    : TNamed(name, name)
{
    // Constructor.

    Init();
    fFiles->Add(new TObjString(file));
}

//______________________________________________________________________________
FAAnalysisResult::~FAAnalysisResult()
{
    // Destructor.

    if (fFiles)
        delete fFiles;
}

//______________________________________________________________________________
void FAAnalysisResult::Init()
{
    // Init this class.

    fFiles = new TObjArray();
    fFiles->SetOwner(kTRUE);
}

//______________________________________________________________________________
Int_t FAAnalysisResult::GetNFiles() const
{
    // Return the number of files.

    return fFiles ? fFiles->GetEntries() : 0;
}

//______________________________________________________________________________
const Char_t* FAAnalysisResult::GetFile(Int_t i) const
{
    // Return the file at index 'i'.

    if (fFiles)
        return ((TObjString*)fFiles->At(i))->String().Data();
    else
        return 0;
}

//______________________________________________________________________________
void FAAnalysisResult::AddFile(const Char_t* f)
{
    // Add the file 'f' to the list of partial files.

    if (!fFiles)
        Init();
    fFiles->Add(new TObjString(f));
}

//______________________________________________________________________________
Long64_t FAAnalysisResult::Merge(TCollection* c)
{
    // Merge results.

    // check incoming collection
    if (!c)
        return 0;
    if (c->IsEmpty())
        return 0;

    // init class if necessary
    if (!fFiles)
        Init();

    // loop over entries
    Int_t n = 0;
    TIter next(c);
    while (TObject* o = next())
    {
        // try to cast
        FAAnalysisResult* r = (FAAnalysisResult*)o;
        if (r)
        {
            // loop over files
            for (Int_t i =  0; i < r->GetNFiles(); i++)
                fFiles->Add(new TObjString(r->GetFile(i)));
            n++;
        }
    }

    return n;
}

//______________________________________________________________________________
void FAAnalysisResult::Print(Option_t* option) const
{
    // Print the content of this class.

    Int_t nFiles = fFiles ? fFiles->GetEntries() : 0;

    printf("Name            : %s\n", GetName());
    printf("Title           : %s\n", GetTitle());
    printf("Number of files : %d\n", nFiles);
    for (Int_t i = 0; i < nFiles; i++)
        printf("  %s\n", GetFile(i));
}

