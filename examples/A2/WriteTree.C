/*************************************************************************
 * Author: Dominik Werthmueller, 2019
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// WriteTree                                                            //
//                                                                      //
// Example ROOT macro showing how to write an event tree of A2 data.    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void WriteTree()
{
    // Main method.

    // configuration
    Int_t nEvent = 100;

    // create the event object
    FAEventA2_B event;

    // open the output file
    TFile* f = new TFile("tree.root", "recreate");

    // create the tree
    TTree* tree = new TTree("events", "A2 event tree");
    tree->Branch("Event", "A2Events", &event);

    // loop over events
    for (Int_t i = 0; i < nEvent; i++)
    {
        // set event variables
        //event.weight =
        //event.taggCh =
        //event.cbSum =

        // add MC particle to event
        FAParticleA2MC_B partMC;
        //partMC.pdg =
        //partMC.vec4 =
        event.AddParticleMC(partMC);

        // add particle to event
        FAParticleA2_B part;
        //part.det =
        //part.detElem =
        //part.theta =
        //part.phi =
        //part.energy =
        //part.time =
        //part.deltaE =
        //part.tof =
        //part.psa_a =
        //part.psa_r =
        event.AddParticle(part);

        // fill the event
        tree->Fill();

        // clear the event
        event.Clear();
    }

    // write the tree and close the file
    tree->Write();
    delete f;

    gSystem->Exit(0);
}

