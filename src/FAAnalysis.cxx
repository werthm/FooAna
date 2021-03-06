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
#include "TH2.h"
#include "TGraph.h"
#include "TCutG.h"
#include "TF1.h"
#include "ROOT/TTreeProcessorMP.hxx"

#include "FAAnalysis.h"
#include "FAUtils.h"
#include "FAProgressClient.h"
#include "FAVarFiller.h"
#include "FAAnalysisResult.h"

ClassImp(FAAnalysis)

//______________________________________________________________________________
FAAnalysis::FAAnalysis(EAnaMode mode, const Char_t* cfg, const Char_t* treeName)
    : TObject()
{
    // Constructor.

    // init members
    fMode = mode;
    fOrigGDir = gDirectory;
    fChain = 0;
    fProgSrvPort = 0;
    fResult = 0;

    // force batch mode
    gROOT->SetBatch();

    // load configuration
    gEnv->ReadFile(cfg, kEnvLocal);

    // prepare chain
    if (treeName)
        fChain = new TChain(treeName);
    else
        fChain = new TChain(gEnv->GetValue("FA.Analysis.TreeName", "null"));

    // add data to chain
    const Char_t* input = gEnv->GetValue("FA.Analysis.Input", "null");
    if (strcmp(input, "null"))
        fChain->Add(input);

    // set up progress monitoring
    if (gEnv->GetValue("FA.Analysis.Progress", 1))
        fProgSrvPort = FAUtils::LaunchProgressServer();

    // generate axes
    fAxis1 = CreateAxis(1);
    fAxis2 = CreateAxis(2);

    // load objects
    LoadObjects("TH1", fObj_TH1);
    LoadObjects("TH2", fObj_TH2);
    LoadObjects("TGraph", fObj_TGraph);
    LoadObjects("TCutG", fObj_TCutG);
    LoadObjects("TF1", fObj_TF1);
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
    for (TH1* i : fObj_TH1)
        delete i;
    for (TH2* i : fObj_TH2)
        delete i;
    for (TGraph* i : fObj_TGraph)
        delete i;
    for (TCutG* i : fObj_TCutG)
        delete i;
    for (TF1* i : fObj_TF1)
        delete i;
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
template <class T>
void FAAnalysis::LoadObjects(const Char_t* objName, std::vector<T*>& list)
{
    // Load objects used in analyses.

    // get number of objects
    Int_t n = LoadKeyInt(Form("FA.Analysis.Load.%s.N", objName), 0);

    // try to load objects
    TString last_name;
    for (Int_t i = 0; i < n; i++)
    {
        // get file and object name
        TString file = FAAnalysis::LoadKeyString(Form("FA.Analysis.Load.%s.%d.File", objName, i), "null");
        TString name = FAAnalysis::LoadKeyString(Form("FA.Analysis.Load.%s.%d.Name", objName, i), "null");

        // check file name
        if (file == "null")
        {
            ::Error("FAAnalysis::LoadObjects", "File name of object %d to load not found!", i);
            continue;
        }

        // check object name
        if (name == "null")
        {
            // try last used object name
            if (last_name != "")
            {
                name = last_name;
            }
            else
            {
                ::Error("FAAnalysis::LoadObjects", "Name of object %d to load not found!", i);
                continue;
            }
        }

        // expand the path
        TString file_ex(file);
        gSystem->ExpandPathName(file_ex);

        // load object
        T* obj = 0;
        FAUtils::LoadObject(file_ex.Data(), name.Data(), obj);

        // add to list
        if (obj)
        {
            ::Info("FAAnalysis::LoadObjects", "Loaded %s [%2d] '%s' from '%s'",
                   objName, i, obj->GetName(), file_ex.Data());
            list.push_back(obj);

            // save last name
            last_name = name;
        }
    }
}

//______________________________________________________________________________
TString FAAnalysis::LoadKeyString(const Char_t* name, const Char_t* defValue)
{
    // Load the config key 'name' of type string. Use 'defValue' as the
    // default value in case the key is undefined.

    TString s = "null";

    // try to load analysis mode-specific key
    if (fMode == kData)
        s = gEnv->GetValue(Form("%s.Data", name), "null");
    else if (fMode == kMC)
        s = gEnv->GetValue(Form("%s.MC", name), "null");

    // load generic key if undefined
    if (s == "null")
        s = gEnv->GetValue(name, defValue);

    return s;
}

//______________________________________________________________________________
Int_t FAAnalysis::LoadKeyInt(const Char_t* name, Int_t defValue)
{
    // Load the config key 'name' of type integer. Use 'defValue' as the
    // default value in case the key is undefined.

    Int_t i = 99999;

    // try to load analysis mode-specific key
    if (fMode == kData)
        i = gEnv->GetValue(Form("%s.Data", name), 99999);
    else if (fMode == kMC)
        i = gEnv->GetValue(Form("%s.MC", name), 99999);

    // load generic key if undefined
    if (i == 99999)
        i = gEnv->GetValue(name, defValue);

    return i;
}

//______________________________________________________________________________
Double_t FAAnalysis::LoadKeyDouble(const Char_t* name, Double_t defValue)
{
    // Load the config key 'name' of type double. Use 'defValue' as the
    // default value in case the key is undefined.

    Double_t d = 99999.99;

    // try to load analysis mode-specific key
    if (fMode == kData)
        d = gEnv->GetValue(Form("%s.Data", name), 99999.99);
    else if (fMode == kMC)
        d = gEnv->GetValue(Form("%s.MC", name), 99999.99);

    // load generic key if undefined
    if (d == 99999.99)
        d = gEnv->GetValue(name, defValue);

    return d;
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
    FAProgressClient* progress = 0;
    if (fProgSrvPort)
        progress = new FAProgressClient("localhost", fProgSrvPort);

    // start progress monitoring
    if (progress)
        progress->RequestInit(nEntries);

    // function wrapper to restore original gDirectory before calling 'func'
    auto funcWrapper = [&](TTreeReader& reader)
    {
        fOrigGDir->cd();
        return func(reader);
    };

    // process events
    if (nWorkers == 1)
    {
        // single-thread processing
        TTreeReader reader(fChain);
        fResult = funcWrapper(reader);
    }
    else
    {
        // multiprocess processing
        ROOT::TTreeProcessorMP workers(nWorkers);
        fResult = workers.Process(*fChain, funcWrapper);
    }

    // stop progress monitoring
    if (progress)
    {
        progress->RequestFinish();
        progress->RequestStop();
    }
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

    //
    // create total output
    //

    if (fResult->GetNFiles() == 1)
    {
        Info("WriteOutputFile", "Renaming output file to '%s'", outfile);
        gSystem->Rename(fResult->GetFile(0), outfile);
    }
    else
    {
        // user info
        Info("WriteOutputFile", "Merging %d partial output files into '%s'",
             fResult->GetNFiles(), outfile);

        // merge partial output files
        if (fResult->GetNFiles() < 20)
        {
            // standard merger
            TFileMerger merger(kFALSE);
            merger.OutputFile(outfile, "recreate");
            for (Int_t i = 0; i < fResult->GetNFiles(); i++)
            {
                if (!merger.AddFile(fResult->GetFile(i), kFALSE))
                    Error("WriteOutputFile", "Partial output file '%s' was not found!", fResult->GetFile(i));
            }
            merger.Merge();
        }
        else
        {
            // faster parallel merger
            TString cmd = TString::Format("hadd -f -j %d %s ",
                                          gEnv->GetValue("FA.Analysis.NWorker", 1),
                                          outfile);
            for (Int_t i = 0; i < fResult->GetNFiles(); i++)
            {
                cmd += fResult->GetFile(i);
                cmd += " ";
            }
            gSystem->Exec(cmd.Data());
        }

        // delete partial output files
        for (Int_t i = 0; i < fResult->GetNFiles(); i++)
            gSystem->Unlink(fResult->GetFile(i));
    }
}

//______________________________________________________________________________
void FAAnalysis::Print(Option_t* option) const
{
    // Print the content of this class.

    printf("################################\n");
    printf("FooAna %s\n", FOOANA_VERSION);
    printf("https://github.com/werthm/FooAna\n");
    printf("################################\n");
    printf("\n");
    printf("FAAnalysis configuration\n");
    printf("Analysis Mode                   : ");
    switch (fMode)
    {
        case kData:
            printf("Data\n"); break;
        case kMC:
            printf("MC\n"); break;
        default:
            printf("unknown\n");
    }
    printf("Number of workers               : %d\n", gEnv->GetValue("FA.Analysis.NWorker", 1));
    printf("Tree name                       : %s\n", fChain ? fChain->GetName() : "empty");
    printf("Files in chain                  : %d\n", fChain ? fChain->GetListOfFiles()->GetEntries() : 0);
    printf("Progress monitoring             : ");
    if (gEnv->GetValue("FA.Analysis.Progress", 1))
        printf("yes\n");
    else
        printf("no\n");
    printf("Axis 1 name                     : %s\n", fAxis1 ? fAxis1->GetName() : "empty");
    printf("Axis 1 bins                     : %d\n", fAxis1 ? fAxis1->GetNbins() : 0);
    printf("Axis 2 name                     : %s\n", fAxis2 ? fAxis2->GetName() : "empty");
    printf("Axis 2 bins                     : %d\n", fAxis2 ? fAxis2->GetNbins() : 0);
    printf("Loaded TH1 objects              : %lu\n", fObj_TH1.size());
    for (UInt_t i = 0; i < fObj_TH1.size(); i++)
        printf("  [%2u]  %s\n", i, fObj_TH1[i]->GetName());
    printf("Loaded TH2 objects              : %lu\n", fObj_TH2.size());
    for (UInt_t i = 0; i < fObj_TH2.size(); i++)
        printf("  [%2u]  %s\n", i, fObj_TH2[i]->GetName());
    printf("Loaded TGraph objects           : %lu\n", fObj_TGraph.size());
    for (UInt_t i = 0; i < fObj_TGraph.size(); i++)
        printf("  [%2u]  %s\n", i, fObj_TGraph[i]->GetName());
    printf("Loaded TCutG objects            : %lu\n", fObj_TCutG.size());
    for (UInt_t i = 0; i < fObj_TCutG.size(); i++)
        printf("  [%2u]  %s\n", i, fObj_TCutG[i]->GetName());
    printf("Loaded TF1 objects              : %lu\n", fObj_TF1.size());
    for (UInt_t i = 0; i < fObj_TF1.size(); i++)
        printf("  [%2u]  %s\n", i, fObj_TF1[i]->GetName());
}

//______________________________________________________________________________
FAAnalysisResult* FAAnalysis::WritePartialOutput(FAVarFiller& filler, const Char_t* inFile,
                                                 Bool_t flat)
{
    // Write the partial output from the filler 'filler' to a file in the current
    // directory using the input file name 'inFile'.
    // If 'flat' is kTRUE, a flat directory is structure is used.

    // build output file name
    TString fout = FAUtils::ExtractFileName(inFile) + ".out.part";

    // user info
    ::Info("FAAnalysis::WritePartialOutput", "Writing partial output file '%s'", fout.Data());

    // write partial output
    filler.WriteFile(fout.Data(), flat);

    // create mergeable list of partial output files
    return new FAAnalysisResult("FooAna_Result", fout.Data());
}

