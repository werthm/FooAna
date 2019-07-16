void TreeReader()
{
    // Main method.

    // create analysis
    FAAnalysisA2 ana("config.rootrc");

    // configure analysis
    ana.Print();

    // event processing lambda function
    auto ProcessEvents = [&](TTreeReader& reader)
    {
        // connect to progress server
        FAProgressClient progress("localhost", ana.GetProgressSrvPort());

        // configure reader
        TTreeReaderValue<FAEventA2_B> event(reader, "Event");

        // define analysis variables
        FAVarFiller filler("analysis", "1n1c analysis", kFALSE);
        FAVar<Short_t> tagg_ch(filler, "tagg_ch", "tagger channel", 0, ana.GetNTagg(), 0, ana.GetNTagg());
        FAVar<Float_t> eg(filler, "eg", "E_{#gamma}", "MeV", ana.GetAxis1());
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
        filler.SetBins1(ana.GetAxis1());
        //filler.SetBins2(ana.GetAxis2());
        //FAVarFiller::EFillMode fillMode = FAVarFiller::kUnbinned;
        FAVarFiller::EFillMode fillMode = FAVarFiller::kBinned;
        filler.Init(fillMode);

        // 4-vectors
        TLorentzVector p4Beam;

        // read events
        Long64_t n = 0;
        while (reader.Next())
        {
            // report processed events to progress server
            n++;
            if (n % 400000 == 0)
            {
                progress.ReportEvents(n);
                progress.PrintProgress();
                n = 0;
            }

            // primary cuts (on directly available tree data)
            // ...

            // set beam 4-vector
            Double_t eBeam = ana.GetTaggE(event->taggCh);
            p4Beam.SetPxPyPzE(0, 0, eBeam, eBeam);

            // set analysis variables
            tagg_ch = event->taggCh;
            eg = p4Beam.E();
            cb_esum = event->cbSum;
            for (Int_t i = 0; i < nPart; i++)
                part[i]->Set(event->particle(i));

            // seconday cuts (on newly calculated variables)
            // ...

            // event weights
            auto weighting = [&] { return event->weight; };

            // fill analysis variables
            filler.FillOverlap(weighting, ana.GetTaggE(event->taggCh),
                               ana.GetTaggEWidth(event->taggCh), 0);
        }

        // report processed events to progress server
        progress.ReportEvents(n);

        // write and register partial output file
        return FAAnalysis::WritePartialOutput(filler, reader.GetTree()->GetCurrentFile()->GetName());
    };

    // process events
    ana.Process(ProcessEvents);

    // write output file
    ana.WriteOutputFile();

    gSystem->Exit(0);
}

