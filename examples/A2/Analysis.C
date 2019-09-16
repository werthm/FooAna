/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Analysis                                                             //
//                                                                      //
// Example ROOT macro showing how to analyze an event tree of A2 data.  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void Analysis()
{
    // Main method.

    // create analysis
    FAAnalysisA2 ana("config.rootrc");

    // configure analysis
    const Double_t kProtonMass = 938.27203;
    ana.Print();

    // event processing lambda function
    auto ProcessEvents = [&](TTreeReader& reader)
    {
        // connect to progress server
        FAProgressClient progress("localhost", ana.GetProgressSrvPort());

        // configure reader
        TTreeReaderValue<FAEventA2_B> event(reader, "Event");

        // define analysis variables
        FAVarFiller filler("analysis", "pi0 analysis", kFALSE);
        FAVar<Short_t> tagg_ch(filler, "tagg_ch", "tagger channel", 0, ana.GetNTagg(), 0, ana.GetNTagg());
        FAVar<Float_t> eg(filler, "eg", "E_{#gamma}", "MeV", ana.GetAxis1());
        FAVar<Float_t> im(filler, "im", "m(#gamma#gamma)", "MeV", 500, 0, 1000);
        filler.AddHistogram2D(&eg, &im);
        const Int_t nPart = 3;
        const Char_t* partName[nPart]  = { "g1", "g2", "p" };
        const Char_t* partTitle[nPart] = { "Photon 1", "Photon 2", "Proton" };
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

            // set particle 4-vectors
            FAVector4 p4Photon1 = FAUtilsA2::CalcVector4(event->part[0], 0);
            FAVector4 p4Photon2 = FAUtilsA2::CalcVector4(event->part[1], 0);
            FAVector4 p4Proton = FAUtilsA2::CalcVector4(event->part[2], kProtonMass);

            // set beam 4-vector
            Double_t eBeam = ana.GetTaggE(event->taggCh);
            FAVector4 p4Beam(0, 0, eBeam, eBeam);

            // set analysis variables
            tagg_ch = event->taggCh;
            eg = p4Beam.E();
            im = (p4Photon1 + p4Photon2).M();
            for (Int_t i = 0; i < nPart; i++)
                part[i]->Set(event->part[i]);

            // seconday cuts (on newly calculated variables)
            // ...

            // event weights
            auto weighting = [&] { return event->weight; };

            // fill analysis variables
            filler.Fill(weighting, ana.GetTaggE(event->taggCh));
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

