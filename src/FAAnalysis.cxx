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


#include "TChain.h"
#include "TROOT.h"
#include "TEnv.h"
#include "TError.h"
#include "TTreeReader.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "ROOT/TTreeProcessorMP.hxx"

#include "FAAnalysis.h"
#include "FAUtils.h"
#include "FAProgressClient.h"
#include "FAVarFiller.h"
#include "FAAnalysisResult.h"

ClassImp(FAAnalysis)

//______________________________________________________________________________
FAAnalysis::FAAnalysis(const Char_t* cfg)
    : TObject()
{
    // Constructor.

    // init members
    fChain = 0;
    fProgSrvPort = 0;
    fResult = 0;

    // force batch mode
    gROOT->SetBatch();

    // load configuration
    gEnv->ReadFile(cfg, kEnvLocal);

    // load data
    fChain = new TChain(gEnv->GetValue("FA.Analysis.TreeName", "null"));
    const Char_t* input = gEnv->GetValue("FA.Analysis.Input", "null");
    if (strcmp(input, "null"))
        fChain->Add(input);

    // set up progress monitoring
    if (gEnv->GetValue("FA.Analysis.Progress", 1))
        fProgSrvPort = FAUtils::LaunchProgressServer();

    // generate axes
    fAxis1 = CreateAxis(1);
    fAxis2 = CreateAxis(2);
}

//______________________________________________________________________________
FAAnalysis::~FAAnalysis()
{
    // Destructor.

    if (fChain)
        delete fChain;
    if (fResult)
        delete fResult;
    if (fAxis1)
        delete fAxis1;
    if (fAxis2)
        delete fAxis2;
}

//______________________________________________________________________________
TAxis* FAAnalysis::CreateAxis(Int_t index)
{
    // Create and return the axis with index 'index'.

    // find axis config
    const Char_t* binsAxis = gEnv->GetValue(TString::Format("FA.Analysis.Axis%d.Binning",
                                                            index).Data(), "null");

    // create axis
    if (strcmp(binsAxis, "null"))
    {
        TAxis* a = 0;

        // check for file
        if (FAUtils::FileExists(binsAxis))
        {
            // try to read file
            FILE* fin = fopen(FAUtils::ExpandPath(binsAxis).Data(), "r");
            if (fin)
            {
                // read the file
                TString binning;
                Char_t buff[256];
                while (fgets(buff, sizeof(buff), fin) != 0)
                {
                    // edit line content
                    TString s(buff);
                    s.ReplaceAll("\n", "");
                    s.ReplaceAll("\r", "");
                    s.ReplaceAll(" ", "");

                    // concatenate binning
                    if (s != "")
                    {
                        binning += s;
                        binning += " ";
                    }
                }
                fclose(fin);

                // create axis
                a = new TAxis(FAUtils::CreateVariableAxis(binning.Data()));
            }
        }
        else
        {
            // direct bin edges
            a = new TAxis(FAUtils::CreateVariableAxis(binsAxis));
        }

        // check for name config
        if (a)
        {
            const Char_t* nameAxis = gEnv->GetValue(TString::Format("FA.Analysis.Axis%d.Name",
                                                    index).Data(), "null");
            if (strcmp(nameAxis, "null"))
                a->SetNameTitle(nameAxis, nameAxis);
        }

        return a;
    }
    else
    {
        return 0;
    }
}

//______________________________________________________________________________
void FAAnalysis::AddInput(const Char_t* path)
{
    // Add the path 'path' to the input chain.

    if (fChain)
        fChain->Add(path);
}

//______________________________________________________________________________
void FAAnalysis::Process(std::function<FAAnalysisResult* (TTreeReader&)> func)
{
    // Event processing function.

    // get number of files, entries and workers
    Long64_t nEntries = fChain->GetEntries();
    Int_t nFiles = fChain->GetListOfFiles()->GetEntries();
    Int_t nWorkers = gEnv->GetValue("FA.Analysis.NWorker", 1);
    if (nWorkers > nFiles)
        nWorkers = nFiles;

    // user info
    Info("Process", "%d file(s) in chain", nFiles);
    Info("Process", "%lld (%e) entries in chain", nEntries, (Double_t)nEntries);
    Info("Process", "Using %d worker(s)", nWorkers);

    // delete old result
    if (fResult)
        delete fResult;

    // connect to progress server
    FAProgressClient progress("localhost", fProgSrvPort);

    // start progress monitoring
    progress.RequestInit(nEntries);

    // process events
    ROOT::TTreeProcessorMP workers(nWorkers);
    fResult = workers.Process(*fChain, func);

    // stop progress monitoring
    progress.RequestFinish();
    progress.RequestStop();
}

//______________________________________________________________________________
void FAAnalysis::WriteOutputFile(const Char_t* out)
{
    // Write the output to the file 'out'.

    // check result
    if (!fResult)
    {
        Error("WriteOutputFile", "No analysis result to write!");
        return;
    }

    // output file
    const Char_t* outfile = out ? out : gEnv->GetValue("FA.Analysis.Output", "out.root");

    // user info
    Info("WriteOutputFile", "Merging %d partial output files into '%s'",
         fResult->GetNFiles(), outfile);

    // merge partial output files
    TFileMerger merger(kFALSE);
    merger.OutputFile(outfile, "recreate");
    for (Int_t i = 0; i < fResult->GetNFiles(); i++)
    {
        if (!merger.AddFile(fResult->GetFile(i), kFALSE))
            Error("WriteOutputFile", "Partial output file '%s' was not found!", fResult->GetFile(i));
    }
    merger.Merge();

    // delete partial output files
    for (Int_t i = 0; i < fResult->GetNFiles(); i++)
        gSystem->Unlink(fResult->GetFile(i));
}

//______________________________________________________________________________
FAAnalysisResult* FAAnalysis::WritePartialOutput(FAVarFiller& filler, const Char_t* inFile)
{
    // Write the partial output from the filler 'filler' to a file in the current
    // directory using the input file name 'inFile'.

    // build output file name
    TString fout = FAUtils::ExtractFileName(inFile) + ".out.part";

    // user info
    ::Info("FAAnalysis::WritePartialOutput", "Writing partial output file '%s'", fout.Data());

    // write partial output
    filler.WriteFile(fout.Data());

    // create mergeable list of partial output files
    return new FAAnalysisResult("FooAna_Result", fout.Data());
}

