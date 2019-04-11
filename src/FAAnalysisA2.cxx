/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FAAnalysisA2                                                         //
//                                                                      //
// FooAna A2 analysis wrapper.                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TEnv.h"

#include "FAAnalysisA2.h"
#include "FAUtilsA2.h"

ClassImp(FAAnalysisA2)

//______________________________________________________________________________
FAAnalysisA2::FAAnalysisA2(const Char_t* cfg)
    : FAAnalysis(cfg)
{
    // Constructor.

    // init members
    fNTagg = 0;
    fTaggE = 0;
    fTaggEWidth = 0;

    // load number of tagger channels
    fNTagg = gEnv->GetValue("FA.Analysis.A2.Tagger.Channels", 0);
    if (fNTagg)
    {
        fTaggE = new Double_t[fNTagg];
        fTaggEWidth = new Double_t[fNTagg];
    }

    // load tagger calibration
    if (fNTagg)
    {
        const Char_t* taggCal = gEnv->GetValue("FA.Analysis.A2.Tagger.Calib", "null");
        if (strcmp(taggCal, "null"))
            FAUtilsA2::LoadTaggerCalibration(taggCal, fNTagg, fTaggE, fTaggEWidth);
        else
            Warning("FAAnalysisA2", "Could not load tagger calibration!");
    }
}

//______________________________________________________________________________
FAAnalysisA2::~FAAnalysisA2()
{
    // Destructor.

    if (fTaggE)
        delete [] fTaggE;
    if (fTaggEWidth)
        delete [] fTaggEWidth;
}

