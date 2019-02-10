/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TreeReader                                                           //
//                                                                      //
// Example ROOT macro showing how to analyze a FooAna tree of A2 events //
// with a ROOT TTreeReader and a FAVarFiller.                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void TreeReader(const Char_t* fin, const Char_t* fout)
{
    // Main method.

    // set-up TChain
    TChain chain("events");
    chain.AddFile(fin);
    TTreeReader reader(&chain);

    // configure reader
    TTreeReaderValue<FAEventA2> event(reader, "Event");

    // define analysis variables
    FAVarFiller filler("analysis", "1n1c analysis");
    FAVar<Double_t> event_id(filler, "event_id", "event ID");
    FAVar<Float_t> weight(filler, "weight", "event weight");
    FAVar<Short_t> tagg_ch(filler, "tagg_ch", "tagger channel", 0, 304, 0, 304);
    FAVar<Float_t> eg(filler, "eg", "E_{#gamma}", "MeV", 64, 120, 760);

    // create analysis particles
    const Int_t nPart = 2;
    const Char_t* partName[nPart]  = { "gamma", "proton" };
    const Char_t* partTitle[nPart] = { "Gamma", "Proton" };
    FAVarParticleA2* part[nPart];
    for (Int_t i = 0; i < nPart; i++)
    {
        part[i] = new FAVarParticleA2(partName[i], partTitle[i]);
        part[i]->AddVarsKinematics();
        part[i]->AddVarsDeltaEE();
        part[i]->AddVarsTOF();
        part[i]->AddVarsPSA();
        filler.AddVariableList(part[i]);
    }

    // configure binning and filling mode
    //filler.SetBins1(primAxis);
    //filler.SetBins2(secAxis);
    //FAVarFiller::EFillMode fillMode = FAVarFiller::kUnbinned;
    FAVarFiller::EFillMode fillMode = FAVarFiller::kBinned;
    filler.Init(fillMode);

    // load the tagger calibration
    const Int_t nTaggCh = 328;
    Double_t taggEnergy[nTaggCh];
    FAUtilsA2::LoadTaggerCalibration("~/loc/calibration/Nov_18/Tagger_Energy/calib_short",
                                     nTaggCh, taggEnergy);

    // read events
    while (reader.Next())
    {
        // set analysis variables
        event_id = event->eventID;
        weight = event->weight;
        tagg_ch = event->taggCh;
        eg = taggEnergy[event->taggCh];

        // read particles
        for (Int_t i = 0; i < nPart; i++)
            *part[i] = event->part[i];

        // fill analysis variables
        filler.Fill(weight.GetVar());
    }

    // write output file
    filler.WriteFile(fout);

    gSystem->Exit(0);
}

