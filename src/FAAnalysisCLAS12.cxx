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


#include "TEnv.h"
#include "TDatabasePDG.h"

#include "FAAnalysisCLAS12.h"

ClassImp(FAAnalysisCLAS12)

//______________________________________________________________________________
FAAnalysisCLAS12::FAAnalysisCLAS12(EAnaMode mode, const Char_t* cfg, const Char_t* treeName)
    : FAAnalysis(mode, cfg, treeName)
{
    // Constructor.

    // init members
    fBeamE = gEnv->GetValue("FA.Analysis.CLAS12.Beam.Energy", 10.6);

    // extend particle database
    TDatabasePDG* db = TDatabasePDG::Instance();
    db->AddParticle("deuteron", "deuteron", 1.875612914, kTRUE,
                    0., 3., "deuteron", 45, -1, 45);
}

//______________________________________________________________________________
void FAAnalysisCLAS12::Print(Option_t* option) const
{
    // Print the content of this class.

    FAAnalysis::Print(option);
    printf("Beam energy                     : %.2f GeV\n", fBeamE);
}

