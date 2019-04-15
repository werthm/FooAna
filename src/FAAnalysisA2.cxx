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
#include "TRandom.h"

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

    // load software trigger settings
    fTrig_CB_ESum_Mean = gEnv->GetValue("FA.Analysis.A2.Trig.CB.ESum.Mean", 0.);
    fTrig_CB_ESum_Sigma = gEnv->GetValue("FA.Analysis.A2.Trig.CB.ESum.Sigma", 0.);
    fTrig_Mult_Total = gEnv->GetValue("FA.Analysis.A2.Trig.Mult.Total", 0);
    fIsTrig_Mult_TAPS = (Bool_t)gEnv->GetValue("FA.Analysis.A2.Trig.Mult.UseTAPS", 1);
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

//______________________________________________________________________________
Bool_t FAAnalysisA2::IsTrigger(Double_t cb_esum, Int_t mult_cb, Int_t mult_taps) const
{
    // Return kTRUE if the trigger conditions specified in the analysis config
    // file are fulfilled for this event. Otherwise return kFALSE.

    // check CB energy sum trigger
    if (cb_esum < gRandom->Gaus(fTrig_CB_ESum_Mean, fTrig_CB_ESum_Sigma))
        return kFALSE;

    // check total multiplicity
    Int_t mult = mult_cb;
    if (fIsTrig_Mult_TAPS)
        mult += mult_taps;
    if (mult < fTrig_Mult_Total)
        return kFALSE;

    return kTRUE;
}

