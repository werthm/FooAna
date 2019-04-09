FAProgressClient* gProgress;
TList* ProcessEvents(TTreeReader& reader);

//______________________________________________________________________________
void TreeReader(const Char_t* fin, const Char_t* fout)
{
    // Main method.

    // force batch mode
    gROOT->SetBatch();

    // load data
    TChain chain("events");
    chain.Add(fin);

    // user info
    Long64_t nEntries = chain.GetEntries();
    printf("Files in chain    : %d\n", chain.GetListOfFiles()->GetEntries());
    printf("Entries in chain  : %lld (%e)\n", nEntries, (Double_t)nEntries);

    // set up progress monitoring
    Int_t port = FAUtils::LaunchProgressServer();
    gProgress = new FAProgressClient("localhost", port);
    gProgress->RequestInit(nEntries);

    // process events (single thread)
    TTreeReader reader(&chain);
    TList* totalOutput = ProcessEvents(reader);

    // process events (multi-threaded)
    //ROOT::TTreeProcessorMP workers(4);
    //TList* totalOutput = workers.Process(chain, ProcessEvents);

    // user info
    gProgress->RequestFinish();

    // write output file
    TFile out(fout, "recreate");
    totalOutput->Write();

    // clean-up
    delete totalOutput;
    gProgress->RequestStop();

    gSystem->Exit(0);
}

//______________________________________________________________________________
TList* ProcessEvents(TTreeReader& reader)
{
    // Event processing method.

    // primary bin axis
    const Int_t nBins1 = 15;
    Double_t bins1[nBins1] = { 100, 150, 200, 250, 300, 350, 400, 450,
                               500, 550, 600, 650, 700, 750, 800 };
    TAxis* primAxis = new TAxis(nBins1-1, bins1);
    primAxis->SetNameTitle("Eg", "Eg");

    // load the tagger calibration
    const Int_t nTaggCh = 344;
    Double_t taggEnergy[nTaggCh];
    FAUtilsA2::LoadTaggerCalibration("data/calib_short", nTaggCh, taggEnergy);

    // configure reader
    TTreeReaderValue<FAEventA2_B> event(reader, "Event");

    // define analysis variables
    FAVarFiller filler("analysis", "1n1c analysis", kFALSE);
    FAVar<Short_t> tagg_ch(filler, "tagg_ch", "tagger channel", 0, nTaggCh, 0, nTaggCh);
    FAVar<Float_t> eg(filler, "eg", "E_{#gamma}", "MeV", primAxis);
    FAVar<Float_t> cb_esum(filler, "cb_esum", "CB energy sum", "MeV", 1000, 0, 2000);
    filler.AddHistogram2D(&eg, &cb_esum);
    const Int_t nPart = 2;
    const Char_t* partName[nPart]  = { "g", "p" };
    const Char_t* partTitle[nPart] = { "Photon", "Proton" };
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
    filler.SetBins1(primAxis);
    //filler.SetBins2(secAxis);
    //FAVarFiller::EFillMode fillMode = FAVarFiller::kUnbinned;
    FAVarFiller::EFillMode fillMode = FAVarFiller::kBinned;
    filler.Init(fillMode);

    // load cuts
    // ..

    // 4-vectors
    TLorentzVector p4Beam;

    // read events
    Long64_t n = 0;
    while (reader.Next())
    {
        // set beam 4-vector
        Double_t eBeam = taggEnergy[event->taggCh];
        p4Beam.SetPxPyPzE(0, 0, eBeam, eBeam);

        // set analysis variables
        tagg_ch = event->taggCh;
        eg = p4Beam.E();
        cb_esum = event->cbSum;
        for (Int_t i = 0; i < nPart; i++)
            *part[i] = *event->particle(i);

        // report processed events to progress server
        n++;
        if (n % 200000 == 0)
        {
            gProgress->AddProcessedEventsAndPrint(n);
            n = 0;
        }

        // cuts
        // ..

        // fill analysis variables
        filler.Fill(event->weight, eg.GetVar(), 0);
    }

    // report processed events to progress server
    gProgress->AddProcessedEventsAndPrint(n);

    // create list of output objects
    TList* output = new TList();
    output->SetOwner(kTRUE);
    filler.AddObjectsToList(output);

    return output;
}

